#ifndef __VMUSB_h__
#define __VMUSB_h__
#include "DAQHandle.h"
#include "BufferTP.h"
#include "libxxusb.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <TRandom.h>
#include "FEE.h"
#include <vector>
#include "v785.h"
#include "MData.h"
#include "Channel.h"
#include <json/json.h>
class VMUSB : public DAQHandle{
 public:
  //VMUSB();
  VMUSB(Json::Value json);
  virtual ~VMUSB();
  bool init();
  bool start();
  bool step();
  bool pause();
  bool resume();
  bool stop();
  bool setBit(short AM, long address, long data, int bitLow,int bitHigh, int dataWidth=16);
  long getBit(short AM, long address, int bitLow,int bitHigh, int dataWidth=16);
  bool write(short AM, long address, long data, int dataWidth=16);
  long read(short AM, long address, int dataWidth=16);

 private:
  bool setRegisterBit(long address, long data, int bitLow, int bitHigh);
  long getRegisterBit(long address, int bitLow,int bitHigh);
  bool writeRegister(long address, long data);
  long readRegister(long address);
  bool setStack(int stackID,long* stackData);
  bool setIRQVector(int IRQVectorID, int stackID, int IRQLevel, int IRQID);
  bool deviceOpen();
  bool deviceClose();
  bool deviceStart();
  bool deviceStop();
  bool deviceUSBTrigger();
  bool deviceClearRegisters();
  bool deviceSystemReset();
  bool deviceScalerDump();
  bool deviceSoftIRQ(int code);

  bool deviceSetBufferSize(int n);
  bool deviceSetAlign32(int n);
  bool deviceSetHeaderOpt(int n);
  bool deviceSetEventsPerBuffer(int n);
  bool cleanIRQVectors();
  bool systemReset();
  bool cleanRegisters();
  int getFirmware();
  xxusb_device_type devices[100];
  struct usb_device *dev=NULL;
  usb_dev_handle *udev=NULL;

  int stackAddress[8];
  int stackFEELabel[8];
  vector<FEE*>fee;
};
#endif
