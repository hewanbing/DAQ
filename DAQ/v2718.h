#ifndef __v2718_h__
#define __v2718_h__
#include "DAQHandle.h"
#include "BufferTP.h"
#include "CAENVMElib.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <TRandom.h>
#include "MString.h"
#include "MData.h"
#include <json/json.h>
#include "FEE.h"

class v2718:public DAQHandle {
 public:
  v2718();
  v2718(Json::Value json);
  virtual ~v2718();
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
  bool deviceOpen();
  bool deviceClose();
  bool systemReset();
  vector<FEE*>fee;
  CVBoardTypes  handleType;
  int32_t       BHandle;

};
#endif
