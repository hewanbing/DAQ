#include "v2718.h"
#include "iostream"
#include "stdio.h"
#include <unistd.h>
#include "v785.h"
#include "v1290.h"

using namespace std;
v2718::v2718(){}
v2718::v2718(Json::Value json){
    setPatternJson(json);
    setBuffer(json["DAQBufferLength"].asInt(),json["DAQBufferNumber"].asInt());
}
v2718::~v2718(){}
bool v2718::deviceOpen(){
  if(CAENVME_Init(cvV2718,0,0,&BHandle)!=cvSuccess){
    BHandle=-1;
    return false;
  }
  return true;
}
bool v2718::init(){
    if(!deviceOpen())return false;
    if(!systemReset())return false;
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
  return true;
}
bool v2718::start(){
  return true;
}
bool v2718::step(){
    CVErrorCodes ret;
    for(unsigned long i=0;i<fee.size();i++){
      for(int j=0;j<fee[i]->getIRQSize();j++){
          FEEIRQ irq=fee[i]->getIRQ(j);
          ret=CAENVME_IRQEnable(BHandle,irq.ID);
          if(ret!=cvSuccess)return false;
          ret=CAENVME_IRQWait(BHandle,irq.ID,100);
          if(ret!=cvSuccess)return false;
          uint32_t address=fee[i]->address(irq.dataAddress);
          uint32_t buf[irq.dataSize];
          int dataSize=irq.dataSize;
          CVAddressModifier AM=(CVAddressModifier)irq.AM;
          for(int i=0;i<dataSize;i++){
              buf[i]=read(9,address,32);
          }
//          int count;
//          ret=CAENVME_BLTReadCycle(BHandle,address,buf,dataSize,AM,cvD64,&count);
//          if(ret!=cvSuccess)return false;
          fee[i]->decode(buf,dataSize,getBufferPtr());
      }
    }
  return true;
}
bool v2718::pause(){
    return true;
}
bool v2718::resume(){
    return true;
}
bool v2718::stop(){
    return true;
}
bool v2718::setBit(short AM, long address, long _data, int bitLow, int bitHigh, int dataWidth){
    long tmp;
    CVErrorCodes ret;
    if(dataWidth==16){
        ret=CAENVME_ReadCycle(BHandle,address,&tmp,(CVAddressModifier)AM,cvD16);
        if(ret!=cvSuccess)return false;
        long data=_data;
        data=data<<bitLow;
        MData mData(tmp);
        tmp=mData.set0(bitLow,bitHigh);
        data+=tmp;
        ret=CAENVME_WriteCycle(BHandle,address,&data,(CVAddressModifier)AM,cvD16);
        if(ret!=cvSuccess)return false;
    }else if(dataWidth==32){
        ret=CAENVME_ReadCycle(BHandle,address,&tmp,(CVAddressModifier)AM,cvD32);
        if(ret!=cvSuccess)return false;
        long data=_data;
        data=data<<bitLow;
        MData mData(tmp);
        tmp=mData.set0(bitLow,bitHigh);
        data+=tmp;
        ret=CAENVME_WriteCycle(BHandle,address,&data,(CVAddressModifier)AM,cvD32);
        if(ret!=cvSuccess)return false;
    }else{return false;}
    return true;
}
long v2718::getBit(short AM, long address, int bitLow, int bitHigh, int dataWidth){
    return MData::parse(read(AM,address,dataWidth)).get(bitLow,bitHigh);
}
bool v2718::write(short AM, long address, long _data, int dataWidth){
    long data=_data;
    if(dataWidth==16){
        if(CAENVME_WriteCycle(BHandle,address,&data,(CVAddressModifier)AM,cvD16)!=cvSuccess)return false;
    }else if(dataWidth==32){
        if(CAENVME_WriteCycle(BHandle,address,&data,(CVAddressModifier)AM,cvD32)!=cvSuccess)return false;
    }else {return false;}
    return true;
}
long v2718::read(short AM, long address, int dataWidth){
    long data;
    if(dataWidth==16){
        CVErrorCodes code=CAENVME_ReadCycle(BHandle,address,&data,(CVAddressModifier)AM,cvD16);
        if(code!=cvSuccess)exit(0);
    }else if(dataWidth==32){
        CVErrorCodes code=CAENVME_ReadCycle(BHandle,address,&data,(CVAddressModifier)AM,cvD32);
        if(code!=cvSuccess)exit(0);
    }else{exit(0);}
    return data;
}
bool v2718::systemReset(){
    CVErrorCodes code=CAENVME_SystemReset(BHandle);
    if(code!=cvSuccess)return false;
    return true;
}
