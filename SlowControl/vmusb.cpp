#include "vmusb.h"
#include <QDebug>
#include "myData.h"
VMUSB::VMUSB(VMEHandle *parent):VMEHandle(parent)
{
    udev=nullptr;
}
bool VMUSB::setBit(short AM, long address, long data, int bitLow, int bitHigh, int dataWidth){
    int ret=0;
    if(dataWidth==16){
        long tmp;
        ret = VME_read_16(udev,AM,address,&tmp);
        long _data=data;
        _data=_data<<bitLow;
        myData mData(tmp);
        tmp=mData.set0(bitLow,bitHigh);
        _data+=tmp;
        ret = VME_write_16(udev,AM,address,_data);
    }else if(dataWidth==32){
        long tmp1,tmp2,tmp3;
        VME_read_16(udev,AM,address,&tmp1);
        VME_read_16(udev,AM,address+2,&tmp2);
        tmp3=tmp1*pow(2,16)+tmp2;
        myData mData1(tmp3);
        long data1=(data<<bitLow)+mData1.set0(bitLow,bitHigh);
        myData mData2(data1);
        VME_write_16(udev,AM,address,mData2.get(16,31));
        ret = VME_write_16(udev,AM,address+2,mData2.get(0,15));
    }
    if(ret<0)return false;
    return true;
}
bool VMUSB::getBit(short AM, long address, long *data, int bitLow, int bitHigh, int dataWidth){
    int ret=0;
    long buf=0;
    ret = VME_read_16(udev,AM,address,&buf);
    if(dataWidth==32){
        long buf2=0;
        VME_read_16(udev,AM,address+2,&buf2);
        buf=buf*pow(2,16)+buf2;
    }
    if(ret<0)return false;
    myData mData(buf);
    buf=mData.get(bitLow,bitHigh);
    memcpy(data,&buf,sizeof(long));
    return true;
}
QString VMUSB::getType(){
    return QString("VMUSB");
}
bool VMUSB::checkConnection(){
    if(udev==nullptr){
      int ret = xxusb_devices_find(devices);
      if(ret<0) return false;
      dev = devices[0].usbdev;
      udev = xxusb_device_open(dev);
      if(!udev) return false;
      qDebug()<<"USB device found";
      long tmp;
      VME_register_read(udev, 0x0, &tmp);
    }
    long tmp;
    int ret=VME_register_read(udev, 0x0, &tmp);
    if(ret <0)return false;
    qDebug()<<"VMUSB checked";
    return true;
}
