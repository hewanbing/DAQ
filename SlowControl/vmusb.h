#ifndef VMUSB_H
#define VMUSB_H

#include <QObject>
#include "vmehandle.h"
#include "libxxusb.h"

class VMUSB:public VMEHandle
{
    Q_OBJECT
public:
    VMUSB(VMEHandle *parent=nullptr);
    virtual bool setBit(short AM, long address, long data, int bitLow, int bitHigh,int dataWidth);
    virtual bool getBit(short AM, long address, long *data, int bitLow, int bitHigh,int dataWidth);
    virtual QString getType() override;
    virtual bool checkConnection() override;
private:
    xxusb_device_type devices[100];
    struct usb_device *dev;
    usb_dev_handle *udev;

};

#endif // VMUSB_H
