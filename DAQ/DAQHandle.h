#ifndef __DAQHandle_h__
#define __DAQHandle_h__
#include <json/json.h>
#include "Channel.h"
#include "BufferTP.h"

class DAQHandle {
public:
    DAQHandle(){}
    virtual ~DAQHandle(){}
    virtual bool init(){return false;}
    virtual bool start(){return false;}
    virtual bool step(){return false;}
    virtual bool pause(){return false;}
    virtual bool resume(){return false;}
    virtual bool stop(){return false;}
    virtual bool setBit(short AM, long address, long data, int bitLow,int bitHigh, int dataWidth=16){}
    virtual long getBit(short AM, long address,int bitLow,int bitHigh, int dataWidth=16){}
    virtual bool write(short AM, long address, long data, int dataWidth=16){}
    virtual long read(short AM, long address, int dataWidth=16){}

    Json::Value getPatternJson(){return patternJson;}
    void setPatternJson(Json::Value json){patternJson=json;}
    void setBuffer(int l,int n){ buffer=new BufferTP<Channel>(l,n);}
    int getBufferLength(){return buffer->getBufferLength();}
    int getBufferNumber(){return buffer->getBufferNumber();}
    int getBufferCharSize(){return buffer->getBufferCharSize();}
    void* getBuffer4Read(){return buffer->getBuffer4Read();}
    BufferTP<Channel>* getBufferPtr(){return buffer;}
    void readDone(){return buffer->readDone();}
    int getBufferStatus(){return buffer->getStatus();}

protected:
    Json::Value patternJson;
    BufferTP<Channel>*buffer;

};
#endif
