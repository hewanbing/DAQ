#include "dpc.h"
#include "MTimer.h"
DPC::DPC()
{
    initSHM();
    reader = RBuilder.newCharReader();
    parsePattern();
    evt=new Event();
    while(true){
         mStatus=getStatus();
         mAction=getAction();
         if(mStatus==mAction){
             if(mStatus==-1)break;
             if(mStatus==1)step();
         }else{
         switch(mAction){
         case -1:
             turnOff();
             break;
         case 0:
             turnOn();
             break;
         case 1:
             start();
             break;
         case 2:
             stop();
             break;
         }
     }
     }
     closeSHM();
}
void DPC::parsePattern(){
    if(patternSEM==NULL)return;
    sem_wait(patternSEM);
    string par=pattern;
    string erros;
    reader->parse(pattern,pattern+par.length(),&patternJson,&erros);
    sem_post(patternSEM);
}
void DPC::initSHM(){
    statusSHMID=shm_open(statusSHMName.c_str(),O_RDWR,S_IRUSR|S_IWUSR);
    bufferStatusSHMID=shm_open(bufferStatusSHMName.c_str(),O_RDWR,S_IRUSR|S_IWUSR);
    actionSHMID=shm_open(actionSHMName.c_str(),O_RDWR,S_IRUSR|S_IWUSR);
    patternSHMID=shm_open(patternSHMName.c_str(),O_RDWR,S_IRUSR|S_IWUSR);
    status = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,statusSHMID,0);
    bufferStatus = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,bufferStatusSHMID,0);
    action = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,actionSHMID,0);
    pattern = (char*)mmap(NULL, patternSHMSize, PROT_READ|PROT_WRITE, MAP_SHARED,patternSHMID,0);
    ftruncate(actionSHMID,sizeof(int));
    ftruncate(statusSHMID,sizeof(int));
    ftruncate(patternSHMID,patternSHMSize);
    ftruncate(bufferStatusSHMID,sizeof(int));
    patternSEM=sem_open(patternSEMName.c_str(),O_RDWR,0644,1);
    statusSEM=sem_open(statusSEMName.c_str(),O_RDWR,0644,1);;
    actionSEM=sem_open(actionSEMName.c_str(),O_RDWR,0644,1);
    bufferStatusSEM=sem_open(bufferStatusSEMName.c_str(),O_RDWR,0644,1);
}
void DPC::closeSHM(){
    munmap(action,sizeof(int));
    munmap(status,sizeof(int));
    munmap(pattern,patternSHMSize);
    munmap(bufferStatus,sizeof(int));
    sem_close(actionSEM);
    sem_close(statusSEM);
    sem_close(patternSEM);
    sem_close(bufferStatusSEM);
    actionSEM=NULL;
    statusSEM=NULL;
    patternSEM=NULL;
    bufferStatusSEM=NULL;
}
void DPC::setAction(int _action){
    if(actionSEM==NULL)return;
    sem_wait(actionSEM);
    action[0]=_action;
    sem_post(actionSEM);
}
void DPC::setStatus(int _status){
    if(statusSEM==NULL)return;
    mStatus=_status;
      sem_wait(statusSEM);
      status[0]=_status;
      sem_post(statusSEM);
}
void DPC::setBufferStatus(int _status){
    if(bufferStatusSEM==NULL)return;
    sem_wait(bufferStatusSEM);
    bufferStatus[0]=_status;
    sem_post(bufferStatusSEM);
}
int DPC::getAction(){
    if(actionSEM==NULL)return -1;
      int _action;
      sem_wait(actionSEM);
      _action=action[0];
      sem_post(actionSEM);
      return _action;
}
int DPC::getStatus(){
    if(statusSEM==NULL)return -1;
      int _status;
      sem_wait(statusSEM);
      _status=status[0];
      sem_post(statusSEM);
      return _status;
}
bool DPC::checkStatus(){
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
void DPC::createTree(){
    mRootFileName=patternJson["outputFileName"].asString()+"_";
    mRootFileName+=MTimer::getString();
    mRootFileName+=".root";
    string treeName=patternJson["treeName"].asString();
    mTree=new TTree(treeName.data(),treeName.data());
    mTree->Branch(patternJson["branchName"].asString().data(),evt,"channel[32]/F");
}
void DPC::saveTree(){
    if(mTree==NULL)return;
    nEvents=0;
    mRootFile=new TFile(mRootFileName,"RECREATE");
    mTree->Write();
    mRootFile->Close();
    mTree->Delete("all");
    mTree=NULL;
}
void DPC:: turnOn(){
    parsePattern();
    int eventBufferLength=1;
    int eventBufferNumber=10;
    if(patternJson.isMember("DPCBufferLength"))eventBufferLength=patternJson["DPCBufferLength"].asInt();
    if(patternJson.isMember("DPCBufferNumber"))eventBufferNumber=patternJson["DPCBufferNumber"].asInt();
    nEventsPerFile=100;
    if(patternJson.isMember("nEventsPerFile"))nEventsPerFile=patternJson["nEventsPerFile"].asInt();
    eventBuffer=new BufferTP<Event>(eventBufferLength,eventBufferNumber);

    mUDPThread=new thread(&DPC::UDPClient, this);

    //____________
    DAQDataBufferLength=patternJson["DAQBufferLength"].asInt();
    DAQDataBuffer=new Channel[DAQDataBufferLength];
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
       perror("socket creation failed");
       exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int UDPPort=60002;
    if(patternJson.isMember("DPCUDPPort"))UDPPort=patternJson["DPCUDPPort"].asInt();
    servaddr.sin_port = htons(UDPPort);
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void*)&portFlag,sizeof(portFlag));
    if(bind(sockfd,(const struct sockaddr*)&servaddr,sizeof(servaddr))<0){
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&timeout,sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO,&timeout,sizeof(timeout));
    setStatus(0);
}
void DPC::turnOff(){
//    mUDPThread->detach();
//    pthread_cancel(mUDPThread->native_handle());
    close(sockfd);
    shutdown(sockfd,2);
    setStatus(-1);
}
void DPC::start(){
    createTree();
    setStatus(1);
}

