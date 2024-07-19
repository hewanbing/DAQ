#ifndef __FEE_h__
#define __FEE_h__
#include "FEEIRQ.h"
#include <vector>
#include "Channel.h"
#include "BufferTP.h"
#include "DAQHandle.h"
#include <json/json.h>

using namespace std;
class FEE{
 public:
    FEE(){}
    virtual ~FEE(){}
    virtual void decode(void* data, int dataLength, BufferTP<Channel>* buffer){}
    virtual bool init(){return true;}
    int getIRQSize(){return IRQ.size();}
    FEEIRQ getIRQ(int i){return IRQ[i];}
    void setIRQ(FEEIRQ irq){IRQ.push_back(irq);}
    void setBaseAddress(long address){baseAddress=address;}
    long getBaseAddress(){return baseAddress;}
    void setCrateID(int id){crateID=id;}
    int getCrateID(){return crateID;}
    void setModuleID(int id){moduleID=id;}
    int getModuleID(){return moduleID;}
    void setHandle(DAQHandle* _handle){handle=_handle;}
    DAQHandle* getHandle(){return handle;}
    void setJson(Json::Value _json){json=_json;}
    Json::Value getJson(){return json;}
    long address(long offset){return baseAddress*0x10000+offset;}
protected:
  DAQHandle* handle;
  int crateID;
  int moduleID;
  vector<FEEIRQ>IRQ;
  long baseAddress;
  Json::Value json;
};
#endif
