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
#include <QMetaEnum>
#include <QTimer>


class CaptoGloveAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant devicesList READ getDevices NOTIFY devicesUpdated)
    Q_PROPERTY(QVariant servicesList READ getServices NOTIFY servicesUpdated)
    Q_PROPERTY(QVariant charactersiticsList READ getCharacteristics NOTIFY characteristicsUpdated)
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool controllerError READ hasControllerError)

public:
    CaptoGloveAPI(QObject *parent, QString configPath);
    ~CaptoGloveAPI();

    QVariant getDevices();                                                                  // xx
    QVariant getServices();                                                                 // xx
    QVariant getCharacteristics();                                                          // xx
    QString getUpdate();                                                                    // xx
    bool state();                                                                           // xx
    bool hasControllerError() const;                                                        // xx

    bool isRandomAddress() const;

    void saveSettings       (QString path);                                                 // init, TODO
    void loadSettings       (QString path);                                                 // init, TODO

    void initializeController (const QBluetoothDeviceInfo &info);                                  // xx, TODO: Initialize controller

    void start();                                                                           // init, TEST method
    void processLoop();                                                                     // TODO: implement

public slots:
    void startDeviceDiscovery();                                                            // xx
    void scanServices(DeviceInfo &device);                          // xx


    void connectToService(const QString &uuid);
    void disconnectFromDevice();

private slots:
    // QBluetoothDeviceDiscoveryAgent
    void addDevice(const QBluetoothDeviceInfo &device);                                     // xx
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);                      // xx
    void deviceScanFinished();

    // QLowEnergyController related
    void addLowEnergyService (const QBluetoothUuid &uuid);
    void deviceConnected();
    void deviceDisconnected();
    void errorReceived();
    void serviceScanDone();

    // QLowEnergyService related
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);

Q_SIGNALS:
    void devicesUpdated();
    void serviceDiscovered();
    void servicesUpdated();
    void characteristicsUpdated();
    void updateChanged();
    void stateChanged();
    void disconnected();

private:
    void setUpdate(const QString &message);
    void getDeviceName();
    void readBatteryLevel();
    void getScanParams();

    QString m_configPath;

    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothLocalDevice *localDevice;

    QLowEnergyController* m_controller;
    QList<DeviceInfo *> m_devices;
    QList<QBluetoothDeviceInfo> m_devicesBTInfo;
    QList<ServiceInfo *> m_services;
    QList<CharacteristicInfo *> m_characteristics;

    int m_BLEScanTimeout;
    bool m_randomAdress;
    bool m_connected;
    bool m_deviceScanState;
    QString m_message;
    DeviceInfo m_peripheralDevice;
};

#endif // CAPTOGLOVEAPI_H

