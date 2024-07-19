#ifndef __v785_h__
#define __v785_h__
#include "FEEIRQ.h"
#include <vector>
#include "FEE.h"
#include <string>
#include <sstream>
#include "stdlib.h"
#include "MString.h"
#include "BufferTP.h"
#include "Channel.h"
#include <json/json.h>
#include "DAQHandle.h"

using namespace std;
class v785 : public FEE{
public:
  v785(DAQHandle *handle,Json::Value json);
  virtual ~v785();
  void decode(void* data, int dataLength, BufferTP<Channel>* buffer) override;
  bool init() override;
private:
//  int channel[32];
  bool systemReset();
  bool setIRQLevel(int n);
  bool setIRQID(int id);
  bool isDataReady();
  bool isEventTriggered();
  bool setEventTrigger(int n);
  bool isBufferEmpty();
  bool isBufferFull();
  long getEventCount();
  bool setCrateNumber(int n);
  bool enableOverflowSuppression();
  bool disableOverflowSuppression();
  bool enableZeroSuppression();
  bool disableZeroSuppression();
  bool setZeroSuppressionResolution2();
  bool setZeroSuppressionResolution16();
  bool enableChannel(int n);
  bool disableChannel(int n);
  bool enableChannelAll();
  bool disableChannelAll();
  bool setThreshold(int ch,int threshold);
  bool setThresholdAll(int threshold);
};
#endif
