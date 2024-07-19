#ifndef V2718_H
#define V2718_H

#include <QObject>
#include "vmehandle.h"
#include <CAENVMElib.h>

class v2718:public VMEHandle
{
    Q_OBJECT
public:
    v2718(VMEHandle* patent=nullptr);
    virtual bool setBit(short AM, long address, long data, int bitLow, int bitHigh,int dataWidth);
    virtual bool getBit(short AM, long address, long *data, int bitLow, int bitHigh,int dataWidth);
    virtual QString getType() override;
    virtual bool checkConnection() override;
private:
    CVBoardTypes  VMEBoard=cvV2718;
    short         Link=0;
    short         Device=0;
    int32_t       BHandle;
};

#endif // V2718_H
