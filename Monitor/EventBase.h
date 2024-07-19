#ifndef __EventBase_h__
#define __EventBase_h__
#include <string>
#include <stdlib.h>

class EventBase {
 public:
  EventBase();
  virtual ~EventBase();
  virtual void setData(void* ptr);
  virtual void getData(void* ptr);
  virtual void setDataSize(int dataSize);
  virtual int getDataSize();
  virtual void setTimeStampe(long time);
  virtual long getTimeStampe();
  virtual void setOrder(int order);
  virtual int getOrder();
  virtual const char* getName();
protected:
  char* data;
  int dataSize;
  std::string name;
  long timeStampe;
  int order;
};
#endif
