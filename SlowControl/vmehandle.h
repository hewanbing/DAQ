#ifndef VMEHANDLE_H
#define VMEHANDLE_H

#include <QObject>

class VMEHandle: public QObject
{
    Q_OBJECT
public:
    VMEHandle(QObject *parent = nullptr);
    virtual bool setBit(short AM, long address, long data, int bitLow, int bitHigh,int dataWidth);
    virtual bool getBit(short AM, long address, long *data, int bitLow, int bitHigh,int dataWidth);
    virtual QString getType();
    virtual bool checkConnection();
};

#endif // VMEHANDLE_H
