#include "v785.h"
#include "iostream"
#include "stdio.h"
#include <unistd.h>
#include "MData.h"
using namespace std;

v785::v785(DAQHandle* _handle,Json::Value _json){
    setHandle(_handle);
    setJson(_json);
    setBaseAddress(MString::parse(json["base"].asString().c_str()).getLong());
    setCrateID(MString::parse(json["crateID"].asString().c_str()).getInt());
    setModuleID(MString::parse(json["moduleID"].asString().c_str()).getInt());
    setEventTrigger(1);
    //enableChannelAll();
    setCrateNumber(getCrateID());
    FEEIRQ irq;
    irq.level=1;
    irq.ID=getModuleID();
    irq.AM=0xB;
    irq.dataSize=32+2;
    irq.dataAddress=0;
    setIRQLevel(irq.level);
    setIRQID(irq.ID);
    setIRQ(irq);
    //init();
}
v785::~v785(){
  IRQ.clear();
}
bool v785::init(){
    return systemReset();
}
bool v785::systemReset(){
    if(!handle->setBit(9,address(0x1006),1,7,7))return false;
    usleep(100000);
    if(!handle->setBit(9,address(0x1008),1,7,7))return false;
    if(handle->getBit(9,address(0x1006),7,7)==1)return false;
    return true;
}
void v785::decode(void* _data, int dataLength, BufferTP<Channel>* buffer){
  Channel ch;
  int *data=(int*)_data;
  MData eventCounter(data[0x21]);
  ch.type[0]='A';
  ch.type[1]='D';
  ch.type[2]='C';
  ch.crateID=getCrateID();
  ch.moduleID=getModuleID();

  ch.eventID=eventCounter.get(0,23);
  for(int i=1;i<dataLength-1;i++){
    MData tmp(data[i]);
    ch.channelID=tmp.get(16,20);
    ch.value=tmp.get(0,11);
    memcpy(buffer->getBuffer4Fill(1),&ch,sizeof(ch));
    buffer->fillDone();
  }
}
bool v785::setIRQLevel(int n){
    return handle->setBit(9,address(0x100A),n,0,2);
}
bool v785::setIRQID(int id){
    return handle->setBit(9,address(0x100C),id,0,7);
}
bool v785::isDataReady(){
    return handle->getBit(9,address(0x100E),0,0);
}
bool v785::isEventTriggered(){
    return handle->getBit(9,address(0x100E),8,8);
}
bool v785::setEventTrigger(int n){
    return handle->setBit(9,address(0x1020),n,0,4);
}
bool v785::isBufferEmpty(){
    return handle->getBit(9,address(0x1022),1,1);
}
bool v785::isBufferFull(){
    return handle->getBit(9,address(0x1022),2,2);
}
long v785::getEventCount(){
    long low=handle->read(9,address(0x1024));
    long high=handle->read(9,address(0x1026));
    return high*0x10000+low;
}
bool v785::setCrateNumber(int n){
    return handle->setBit(9,address(0x103C),n,0,7);
}
bool v785::enableOverflowSuppression(){
    return handle->setBit(9,address(0x1032),1,3,3);
}
bool v785::disableOverflowSuppression(){
    return handle->setBit(9,address(0x1032),0,3,3);
}
bool v785::enableZeroSuppression(){
    return handle->setBit(9,address(0x1032),1,4,4);
}
bool v785::disableZeroSuppression(){
    return handle->setBit(9,address(0x1032),0,4,4);
}
bool v785::setZeroSuppressionResolution2(){
    return handle->setBit(9,address(0x1032),1,8,8);
}
bool v785::setZeroSuppressionResolution16(){
    return handle->setBit(9,address(0x1032),0,8,8);
}
bool v785::enableChannel(int n){
    return handle->setBit(9,address(0x1080+2*n),0,8,8);
}
bool v785::disableChannel(int n){
    return handle->setBit(9,address(0x1080+2*n),1,8,8);
}
bool v785::enableChannelAll(){
    for(int i=0;i<32;i++){
        if(!enableChannel(i)) return false;
    }
    return true;
}
bool v785::disableChannelAll(){
    for(int i=0;i<32;i++){
        if(!disableChannel(i)) return false;
    }
    return true;
}
bool v785::setThreshold(int ch, int threshold){
    return handle->setBit(9,address(0x1080+2*ch),threshold,0,7);
}
bool v785::setThresholdAll(int threshold){
    for(int i=0;i<32;i++){
        if(!setThreshold(i,threshold)) return false;
    }
    return true;
}
