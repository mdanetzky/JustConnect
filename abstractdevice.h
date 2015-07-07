#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

class AbstractDevice
{
public:
    AbstractDevice();
    bool isConnected();
    void connect();
    void disconnect();

};

#endif // ABSTRACTDEVICE_H
