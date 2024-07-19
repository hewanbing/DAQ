#include "vhs.h"
#include <QDebug>
#include "myData.h"
#include "IEEE754.h"
VHS::VHS(VMEHandle* _handle, QObject* parent):QObject(parent)
{
    handle=_handle;
    AM=0x29;
    ChAddr[0]=0x60;
    ChAddr[1]=0x90;
    ChAddr[2]=0xC0;
    ChAddr[3]=0xF0;
    IsConnected=false;
    SN=0;
}
void VHS::setBase(long _base){
    base=_base;
    long data;
    bool ret=getBit(0x34,&data,0,31,32);
    if(ret){
        IsConnected=true;
        SN=data;
        qDebug()<<"SN: "<<hex<<SN;
    }
}
bool VHS::checkFirmware(){
    long data=0;
    bool status= getBit(0x38,&data,0,31,32);
    qDebug()<<"Firmware is "<<hex<<data;
    return status;
}
bool VHS::setBit(long offset, long data, int bitLow, int bitHigh, int dataWidth){
    return handle->setBit(AM,base+offset,data,bitLow,bitHigh,dataWidth);
}
bool VHS::getBit(long offset, long *data, int bitLow, int bitHigh, int dataWidth){
    return handle->getBit(AM,base+offset,data,bitLow,bitHigh,dataWidth);
}
bool VHS::refresh(){
    long moduleStatus=0;
    bool ret=getBit(0,&moduleStatus,0,15,16);
    myData data(moduleStatus);
    IsKillEnable=data.get(15);
    IsTemperatureGood=data.get(14);
    IsSupplyGood=data.get(13);
    IsModuleGood=data.get(12);
    IsEventActive=data.get(11);
    IsSafetyLoopGood=data.get(10);
    IsNoRamp=data.get(9);
    IsNoSumError=data.get(8);
    IsCommandComplete=data.get(7);
    IsSpecialMode=data.get(6);
    IsInputError=data.get(5);
    IsServiceNeeded=data.get(4);
    IsStop=data.get(3);
    IsInterlockOutput=data.get(2);
    IsAdjustment=data.get(1);
    Reserved=data.get(0);
    long tmp;
    for(int i=0;i<4;i++){
        getBit(ChAddr[i]+16,&tmp,0,31,32);
        Voltage[i]=IEEE754::toFloat(tmp);
        getBit(ChAddr[i]+20,&tmp,0,31,32);
        Current[i]=IEEE754::toFloat(tmp)*1000000;
        getBit(ChAddr[i],&tmp,0,15,16);
        myData _tmp(tmp);
        IsChannelOn[i]=_tmp.get(3);
    }
    ret = getBit(0x30,&tmp,0,31,32);
    temperature=IEEE754::toFloat(tmp);
    for(int i=0;i<4;i++){
        getBit(ChAddr[i]+8,&tmp,0,31,32);
        voltageSet[i]=IEEE754::toFloat(tmp);
    }
    return true;
}
void VHS::setVoltage(int ch, float v){
    myData voltage(IEEE754::toInt(v));
    //long data=voltage.get(0,31);
    setBit(ChAddr[ch]+8,voltage.get(0,31),0,31,32);
}
int VHS::getSN(){
    return SN;
}
void VHS::turnChannelOn(int ch){
    setBit(ChAddr[ch]+2,1,3,3,16);
}
void VHS::turnChannelOff(int ch){
    setBit(ChAddr[ch]+2,0,3,3,16);
}
bool VHS::channelSwitch(int ch){
    if(IsChannelOn[ch]){
        turnChannelOff(ch);
    }else{
        turnChannelOn(ch);
    }
}
