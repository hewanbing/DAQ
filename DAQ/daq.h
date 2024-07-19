#ifndef DAQ_H
#define DAQ_H
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
#include <sys/time.h>
#include <set>
#include <json/json.h>
#include <semaphore.h>
#include <iostream>
#include "DAQHandle.h"

#define patternSHMSize 8192
using namespace std;

class DAQ
{
public:
    DAQ();

private:

    int mStatus;
    int mAction;
    int* action;
    int* status;
    int* bufferStatus;//bufferStatus
    char* pattern;

    DAQHandle* handle;
    int actionSHMID;
    int statusSHMID;
    int bufferStatusSHMID;//bufferStatus
    int patternSHMID;

    string actionSHMName="DAQActionSHM";
    string statusSHMName="DAQStatusSHM";
    string bufferStatusSHMName="DAQBufferStatusSHM";//bufferStatus
    string patternSHMName="patternSHM";

    string actionSEMName="DAQActionSEM";
    string statusSEMName="DAQStatusSEM";
    string bufferStatusSEMName="DAQBufferStatusSEM";//bufferStatus
    string patternSEMName="patternSEM";

    sem_t* actionSEM=NULL;
    sem_t* statusSEM=NULL;
    sem_t* bufferStatusSEM=NULL;//bufferStatus
    sem_t* patternSEM=NULL;

    void initSHM();
    void closeSHM();

    void setAction(int action);
    void setStatus(int status);
    void setBufferStatus(int status);//bufferStatus
    int getAction();
    int getStatus();
    bool checkStatus();

    void turnOn();
    void init();
    void start();
    void stop();
    void pause();
    void resume();
    void turnOff();
    void step();

    Json::CharReaderBuilder RBuilder;
    Json::CharReader * reader;
    Json::Value patternJson;
    void parsePattern();

    thread *mUDPThread=NULL;
    void UDPClient();
    void startDataSender();
    void stopDataSender();
    void setupHandle();
};

#endif // DAQ_H
