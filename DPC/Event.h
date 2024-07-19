#ifndef __Event_h__
#define __Event_h__
#include "TGClient.h"
#include "EventBase.h"
#include "Channel.h"
class Event:public EventBase {
 public:
  Event();
  virtual ~Event();
  bool fillChannel(Channel ch);
  Channel getADCChannel(int i);
  void reset();
 private:
  int eventID;
  Channel ADC[32];
  int channelFilled;
  ClassDef(Event,1)
};
#endif
