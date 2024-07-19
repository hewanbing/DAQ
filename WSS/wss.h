#ifndef WSS_H
#define WSS_H
#include <iostream>
#include <thread>
#include <pthread.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <set>
#include <json/json.h>
#include <semaphore.h>

#define patternSHMSize 8192

using namespace std;
typedef websocketpp::server<websocketpp::config::asio> WebsocketServer;
typedef WebsocketServer::message_ptr message_ptr;
using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
class WSS
{
public:
    WSS(const char* WSPort="60001",const char* user="hirg");
    virtual ~WSS();
private:
    WebsocketServer WSServer;
    void onWSOpen(websocketpp::connection_hdl hdl);
    void onWSClose(websocketpp::connection_hdl hdl);
    void onWSMessage(websocketpp::connection_hdl hdl, message_ptr msg);
    int mWSPort;
    int mDAQ2DPCPort;
    string mDPCIP;
    string user;
    Json::CharReaderBuilder RBuilder;
    Json::CharReader * reader;
    Json::StreamWriterBuilder WBuilder;
    Json::StreamWriter * writer;
    Json::Value root;
    string errors;

    void initSHM();
    void initDAQSHM();
    void initDPCSHM();
    void initMonSHM();
    void initPatternSHM();
    void closeSHM();
    void closeDAQSHM();
    void closeDPCSHM();
    void closeMonSHM();
    void closePatternSHM();

    void setDAQAction(int action);
    void setDAQStatus(int status);
    void setDAQBufferStatus(int status);//bufferStatus
    int getDAQStatus();
    int getDAQBufferStatus();//bufferStatus
    bool checkDAQStatus();
    void setDPCAction(int action);
    void setDPCStatus(int status);
    void setDPCBufferStatus(int status);
    int getDPCStatus();
    int getDPCBufferStatus();//bufferStatus
    bool checkDPCStatus();
    void setMonAction(int action);
    void setMonStatus(int status);
    int getMonStatus();
    bool checkMonStatus();
    
    void turnOnDAQ();
    void turnOffDAQ();
    void turnOnDPC();
    void turnOffDPC();
    void turnOnMon();
    void turnOffMon();

    void WSResponseDAQStatus(websocketpp::connection_hdl hdl);
    void WSResponseDAQBufferStatus(websocketpp::connection_hdl hdl);//bufferStatus
    void WSResponseDPCStatus(websocketpp::connection_hdl hdl);
    void WSResponseDPCBufferStatus(websocketpp::connection_hdl hdl);//bufferStatus
    void WSResponseMonStatus(websocketpp::connection_hdl hdl);

    FILE* DAQProc;
    FILE* DPCProc;
    FILE* MonProc;
  
    int mDAQAction;
    int mDPCAction;
    int mMonAction;

    int* DAQAction;
    int* DAQStatus;
    int* DAQBufferStatus;//bufferStatus
    int* DPCAction;
    int* DPCStatus;
    int* DPCBufferStatus;//bufferStatus
    int* MonAction;
    int* MonStatus;
    char* pattern;

    int DAQActionSHMID;
    int DAQStatusSHMID;
    int DAQBufferStatusSHMID;//bufferStatus
    int DPCActionSHMID;
    int DPCStatusSHMID;
    int DPCBufferStatusSHMID;//bufferStatus
    int MonActionSHMID;
    int MonStatusSHMID;
    int patternSHMID;

    string DAQActionSHMName="DAQActionSHM";
    string DAQStatusSHMName="DAQStatusSHM";
    string DAQBufferStatusSHMName="DAQBufferStatusSHM";//bufferStatus
    string DPCActionSHMName="DPCActionSHM";
    string DPCStatusSHMName="DPCStatusSHM";
    string DPCBufferStatusSHMName="DPCBufferStatusSHM";//bufferStatus
    string MonActionSHMName="MonActionSHM";
    string MonStatusSHMName="MonStatusSHM";
    string patternSHMName="patternSHM";

    string DAQActionSEMName="DAQActionSEM";
    string DAQStatusSEMName="DAQStatusSEM";
    string DAQBufferStatusSEMName="DAQBufferStatusSEM";//bufferStatus
    string DPCActionSEMName="DPCActionSEM";
    string DPCStatusSEMName="DPCStatusSEM";
    string DPCBufferStatusSEMName="DPCBufferStatusSEM";//bufferStatus
    string MonActionSEMName="MonActionSEM";
    string MonStatusSEMName="MonStatusSEM";
    string patternSEMName="patternSEM";

    sem_t* DAQActionSEM=NULL;
    sem_t* DAQStatusSEM=NULL;
    sem_t* DAQBufferStatusSEM=NULL;//bufferStatus
    sem_t* DPCActionSEM=NULL;
    sem_t* DPCStatusSEM=NULL;
    sem_t* DPCBufferStatusSEM=NULL;//bufferStatus
    sem_t* MonActionSEM=NULL;
    sem_t* MonStatusSEM=NULL;
    sem_t* patternSEM=NULL;


  void writePattern2SHM(const char* str,int length);
  void clearSHMCaches();
};

#endif // WSS_H
