#include "EventBase.h"
#include "iostream"
#include "stdio.h"
#include <unistd.h>
#include <string.h>
using namespace std;

EventBase::EventBase(){
  data=NULL;
}
EventBase::~EventBase(){
  if(data!=NULL)delete data;
}
void EventBase::setData(void *ptr){
  memcpy(data,ptr,dataSize);
}
void EventBase::getData(void *ptr){
  memcpy(ptr,data,dataSize);
}
void EventBase::setDataSize(int size){
  dataSize=size;
  if(data!=NULL)delete data;
  data=new char[size];
}
int EventBase::getDataSize(){
  return dataSize;
}
void EventBase::setTimeStampe(long time){
  timeStampe=time;
}
long EventBase::getTimeStampe(){
  return timeStampe;
}
void EventBase::setOrder(int _order){
  order=_order;
}
int EventBase::getOrder(){
  return order;
}
const char* EventBase::getName(){
  return name.data();
}
