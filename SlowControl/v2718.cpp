#include "v2718.h"
#include <QDebug>
#include <math.h>
#include "myData.h"

v2718::v2718(VMEHandle* parent):VMEHandle(parent)
{
    BHandle=-1;
}
bool v2718::setBit(short _AM, long address, long data, int bitLow, int bitHigh,int dataWidth){
    CVAddressModifier AM=cvA16_S;
    if(_AM==0x29)AM=cvA16_U;
    CVErrorCodes Code;
    if(dataWidth==16){
        int tmp=0;
        Code=CAENVME_ReadCycle(BHandle,address,&tmp,AM,cvD16);
        if(Code!=cvSuccess)return false;
        int _data=data<<bitLow;
        myData mData(tmp);
        tmp=mData.set0(bitLow,bitHigh);
        _data+=tmp;
        Code=CAENVME_WriteCycle(BHandle,address,&_data,AM,cvD16);
        if(Code!=cvSuccess)return false;
    }else if(dataWidth==32){
        int tmp1=0;
        int tmp2=0;
        int tmp3=0;
        Code=CAENVME_ReadCycle(BHandle,address,&tmp1,AM,cvD16);
        if(Code!=cvSuccess)return false;
        Code=CAENVME_ReadCycle(BHandle,address+2,&tmp2,AM,cvD16);
        if(Code!=cvSuccess)return false;
        tmp3=tmp1*pow(2,16)+tmp2;
        myData mData1(tmp3);
        int data1=(data<<bitLow)+mData1.set0(bitLow,bitHigh);
        myData mData2(data1);
        int data2=mData2.get(16,31);
        int data3=mData2.get(0,15);
        Code=CAENVME_WriteCycle(BHandle,address,&data2,AM,cvD16);
        if(Code!=cvSuccess)return false;
        Code=CAENVME_WriteCycle(BHandle,address+2,&data3,AM,cvD16);
        if(Code!=cvSuccess)return false;
    }
   return true;
}
bool v2718::getBit(short _AM, long address, long *data, int bitLow, int bitHigh,int dataWidth){
    CVAddressModifier AM=cvA16_S;
    if(_AM==0x29)AM=cvA16_U;
    int buf=0;
    CVErrorCodes Code=CAENVME_ReadCycle(BHandle,address,&buf,AM,cvD16);
    if(dataWidth==32){
        int buf2=0;
        Code=CAENVME_ReadCycle(BHandle,address+2,&buf2,AM,cvD16);
        buf=buf*pow(2,16)+buf2;
    }
    if(Code!=cvSuccess)return false;
    myData mData(buf);
    long buf3=mData.get(bitLow,bitHigh);
    memcpy(data,&buf3,sizeof(long));
    return true;
}
QString v2718::getType(){
    return QString("v2718");
}
bool v2718::checkConnection(){
    if(BHandle<0){
        CVErrorCodes Code=CAENVME_Init(VMEBoard,Link,Device,&BHandle);
        if(Code!=cvSuccess){
            BHandle=-1;
            qDebug()<<"v2718 not found";
            return false;
        }
    }
    qDebug()<<"v2718 connected";
    return true;
}