void DPC::stop(){
    saveTree();
    setStatus(2);
}
void DPC::UDPClient(){
    int MonitorSockfd;
    struct sockaddr_in MonitorServaddr;
    if((MonitorSockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
      perror("socket creation failed");
      exit(EXIT_FAILURE);
    }
    memset(&MonitorServaddr, 0, sizeof(MonitorServaddr));
    MonitorServaddr.sin_family = AF_INET;
    int MonitorUDPPort=60003;
    if(patternJson.isMember("MonitorUDPPort"))MonitorUDPPort=patternJson["MonitorUDPPort"].asInt();
    MonitorServaddr.sin_port = htons(MonitorUDPPort);
    MonitorServaddr.sin_addr.s_addr = inet_addr(patternJson["MonitorIP"].asString().c_str());
    int n;
    socklen_t len;
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(MonitorSockfd, SOL_SOCKET, SO_RCVTIMEO,&timeout,sizeof(timeout));
    setsockopt(MonitorSockfd, SOL_SOCKET, SO_SNDTIMEO,&timeout,sizeof(timeout));
    int responseBufferSize=1024;
    char responseBuffer[responseBufferSize];
    while(true){
      sendto(MonitorSockfd,
         eventBuffer->getBuffer4Read(),
         eventBuffer->getBufferCharSize(),
         0,
         (const struct sockaddr*)&MonitorServaddr,sizeof(MonitorServaddr));
      n = recvfrom(MonitorSockfd,(char*)responseBuffer,responseBufferSize,0,
           (struct sockaddr*)&MonitorServaddr,&len);
      if(n>0){
        eventBuffer->readDone();
      }
      setBufferStatus(eventBuffer->getStatus());
    }
}
void DPC::step(){
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    socklen_t len=sizeof(cliaddr);
    int n=recvfrom(sockfd,(char *)DAQDataBuffer,
           DAQDataBufferLength*sizeof(Channel)/sizeof(char),
           MSG_WAITALL,
           (struct sockaddr*)&cliaddr,&len);
    if(n>0){
        sendto(sockfd, "recived", strlen("recived"),MSG_CONFIRM,(const struct sockaddr*)&cliaddr,len);
        for(int i=0;i<DAQDataBufferLength;i++){
            if(evt->fillChannel(DAQDataBuffer[i])){
                memcpy(eventBuffer->getBuffer4Fill(1),evt,sizeof(Event));
                mTree->Fill();
                nEvents++;
                eventBuffer->fillDone();
                evt->reset();
            }
            if(nEvents>=nEventsPerFile){
                saveTree();
                createTree();
            }        
        }
    }
}
