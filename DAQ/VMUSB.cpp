#include "VMUSB.h"
#include "iostream"
#include "stdio.h"
#include <unistd.h>
#include "MString.h"
#include <bitset>
#include "MData.h"
#include "v1290.h"
using namespace std;

VMUSB::VMUSB(Json::Value json){
    stackAddress[0]=0x02;
    stackAddress[1]=0x03;
    stackAddress[2]=0x12;
    stackAddress[3]=0x13;
    stackAddress[4]=0x22;
    stackAddress[5]=0x23;
    stackAddress[6]=0x32;
    stackAddress[7]=0x33;
    setPatternJson(json);
    setBuffer(json["DAQBufferLength"].asInt(),json["DAQBufferNumber"].asInt());
}
VMUSB::~VMUSB(){}
bool VMUSB::deviceOpen(){
  if(xxusb_devices_find(devices)<0) return false;
  dev = devices[0].usbdev;
  udev = xxusb_device_open(dev);
  if(!udev) return false;
  return true;
}
bool VMUSB::deviceClose(){
  if(xxusb_device_close(udev)<0) return false;
  return true;
}
bool VMUSB::setStack(int stackID,long* stackData){
  int stackLength=stackData[0]+1;
  long stackRead[stackLength];
  if(!xxusb_stack_write(udev, stackAddress[stackID], stackData))return false;
  if(!xxusb_stack_read(udev, stackAddress[stackID], stackRead))return false;
  for(int i=0;i<stackLength;i++) {
    if(stackRead[i]!=stackData[i])return false;
  }
  return true;
}
bool VMUSB::setIRQVector(int IRQVectorID, int stackID, int IRQLevel, int IRQID){
    if(IRQVectorID==0){
        if(!setRegisterBit(0x28,stackID,12,13))return false;
        if(!setRegisterBit(0x28,IRQLevel,8,10))return false;
        if(!setRegisterBit(0x28,MData::parse(IRQID).get(0,7),0,7))return false;
        if(!setRegisterBit(0x40,MData::parse(IRQID).get(8,15),0,7))return false;
    }
    if(IRQVectorID==1){
        if(!setRegisterBit(0x28,stackID,28,30))return false;
        if(!setRegisterBit(0x28,IRQLevel,24,26))return false;
        if(!setRegisterBit(0x28,MData::parse(IRQID).get(0,7),16,23))return false;
        if(!setRegisterBit(0x40,MData::parse(IRQID).get(8,15),8,15))return false;
    }
    if(IRQVectorID==2){
        if(!setRegisterBit(0x2C,stackID,12,13))return false;
        if(!setRegisterBit(0x2C,IRQLevel,8,10))return false;
        if(!setRegisterBit(0x2C,MData::parse(IRQID).get(0,7),0,7))return false;
        if(!setRegisterBit(0x40,MData::parse(IRQID).get(8,15),16,23))return false;
    }
    if(IRQVectorID==3){
        if(!setRegisterBit(0x2C,stackID,28,30))return false;
        if(!setRegisterBit(0x2C,IRQLevel,24,26))return false;
        if(!setRegisterBit(0x2C,MData::parse(IRQID).get(0,7),16,23))return false;
        if(!setRegisterBit(0x40,MData::parse(IRQID).get(8,15),24,31))return false;
    }
    if(IRQVectorID==4){
        if(!setRegisterBit(0x30,stackID,12,13))return false;
        if(!setRegisterBit(0x30,IRQLevel,8,10))return false;
        if(!setRegisterBit(0x30,MData::parse(IRQID).get(0,7),0,7))return false;
        if(!setRegisterBit(0x44,MData::parse(IRQID).get(8,15),0,7))return false;
    }
    if(IRQVectorID==5){
        if(!setRegisterBit(0x30,stackID,28,30))return false;
        if(!setRegisterBit(0x30,IRQLevel,24,26))return false;
        if(!setRegisterBit(0x30,MData::parse(IRQID).get(0,7),16,23))return false;
        if(!setRegisterBit(0x44,MData::parse(IRQID).get(8,15),8,15))return false;
    }
    if(IRQVectorID==6){
        if(!setRegisterBit(0x34,stackID,12,13))return false;
        if(!setRegisterBit(0x34,IRQLevel,8,10))return false;
        if(!setRegisterBit(0x34,MData::parse(IRQID).get(0,7),0,7))return false;
        if(!setRegisterBit(0x44,MData::parse(IRQID).get(8,15),16,23))return false;
    }
    if(IRQVectorID==7){
        if(!setRegisterBit(0x34,stackID,28,30))return false;
        if(!setRegisterBit(0x34,IRQLevel,24,26))return false;
        if(!setRegisterBit(0x34,MData::parse(IRQID).get(0,7),16,23))return false;
        if(!setRegisterBit(0x44,MData::parse(IRQID).get(8,15),24,31))return false;
    }
  return true;
}
bool VMUSB::systemReset(){
    int ret = xxusb_register_write(udev,1,0b1000);
    if(ret<0)return false;
    sleep(1);
    ret = xxusb_register_write(udev,1,0);
    if(ret<0)return false;
    return true;
}
bool VMUSB::cleanRegisters(){
    int ret = xxusb_register_write(udev,1,0b100);
    if(ret<0)return false;
    sleep(1);
    ret = xxusb_register_write(udev,1,0);
    if(ret<0)return false;
    return true;
}
int VMUSB::getFirmware(){
    long firmware;
    VME_register_read(udev,0,&firmware);
    return firmware;
}
bool VMUSB::init(){
  if(!udev){
    if(!deviceOpen())return false;
  }
  if(!deviceStop())return false;
  if(!systemReset())return false;
  if(!cleanRegisters())return false;
  if(!deviceSetBufferSize(8))return false;
  if(!deviceSetAlign32(1))return false;
  if(!deviceSetHeaderOpt(1))return false;
  //deviceSetEventsPerBuffer(1);
  //________________________________
  int nModule=getPatternJson()["Modules"].size();
  for(int i=0;i<nModule;i++){
    Json::Value moduleJson=getPatternJson()["Modules"][i];
    if(strcmp(moduleJson["type"].asString().c_str(),"V785")==0){
      FEE* module=new v785(this,moduleJson);
      fee.push_back(module);
    }
    if(strcmp(moduleJson["type"].asString().c_str(),"v1290")==0){
      FEE* module=new v1290(this,moduleJson);
      fee.push_back(module);
    }
  }
  //_____________________________________
  if(!cleanIRQVectors())return false;
  int IRQVectorID=0;
  for(unsigned long i=0;i<fee.size();i++){
    for(int j=0;j<fee[i]->getIRQSize();j++){
        if(IRQVectorID>7)return false;
        int stackID=(IRQVectorID+2)%8;
        stackFEELabel[stackID]=i;
        int stackLength=6;
        long stack[stackLength];
        FEEIRQ irq=fee[i]->getIRQ(j);
        stack[0]=stackLength-1;
        stack[1]=0x100*IRQVectorID;
        stack[2]=0x100+irq.AM;
        stack[3]=irq.dataSize*0x100;//last bit 1 for big endian
        stack[4]=irq.dataAddress;
        stack[5]=fee[i]->getBaseAddress();

        if(!setStack(stackID,stack))return false;
        if(!setIRQVector(IRQVectorID,stackID,irq.level,irq.ID))return false;
        IRQVectorID++;
    }
  }
  return true;
}
bool VMUSB::start(){
  return deviceStart();
}
bool VMUSB::step(){
  int ret=0;
  int rawData[1000];
  for(int i=0;i<1000;i++){
    rawData[i]=0;
  }
  ret=xxusb_bulk_read(udev,rawData,4000,30);
  if(ret<=0){
      return false;
  }else{
      int nEvent=MData::parse(rawData[0]).get(0,12);
      int* decodePtr=rawData+3;
      for(int i=0;i<nEvent;i++){
          int lEvent=ceil(MData::parse(rawData[2]).get(0,11)/2);
          int stackID=MData::parse(rawData[2]).get(13,15);
          int feeID=(stackID+6)%8;
          fee[feeID]->decode(decodePtr,lEvent,getBufferPtr());
          decodePtr+=lEvent;
      }
  }
  return true;
}
bool VMUSB::pause(){
    if(xxusb_register_write(udev,1,0x0)<0)return false;
    return true;
}
bool VMUSB::resume(){
    if(xxusb_register_write(udev,1,0x1)<0)return false;
    return true;
}
bool VMUSB::stop(){
 return deviceStop();
}
bool VMUSB::setBit(short AM, long address, long _data, int bitLow, int bitHigh,int dataWidth){
  long tmp;
  int ret;
  if(dataWidth==16){
    ret=VME_read_16(udev,AM,address,&tmp);
    ret=VME_read_16(udev,AM,address,&tmp);
    if(ret<0)return false;
    long data=_data;
    data=data<<bitLow;
    MData mData(tmp);
    tmp=mData.set0(bitLow,bitHigh);
    data+=tmp;
    ret = VME_write_16(udev,AM,address,data);
    if(ret<0)return false;
  }else{
    ret=VME_read_32(udev,AM,address,&tmp);
    ret=VME_read_32(udev,AM,address,&tmp);
    if(ret<0)return false;
    long data=_data;
    data=data<<bitLow;
    MData mData(tmp);
    tmp=mData.set0(bitLow,bitHigh);
    data+=tmp;
    ret = VME_write_32(udev,AM,address,data);
    if(ret<0)return false;
  }
  return true;
}
long VMUSB::getBit(short AM, long address, int bitLow,int bitHigh, int dataWidth){
  return MData::parse(read(AM,address,dataWidth)).get(bitLow,bitHigh);
}
bool VMUSB::write(short AM, long address, long data, int dataWidth){
    int ret;
    if(dataWidth==16){
      ret = VME_write_16(udev,AM,address,data);
      if(ret<0)return false;
    }else{
      ret = VME_write_32(udev,AM,address,data);
      if(ret<0)return false;
    }
    return  true;
}
long VMUSB::read(short AM, long address, int dataWidth){
    long tmp;
    int ret;
    if(dataWidth==16){
        ret=VME_read_16(udev,AM,address,&tmp);
        ret=VME_read_16(udev,AM,address,&tmp);
        if(ret<0)exit(0);
    }else{
        ret=VME_read_32(udev,AM,address,&tmp);
        ret=VME_read_32(udev,AM,address,&tmp);
        if(ret<0)exit(0);
    }
    return tmp;
}
bool VMUSB::setRegisterBit(long address, long _data, int bitLow, int bitHigh){
  long tmp;
  int ret;
  ret=VME_register_read(udev, address, &tmp);
  if(ret<0)return false;
  long data=_data;
  data=data<<bitLow;
  tmp=MData::parse(tmp).set0(bitLow,bitHigh);
  data+=tmp;
  ret = VME_register_write(udev,address,data);
  if(ret<0)return false;
  return true;
}
long VMUSB::getRegisterBit(long address, int bitLow, int bitHigh){
    return MData::parse(readRegister(address)).get(bitLow,bitHigh);
}
bool VMUSB::writeRegister(long address, long data){
    int ret=VME_register_write(udev,address,data);
    if(ret<0) return false;
    return true;
}
long VMUSB::readRegister(long address){
    long tmp;
    int ret=VME_register_read(udev,address,&tmp);
    if(ret<0)exit(0);
    return tmp;
}
bool VMUSB::deviceStart(){
    if(xxusb_register_write(udev,1,0x1)<0)return false;
    return true;
}
bool VMUSB::deviceStop(){
    if(xxusb_register_write(udev,1,0x0)<0)return false;
    int tmpArray[8192];
    int ret = xxusb_usbfifo_read(udev, tmpArray, 8192, 30);
    while(ret >0) {
       ret = xxusb_usbfifo_read(udev, tmpArray, 8192, 30);
    }
    return true;
}
bool VMUSB::deviceSetBufferSize(int n){
    return setRegisterBit(4,n,0,4);
}
bool VMUSB::deviceSetAlign32(int n){
    return setRegisterBit(4,n,7,7);
}
bool VMUSB::deviceSetHeaderOpt(int n){
    return setRegisterBit(4,n,8,8);
}
bool VMUSB::deviceSetEventsPerBuffer(int n){
    if(getRegisterBit(4,0,4)!=9)return false;
    return writeRegister(0x24,n);
}
bool VMUSB::deviceUSBTrigger(){
    if(xxusb_register_write(udev,1,0b10)<0)return false;
    return true;
}
bool VMUSB::deviceClearRegisters(){
    if(xxusb_register_write(udev,1,0b100)<0)return false;
    return true;
}
bool VMUSB::deviceSystemReset(){
    if(xxusb_register_write(udev,1,0b1000)<0)return false;
    sleep(1);
    if(xxusb_register_write(udev,1,0)<0)return false;
    return true;
}
bool VMUSB::deviceScalerDump(){
    if(xxusb_register_write(udev,1,0b10000)<0)return false;
    return true;
}
bool VMUSB::deviceSoftIRQ(int code){
    int IRQ=code<<8;
    IRQ++;
    if(xxusb_register_write(udev,1,IRQ)<0)return false;
    return true;
}
bool VMUSB::cleanIRQVectors(){
    if(!writeRegister(0x28,0))return false;
    if(!writeRegister(0x2C,0))return false;
    if(!writeRegister(0x30,0))return false;
    if(!writeRegister(0x34,0))return false;
    if(!writeRegister(0x40,0))return false;
    if(!writeRegister(0x44,0))return false;
    return true;
}
