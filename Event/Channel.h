#ifndef __Channel_h__
#define __Channel_h__
#include "TGClient.h"
class Channel{
 public:
  Channel(){};
  virtual ~Channel(){};
  char type[3];
  int crateID;
  int moduleID;
  int channelID;
  long eventID;
  long time;
  long value;
  ClassDef(Channel,1)
};
#endif
