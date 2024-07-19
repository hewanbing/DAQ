#ifndef __monitor_h__
#define __monitor_h__
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

using namespace std;
#define patternSHMSize 8192

class monitor {
 public:
  monitor();
  virtual ~monitor();

 private:
  int mStatus;
  int mAction;
  int* action;
  int* status;

  int actionSHMID;
  int statusSHMID;

  string actionSHMName="MonActionSHM";
  string statusSHMName="MonStatusSHM";

  string actionSEMName="MonActionSEM";
  string statusSEMName="MonStatusSEM";

  sem_t* actionSEM=NULL;
  sem_t* statusSEM=NULL;

  sem_t* patternSEM=NULL;
  char* pattern;
  int patternSHMID;
  string patternSHMName="patternSHM";
  string patternSEMName="patternSEM";

  void initSHM();
  void closeSHM();

  void setAction(int action);
  void setStatus(int status);
  int getAction();
  int getStatus();
  bool checkStatus();

  Json::CharReaderBuilder RBuilder;
  Json::CharReader * reader;
  Json::Value patternJson;
  void parsePattern();

  THttpServer* JSROOTServer;

  void turnOn();
  void turnOff();
  void cleanData();
  void step();
  const char* jobname="Plots";
  TH1F *ADC[32];
  TH2D *ADC2D;
  THttpServer* serv;
  Event *buffer;
  int eventBufferLength;
  int eventBufferCharSize;
  int sockfd;
  struct sockaddr_in servaddr;

  void setupUDPServer();
  void closeUDPServer();
  void setupROOTObjects();
  void setupJSROOT();
  void setupBuffer();
};
#endif
