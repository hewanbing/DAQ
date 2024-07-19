#ifndef DPC_H
#define DPC_H
#include <iostream>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <websocketpp/config/asio_no_tls.hpp>
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
#include "Event.h"
#include <THttpServer.h>
#include <json/json.h>
#include <semaphore.h>
#include <TFile.h>
#include "BufferTP.h"
#include <TTree.h>
#include "Channel.h"

#define patternSHMSize 8192
using namespace std;
class DPC
{
public:
    DPC();
private:
    int mStatus;
    int mAction;
    int* action;
    int* status;
    int* bufferStatus;//bufferStatus

    int actionSHMID;
    int statusSHMID;
    int bufferStatusSHMID;//bufferStatus

    string actionSHMName="DPCActionSHM";
    string statusSHMName="DPCStatusSHM";
    string bufferStatusSHMName="DPCBufferStatusSHM";//bufferStatus

    string actionSEMName="DPCActionSEM";
    string statusSEMName="DPCStatusSEM";
    string bufferStatusSEMName="DAQBufferStatusSEM";//bufferStatus

    sem_t* actionSEM=NULL;
    sem_t* statusSEM=NULL;
    sem_t* bufferStatusSEM=NULL;//bufferStatus

    sem_t* patternSEM=NULL;
    char* pattern;
    int patternSHMID;
    string patternSHMName="patternSHM";
    string patternSEMName="patternSEM";

    void initSHM();
    void closeSHM();

    void setAction(int action);
    void setStatus(int status);
    void setBufferStatus(int status);
    int getAction();
    int getStatus();
    bool checkStatus();

    Json::CharReaderBuilder RBuilder;
    Json::CharReader * reader;
    Json::Value patternJson;
    void parsePattern();

    void createTree();
    void saveTree();

    void turnOn();
    void turnOff();
    void start();
    void stop();
    void UDPClient();
    void step();
    thread *mUDPThread=NULL;

    BufferTP<Event>*eventBuffer;
    Event *evt;
    TFile* mRootFile;
    string mRootFileNameBase;
    TString mRootFileName;
    int nEventsPerFile;
    int nEvents=0;
    TTree* mTree;

    int portFlag;
    Channel *DAQDataBuffer;
    int DAQDataBufferLength;
    int sockfd;
    struct sockaddr_in servaddr;
};

#endif // DPC_H
