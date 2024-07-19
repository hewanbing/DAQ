#include "wss.h"
#include "MString.h"
WSS::WSS(const char* WSPort,const char* _user)
{
    clearSHMCaches();
    //initSHM();
    initPatternSHM();
    user=_user;
    reader = RBuilder.newCharReader();
    writer = WBuilder.newStreamWriter();
    WSServer.set_reuse_addr(true);
    WSServer.set_access_channels(websocketpp::log::alevel::all);
    WSServer.clear_access_channels(websocketpp::log::alevel::frame_payload);
    WSServer.init_asio();
    WSServer.set_open_handler(bind(&WSS::onWSOpen, this, ::_1));
    WSServer.set_close_handler(bind(&WSS::onWSClose, this, ::_1));
    WSServer.set_message_handler(bind(&WSS::onWSMessage, this, ::_1, ::_2));
    WSServer.listen(MString::parse(WSPort).getInt());
    WSServer.start_accept();
    WSServer.run();
}
WSS::~WSS(){
}
void WSS::setDAQAction(int _action){
    if(DAQActionSEM==NULL)return;
    mDAQAction=_action;
    sem_wait(DAQActionSEM);
    DAQAction[0]=_action;
    sem_post(DAQActionSEM);
}
void WSS::setDAQStatus(int status){
  if(DAQStatusSEM==NULL)return;
    sem_wait(DAQStatusSEM);
    DAQStatus[0]=status;
    sem_post(DAQStatusSEM);
}
void WSS::setDAQBufferStatus(int status){//bufferStatus
  if(DAQBufferStatusSEM==NULL)return;
    sem_wait(DAQBufferStatusSEM);
    DAQBufferStatus[0]=status;
    sem_post(DAQBufferStatusSEM);
}
int WSS::getDAQStatus(){
  if(DAQStatusSEM==NULL)return -1;
    int status;
    sem_wait(DAQStatusSEM);
    status=DAQStatus[0];
    sem_post(DAQStatusSEM);
    return status;
}
int WSS::getDAQBufferStatus(){//bufferStatus
  if(DAQBufferStatusSEM==NULL)return -2;
    int status;
    sem_wait(DAQBufferStatusSEM);
    status=DAQBufferStatus[0];
    sem_post(DAQBufferStatusSEM);
    return status;
}
bool WSS::checkDAQStatus(){
    bool check=false;
    for(int i=0;i<100;i++){
        if(mDAQAction==getDAQStatus()){
            check=true;
            break;
        }
        usleep(100);
    }
    return check;
}
void WSS::setDPCAction(int _action){
  if(DPCActionSEM==NULL)return;
    mDPCAction=_action;
    sem_wait(DPCActionSEM);
    DPCAction[0]=_action;
    sem_post(DPCActionSEM);
}
void WSS::setDPCStatus(int status){
  if(DPCStatusSEM==NULL)return;
    sem_wait(DPCStatusSEM);
    DPCStatus[0]=status;
    sem_post(DPCStatusSEM);
}
void WSS::setDPCBufferStatus(int status){//bufferStatus
  if(DPCBufferStatusSEM==NULL)return;
    sem_wait(DPCBufferStatusSEM);
    DPCBufferStatus[0]=status;
    sem_post(DPCBufferStatusSEM);
}
int WSS::getDPCStatus(){
  if(DPCStatusSEM==NULL)return -1;
    int status;
    sem_wait(DPCStatusSEM);
    status=DPCStatus[0];
    sem_post(DPCStatusSEM);
    return status;
}
int WSS::getDPCBufferStatus(){//bufferStatus
  if(DPCBufferStatusSEM==NULL)return -2;
    int status;
    sem_wait(DPCBufferStatusSEM);
    status=DPCBufferStatus[0];
    sem_post(DPCBufferStatusSEM);
    return status;
}
bool WSS::checkDPCStatus(){
    bool check=false;
    for(int i=0;i<100;i++){
        if(mDPCAction==getDPCStatus()){
            check=true;
            break;
        }
        usleep(100);
    }
    return check;
}
void WSS::setMonAction(int _action){
  if(MonActionSEM==NULL)return;
    mMonAction=_action;
    sem_wait(MonActionSEM);
    MonAction[0]=_action;
    sem_post(MonActionSEM);
}
void WSS::setMonStatus(int status){
  if(MonStatusSEM==NULL)return;
    sem_wait(MonStatusSEM);
    MonStatus[0]=status;
    sem_post(MonStatusSEM);
}
int WSS::getMonStatus(){
  if(MonStatusSEM==NULL)return -1;
    int status;
    sem_wait(MonStatusSEM);
    status=MonStatus[0];
    sem_post(MonStatusSEM);
    return status;
}
bool WSS::checkMonStatus(){
    bool check=false;
    for(int i=0;i<100;i++){
        if(mMonAction==getMonStatus()){
            check=true;
            break;
        }
        usleep(100);
    }
    return check;
}
void WSS::closeSHM(){
    closeDAQSHM();
    closeDPCSHM();
    closeMonSHM();
    closePatternSHM();
}
void WSS::closePatternSHM(){
    munmap(pattern,patternSHMSize);
    shm_unlink(patternSHMName.c_str());
    sem_unlink(patternSEMName.c_str());
    sem_close(patternSEM);
    patternSEM=NULL;
}
void WSS::closeDAQSHM(){
    munmap(DAQAction,sizeof(int));
    shm_unlink(DAQActionSHMName.c_str());
    sem_unlink(DAQActionSEMName.c_str());
    sem_close(DAQActionSEM);
    munmap(DAQStatus,sizeof(int));
    shm_unlink(DAQStatusSHMName.c_str());
    sem_unlink(DAQStatusSEMName.c_str());
    sem_close(DAQStatusSEM);
    munmap(DAQBufferStatus,sizeof(int));//bufferStatus
    shm_unlink(DAQBufferStatusSHMName.c_str());//bufferStatus
    sem_unlink(DAQBufferStatusSEMName.c_str());//bufferStatus
    sem_close(DAQBufferStatusSEM);//bufferStatus
    DAQActionSEM=NULL;
    DAQStatusSEM=NULL;
    DAQBufferStatusSEM=NULL;//bufferStatus
}
void WSS::closeDPCSHM(){
    munmap(DPCAction,sizeof(int));
    shm_unlink(DPCActionSHMName.c_str());
    sem_unlink(DPCActionSEMName.c_str());
    sem_close(DPCActionSEM);
    munmap(DPCStatus,sizeof(int));
    shm_unlink(DPCStatusSHMName.c_str());
    sem_unlink(DPCStatusSEMName.c_str());
    sem_close(DPCStatusSEM);
    munmap(DPCBufferStatus,sizeof(int));//bufferStatus
    shm_unlink(DPCBufferStatusSHMName.c_str());//bufferStatus
    sem_unlink(DPCBufferStatusSEMName.c_str());//bufferStatus
    sem_close(DPCBufferStatusSEM);//bufferStatus
    DPCActionSEM=NULL;
    DPCStatusSEM=NULL;
    DPCBufferStatusSEM=NULL;//bufferStatus
}
void WSS::closeMonSHM(){
    munmap(MonAction,sizeof(int));
    shm_unlink(MonActionSHMName.c_str());
    sem_unlink(MonActionSEMName.c_str());
    sem_close(MonActionSEM);
    munmap(MonStatus,sizeof(int));
    shm_unlink(MonStatusSHMName.c_str());
    sem_unlink(MonStatusSEMName.c_str());
    sem_close(MonStatusSEM);
    MonActionSEM=NULL;
    MonStatusSEM=NULL;
}
void WSS::initSHM(){
    initDAQSHM();
    initDPCSHM();
    initMonSHM();
    initPatternSHM();
}
void WSS::initPatternSHM(){
    patternSEM=sem_open(patternSEMName.c_str(),O_RDWR|O_CREAT,0644,1);
    patternSHMID=shm_open(patternSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    ftruncate(patternSHMID,patternSHMSize);
    pattern = (char*)mmap(NULL, patternSHMSize, PROT_READ|PROT_WRITE, MAP_SHARED,patternSHMID,0);
}
void WSS::initDAQSHM(){
    DAQActionSEM=sem_open(DAQActionSEMName.c_str(),O_RDWR|O_CREAT,0644,1);
    DAQStatusSEM=sem_open(DAQStatusSEMName.c_str(),O_RDWR|O_CREAT,0644,1);
    DAQBufferStatusSEM=sem_open(DAQBufferStatusSEMName.c_str(),O_RDWR|O_CREAT,0644,1);//bufferStatus
    DAQActionSHMID=shm_open(DAQActionSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    DAQStatusSHMID=shm_open(DAQStatusSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    DAQBufferStatusSHMID=shm_open(DAQBufferStatusSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);//bufferStatus
    ftruncate(DAQActionSHMID,sizeof(int));
    ftruncate(DAQStatusSHMID,sizeof(int));
    ftruncate(DAQBufferStatusSHMID,sizeof(int));//bufferStatus
    DAQAction = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,DAQActionSHMID,0);
    DAQStatus = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,DAQStatusSHMID,0);
    DAQBufferStatus = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,DAQBufferStatusSHMID,0);//bufferStatus
    setDAQAction(-1);
    setDAQStatus(-1);
    setDAQBufferStatus(-2);
}
void WSS::initDPCSHM(){
    DPCActionSEM=sem_open(DPCActionSEMName.c_str(),O_RDWR|O_CREAT,0644,1);
    DPCStatusSEM=sem_open(DPCStatusSEMName.c_str(),O_RDWR|O_CREAT,0644,1);
    DPCBufferStatusSEM=sem_open(DPCBufferStatusSEMName.c_str(),O_RDWR|O_CREAT,0644,1);//bufferStatus
    DPCActionSHMID=shm_open(DPCActionSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    DPCStatusSHMID=shm_open(DPCStatusSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    DPCBufferStatusSHMID=shm_open(DPCBufferStatusSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);//bufferStatus
    ftruncate(DPCActionSHMID,sizeof(int));
    ftruncate(DPCStatusSHMID,sizeof(int));
    ftruncate(DPCBufferStatusSHMID,sizeof(int));//bufferStatus
    DPCAction = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,DPCActionSHMID,0);
    DPCStatus = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,DPCStatusSHMID,0);
    DPCBufferStatus = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,DPCBufferStatusSHMID,0);//bufferStatus
    setDPCAction(-1);
    setDPCStatus(-1);
    setDPCBufferStatus(-2);
}
void WSS::initMonSHM(){
    MonActionSEM=sem_open(MonActionSEMName.c_str(),O_RDWR|O_CREAT,0644,1);
    MonStatusSEM=sem_open(MonStatusSEMName.c_str(),O_RDWR|O_CREAT,0644,1);
    MonActionSHMID=shm_open(MonActionSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    MonStatusSHMID=shm_open(MonStatusSHMName.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    ftruncate(MonActionSHMID,sizeof(int));
    ftruncate(MonStatusSHMID,sizeof(int));
    MonAction = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,MonActionSHMID,0);
    MonStatus = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED,MonStatusSHMID,0);
    setMonAction(-1);
    setMonStatus(-1);    
}
void WSS::writePattern2SHM(const char* str,int length){
  if(patternSEM==NULL)return;
    sem_wait(patternSEM);
    memcpy(pattern,str,length);
    sem_post(patternSEM);
}
void WSS::clearSHMCaches(){
  system("rm /dev/shm/*DAQ*");
  system("rm /dev/shm/*DPC*");
  system("rm /dev/shm/*Mon*");
  system("rm /dev/shm/*pattern*");
}
void WSS::turnOnDAQ(){
    initDAQSHM();
    setDAQAction(0);
    DAQProc=popen("/home/Qt/build-DAQ-Desktop_Qt_5_14_2_GCC_64bit-Release/DAQ","r");
};
void WSS::turnOffDAQ(){
    setDAQAction(-1);
    checkDAQStatus();
    pclose(DAQProc);
    closeDAQSHM();
    system("rm /dev/shm/*DAQ*");
};
void WSS::turnOnDPC(){
    initDPCSHM();
    setDPCAction(0);
    DPCProc=popen("/home/Qt/build-DPC-Desktop_Qt_5_14_2_GCC_64bit-Release/DPC","r");
};
void WSS::turnOffDPC(){
    setDPCAction(-1);
    checkDPCStatus();
    pclose(DPCProc);
    closeDPCSHM();
    system("rm /dev/shm/*DPC*");
};
void WSS::turnOnMon(){
    initMonSHM();
    setMonAction(0);
    MonProc=popen("/home/Qt/build-Monitor-Desktop_Qt_5_14_2_GCC_64bit-Release/Monitor","r");
};
void WSS::turnOffMon(){
    setMonAction(-1);
    checkMonStatus();
    pclose(MonProc);
    closeMonSHM();
    system("rm /dev/shm/*Mon*");
};
void WSS::WSResponseDAQStatus(websocketpp::connection_hdl hdl){
  Json::Value response;
  response["DAQStatus"]=Json::Value(getDAQStatus());
  WSServer.send(hdl, Json::writeString(WBuilder, response), websocketpp::frame::opcode::text);
}
void WSS::WSResponseDAQBufferStatus(websocketpp::connection_hdl hdl){//bufferStatus
  Json::Value response;
  response["DAQBufferStatus"]=Json::Value(getDAQBufferStatus());
  WSServer.send(hdl, Json::writeString(WBuilder, response), websocketpp::frame::opcode::text);
}
void WSS::WSResponseDPCStatus(websocketpp::connection_hdl hdl){
  Json::Value response;
  response["DPCStatus"]=Json::Value(getDPCStatus());
  WSServer.send(hdl, Json::writeString(WBuilder, response), websocketpp::frame::opcode::text);
}
void WSS::WSResponseDPCBufferStatus(websocketpp::connection_hdl hdl){//bufferStatus
  Json::Value response;
  response["DPCBufferStatus"]=Json::Value(getDPCBufferStatus());
  WSServer.send(hdl, Json::writeString(WBuilder, response), websocketpp::frame::opcode::text);
}
void WSS::WSResponseMonStatus(websocketpp::connection_hdl hdl){
  Json::Value response;
  response["MonStatus"]=Json::Value(getMonStatus());
  WSServer.send(hdl, Json::writeString(WBuilder, response), websocketpp::frame::opcode::text);
}
void WSS::onWSOpen(websocketpp::connection_hdl hdl){}
void WSS::onWSClose(websocketpp::connection_hdl hdl){}
void WSS::onWSMessage(websocketpp::connection_hdl hdl, message_ptr msg){
    string strMsg = msg->get_payload();
    cout<<strMsg<<endl;
    bool parsingSuccessful = reader->parse(strMsg.c_str(),
                                           strMsg.c_str() + strMsg.size(),
                                           &root,
                                           &errors);
    if(!parsingSuccessful){
        Json::Value response;
        response["error"]=Json::Value("message format incorrect");
        WSServer.send(hdl, Json::writeString(WBuilder, response), websocketpp::frame::opcode::text);
        return;
    }
    if(strcmp(user.c_str(),root["user"].asString().c_str())!=0) {
        Json::Value response;
        response["error"]=Json::Value("unknown user");
        WSServer.send(hdl, Json::writeString(WBuilder, response), websocketpp::frame::opcode::text);
        return ;
    }
    if(strcmp("update pattern",root["action"].asString().c_str())==0){
      writePattern2SHM(strMsg.c_str(),strMsg.length());
        return;
    }
    //_______________________________________________________
    if(strcmp("DAQ",root["Machine"].asString().c_str())==0){
        if(strcmp("TurnOn",root["action"].asString().c_str())==0){
	  turnOnDAQ();
	  checkDAQStatus();
	  WSResponseDAQStatus(hdl);
	  return ;
        }
        if(strcmp("TurnOff",root["action"].asString().c_str())==0){
	  turnOffDAQ();
	  checkDAQStatus();
	  WSResponseDAQStatus(hdl);
	  return ;
        }
        if(strcmp("status request",root["action"].asString().c_str())==0){
	  WSResponseDAQStatus(hdl);
            return ;
        }
        if(strcmp("buffer status request",root["action"].asString().c_str())==0){//bufferStatus
      WSResponseDAQBufferStatus(hdl);
            return ;
        }
        if(strcmp("Init",root["action"].asString().c_str())==0){
            setDAQAction(1);
            checkDAQStatus();
	    WSResponseDAQStatus(hdl);	    
            return ;
        }
        if(strcmp("Start",root["action"].asString().c_str())==0){
            setDAQAction(2);
            checkDAQStatus();
	    WSResponseDAQStatus(hdl);
            return ;
        }
        if(strcmp("Stop",root["action"].asString().c_str())==0){
            setDAQAction(3);
            checkDAQStatus();
	  WSResponseDAQStatus(hdl);
            return ;
        }
        if(strcmp("Pause",root["action"].asString().c_str())==0){
            setDAQAction(4);
            checkDAQStatus();
	    WSResponseDAQStatus(hdl);
            return ;
        }
        if(strcmp("Resume",root["action"].asString().c_str())==0){
            setDAQAction(5);
            checkDAQStatus();
	  WSResponseDAQStatus(hdl);	    
            return ;
        }
    }
    //_______________________________________________________
    if(strcmp("DPC",root["Machine"].asString().c_str())==0){
        if(strcmp("TurnOn",root["action"].asString().c_str())==0){
	  turnOnDPC();
	  checkDPCStatus();
	  WSResponseDPCStatus(hdl);
	  return ;
        }
        if(strcmp("TurnOff",root["action"].asString().c_str())==0){
	  turnOffDPC();
	  checkDPCStatus();
	  WSResponseDPCStatus(hdl);
	  return ;
        }
        if(strcmp("status request",root["action"].asString().c_str())==0){
	  WSResponseDPCStatus(hdl);
            return ;
        }
        if(strcmp("buffer status request",root["action"].asString().c_str())==0){
      WSResponseDPCBufferStatus(hdl);
            return ;
        }
        if(strcmp("Start",root["action"].asString().c_str())==0){
            setDPCAction(1);
            checkDPCStatus();
	    WSResponseDPCStatus(hdl);
            return ;
        }
        if(strcmp("Stop",root["action"].asString().c_str())==0){
            setDPCAction(2);
            checkDPCStatus();
	  WSResponseDPCStatus(hdl);
            return ;
        }
    }
    //_______________________________________________________
    if(strcmp("Monitor",root["Machine"].asString().c_str())==0){
        if(strcmp("TurnOn",root["action"].asString().c_str())==0){
	  turnOnMon();
	  checkMonStatus();
	  WSResponseMonStatus(hdl);
	  return ;
        }
        if(strcmp("TurnOff",root["action"].asString().c_str())==0){
	  turnOffMon();
	  checkMonStatus();
	  WSResponseMonStatus(hdl);
	  return ;
        }
        if(strcmp("status request",root["action"].asString().c_str())==0){
	  WSResponseMonStatus(hdl);
            return ;
        }
        if(strcmp("Clean Data",root["action"].asString().c_str())==0){
            setMonAction(1);
            checkMonStatus();
	    WSResponseMonStatus(hdl);
            return ;
        }
    }
}
