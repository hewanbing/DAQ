#include "daq.h"
#include "VMUSB.h"
#include "v2718.h"

DAQ::DAQ()
{
    initSHM();
    reader = RBuilder.newCharReader();
    parsePattern();
    while(true){
        mStatus=getStatus();
        mAction=getAction();
        if(mStatus==mAction){
            if(mStatus==-1)break;
            switch(mStatus){
            case 2:
                step();
                break;
            case 5:
                step();
                break;
            }
        }else{
        switch(mAction){
        case -1:
            turnOff();
            break;
        case 0:
            turnOn();
            break;
        case 1:
            init();
            break;
        case 2:
            start();
            break;
        case 3:
            stop();
            break;
        case 4:
            pause();
            break;
        case 5:
            resume();
            break;
        }
    }
    }

    closeSHM();
}
void DAQ::parsePattern(){
    if(patternSEM==NULL)return;
    sem_wait(patternSEM);
    string par=pattern;
    string erros;
    reader->parse(pattern,pattern+par.length(),&patternJson,&erros);
    sem_post(patternSEM);
}
void DAQ::initSHM(){
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
    statusSEM=sem_open(statusSEMName.c_str(),O_RDWR,0644,1);
    bufferStatusSEM=sem_open(bufferStatusSEMName.c_str(),O_RDWR,0644,1);
    actionSEM=sem_open(actionSEMName.c_str(),O_RDWR,0644,1);
}
void DAQ::closeSHM(){
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
void DAQ::setAction(int _action){
    if(actionSEM==NULL)return;
    sem_wait(actionSEM);
    action[0]=_action;
    sem_post(actionSEM);
}
void DAQ::setStatus(int _status){
    if(statusSEM==NULL)return;
    mStatus=_status;
      sem_wait(statusSEM);
      status[0]=_status;
      sem_post(statusSEM);
}
void DAQ::setBufferStatus(int _status){
    if(bufferStatusSEM==NULL)return;
    sem_wait(bufferStatusSEM);
    bufferStatus[0]=_status;
    sem_post(bufferStatusSEM);
}
int DAQ::getAction(){
    if(actionSEM==NULL)return -1;
      int _action;
      sem_wait(actionSEM);
      _action=action[0];
      sem_post(actionSEM);
      return _action;
}
int DAQ::getStatus(){
    if(statusSEM==NULL)return -1;
      int _status;
      sem_wait(statusSEM);
      _status=status[0];
      sem_post(statusSEM);
      return _status;
}
bool DAQ::checkStatus(){
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
void DAQ::UDPClient(){
    int sockfd;
    struct sockaddr_in servaddr;
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
      perror("socket creation failed");
      exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    int DPCUDPPort=60002;
    if(patternJson.isMember("DPCUDPPort"))DPCUDPPort=patternJson["DPCUDPPort"].asInt();
    servaddr.sin_port = htons(DPCUDPPort);
    servaddr.sin_addr.s_addr = inet_addr(patternJson["DPCIP"].asString().c_str());
    int n;
    socklen_t len;
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&timeout,sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO,&timeout,sizeof(timeout));
  ;
    int responseBufferSize=1024;
    char buffer[responseBufferSize];
    while(true){
      sendto(sockfd,
             handle->getBuffer4Read(),
             handle->getBufferCharSize(),
             0,
             (const struct sockaddr*)&servaddr,sizeof(servaddr));
      n = recvfrom(sockfd,(char*)buffer,responseBufferSize,0,
               (struct sockaddr*)&servaddr,&len);
      if(n>0){
        handle->readDone();
      }
      setBufferStatus(handle->getBufferStatus());//bufferStatus
    }
}
void DAQ::turnOn(){
    parsePattern();
    setupHandle();
    startDataSender();
    setStatus(0);    
}
void DAQ::setupHandle(){
    string handleType=patternJson["Controller"].asString();
    if(strcmp(handleType.c_str(),"VMUSB")==0){
        handle=new VMUSB(patternJson);
    }
    if(strcmp(handleType.c_str(),"v2718")==0){
        handle=new v2718(patternJson);
    }
}
void DAQ::startDataSender(){
    mUDPThread=new thread(&DAQ::UDPClient, this);
}
void DAQ::init(){
    if(handle->init())setStatus(1);
}
void DAQ::start(){
    if(handle->start())setStatus(2);
}
void DAQ::stop(){
    if(handle->stop()) setStatus(3);
}
void DAQ::pause(){
    if(handle->pause()) setStatus(4);
}
void DAQ::resume(){
    if(handle->resume())setStatus(5);
}
void DAQ::turnOff(){
    stopDataSender();
    setStatus(-1);
}
void DAQ::step(){
    handle->step();
}
void DAQ::stopDataSender(){
    mUDPThread->detach();
    pthread_cancel(mUDPThread->native_handle());
}
