#ifndef CAPTOGLOVEAPI_H
#define CAPTOGLOVEAPI_H

// Bluetooth include
#include "qbluetooth.h"
#include "qbluetoothdevicediscoveryagent.h"
#include "qbluetoothlocaldevice.h"

// Q include
#include "qlowenergycontroller.h"
#include "qfile.h"
#include "qsettings.h"

#include "deviceinfo.h"
#include "serviceinfo.h"
#include "characteristicinfo.h"

// Specific datatypes include
#include <QDebug>


class CaptoGloveAPI : public QObject
{
    Q_OBJECT
public:
    CaptoGloveAPI(QObject *parent, QString configPath);
    ~CaptoGloveAPI();

    void saveSettings       (QString path);
    void loadSettings       (QString path);

    void initializeController (DeviceInfo centralDevice);

    void discoverServices(DeviceInfo device, int timeout);
    void readCharacteristic(CharacteristicInfo characteristic, int timeout);
    void writeCharacteristicReq(CharacteristicInfo characteristic,
                                QByteArray Data,
                                int timeout);

    bool isAlive();

    void start();
    void processLoop();

public slots:
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void addDevice(const QBluetoothDeviceInfo &device);
    void deviceScanFinished();

    void deviceConnected();
    void deviceDisconnected();
    void errorReceived();
    void addLowEnergyService();
    void serviceScanDone();

private:

    void getDeviceName();
    void readBatteryLevel();
    void getScanParams();

    QString m_configPath;

    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    QString localDeviceName;

    QLowEnergyController* m_controller;
    QList<DeviceInfo *> m_devices;
    QList<ServiceInfo> m_services;
    QList<CharacteristicInfo> m_characteristics;

    int m_BLEScanTimeout;
    bool m_connected;
};

#endif // CAPTOGLOVEAPI_H

