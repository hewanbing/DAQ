#include "monitor.h"
#include "sstream"
#include <TFile.h>
#include <TMemFile.h>
#include <TNtuple.h>
#include <TH2.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TRandom3.h>
#include <TBenchmark.h>
#include <TInterpreter.h>
#include <THttpServer.h>
#include "Event.h"

using namespace std;
monitor::monitor(){
    initSHM();
    reader = RBuilder.newCharReader();
    while(true){
        mStatus=getStatus();
        mAction=getAction();
        if(mStatus==mAction){
            if(mStatus==-1)break;
            if(mStatus==0)step();
        }else{
            switch(mAction){
                case -1:
                    turnOff();
                    break;
                case 0:
                    turnOn();
                    break;
                case 1:
                    cleanData();
                    break;
            }
        }
    }
    closeSHM();
}
monitor::~monitor(){
    for(int i=0;i<32;i++){
      ADC[i]->Delete();
    }
    ADC2D->Delete();
}
void monitor::initSHM(){
    statusSHMID=shm_open(statusSHMName.c_str(),O_RDWR,S_IRUSR|S_IWUSR);
    actionSHMID=shm_open(actionSHMName.c_str(),O_RDWR,S_IRUSR|S_IWUSR);
    patternSHMID=shm_open(patternSHMName.c_str(),O_RDWR,S_IRUSR|S_IWUSR);
    status = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,statusSHMID,0);
    action = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,actionSHMID,0);
    pattern = (char*)mmap(NULL, patternSHMSize, PROT_READ|PROT_WRITE, MAP_SHARED,patternSHMID,0);
    ftruncate(actionSHMID,sizeof(int));
    ftruncate(statusSHMID,sizeof(int));
    ftruncate(patternSHMID,patternSHMSize);
    patternSEM=sem_open(patternSEMName.c_str(),O_RDWR,0644,1);
    statusSEM=sem_open(statusSEMName.c_str(),O_RDWR,0644,1);;
    actionSEM=sem_open(actionSEMName.c_str(),O_RDWR,0644,1);
}
void monitor::closeSHM(){
    munmap(action,sizeof(int));
    munmap(status,sizeof(int));
    munmap(pattern,patternSHMSize);
    sem_close(actionSEM);
    sem_close(statusSEM);
    sem_close(patternSEM);
    actionSEM=NULL;
    statusSEM=NULL;
    patternSEM=NULL;
}
void monitor::setAction(int _action){
    if(actionSEM==NULL)return;
    sem_wait(actionSEM);
    action[0]=_action;
    sem_post(actionSEM);
}
void monitor::setStatus(int _status){
    if(statusSEM==NULL)return;
    mStatus=_status;
      sem_wait(statusSEM);
      status[0]=_status;
      sem_post(statusSEM);
}
int monitor::getAction(){
    if(actionSEM==NULL)return -1;
      int _action;
      sem_wait(actionSEM);
      _action=action[0];
      sem_post(actionSEM);
      return _action;
}
int monitor::getStatus(){
    if(statusSEM==NULL)return -1;
      int _status;
      sem_wait(statusSEM);
      _status=status[0];
      sem_post(statusSEM);
      return _status;
}
bool monitor::checkStatus(){
    bool check=false;
    for(int i=0;i<100;i++){
        if(mStatus==getAction()){
            check=true;
            break;
        }
        usleep(100);
    }
    return check;
}
void monitor::parsePattern(){
    if(patternSEM==NULL)return;
    sem_wait(patternSEM);
    string par=pattern;
    string erros;
    reader->parse(pattern,pattern+par.length(),&patternJson,&erros);
    sem_post(patternSEM);
}
void monitor::turnOn(){
    parsePattern();
    setupROOTObjects();
    setupBuffer();
    setupJSROOT();
    setupUDPServer();
    setStatus(0);
}
void monitor::setupBuffer(){
    eventBufferLength=patternJson["DPCBufferLength"].asInt();
    buffer=new Event[eventBufferLength];
    eventBufferCharSize=eventBufferLength*sizeof(Event)/sizeof(char);
}
void monitor::setupJSROOT(){
    int port=8080;
    if(patternJson.isMember("JSROOTPort"))port=patternJson["JSROOTPort"].asInt();
    serv = new THttpServer(Form("http:%d?top=%s",port, jobname));
    serv->SetReadOnly(kFALSE);
}
void monitor::setupROOTObjects(){
    for(int i=0;i<32;i++){
        string name="ADCChannel";
        name+=to_string(i);
        string title="ADCChannel";
        title+=to_string(i);
        ADC[i] = new TH1F(name.data(),title.data(),100,0,4000);
        ADC[i]->SetFillColor(48);
    }
    ADC2D=new TH2D("TH2D","Channel 1 VS 2",100,0,4000,100,0,4000);
    ADC2D->SetFillColor(48);
}
void monitor::setupUDPServer(){
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int UDPPort=60003;
    if(patternJson.isMember("MonitorUDPPort"))UDPPort=patternJson["MonitorUDPPort"].asInt();
    servaddr.sin_port = htons(UDPPort);
    if(bind(sockfd,(const struct sockaddr*)&servaddr,sizeof(servaddr))<0){
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&timeout,sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO,&timeout,sizeof(timeout));
}
void monitor::turnOff(){
    closeUDPServer();
    setStatus(-1);
}
void monitor::closeUDPServer(){
    close(sockfd);
    shutdown(sockfd,2);
    //serv->Delete();
}
void monitor::cleanData(){
    for(int i=0;i<32;i++){
        ADC[i]->Reset("ICESM");
    }
    ADC2D->Reset("ICESM");
    serv->ProcessRequests();
    setAction(0);
}
void monitor::step(){
  struct sockaddr_in cliaddr;
  memset(&cliaddr, 0, sizeof(cliaddr));
  socklen_t len = sizeof(cliaddr);
  int n=recvfrom(sockfd,(char *)buffer,
                 eventBufferCharSize,
                 MSG_WAITALL,
                 (struct sockaddr*)&cliaddr,
                 &len);
  if(n>0){
      sendto(sockfd, "recived", strlen("recived"),MSG_CONFIRM,
             (const struct sockaddr*)&cliaddr,len);
      for(int i=0;i<eventBufferLength;i++){
          for(int j=0;j<32;j++){
              ADC[j]->Fill(buffer[i].getADCChannel(j).value);
          }
          ADC2D->Fill(buffer[i].getADCChannel(1).value,buffer[i].getADCChannel(2).value);
      }
  }
  serv->ProcessRequests();
}
