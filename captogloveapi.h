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
#include <QtEndian>
#include <QThread>


class CaptoGloveAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant devicesList READ getDevices NOTIFY devicesUpdated)
    Q_PROPERTY(QVariant servicesList READ getServices NOTIFY servicesUpdated)
    Q_PROPERTY(QVariant charactersiticsList READ getCharacteristics NOTIFY characteristicsUpdated)
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool controllerError READ hasControllerError)
    Q_PROPERTY(bool alive READ alive NOTIFY aliveChanged)

public:
    CaptoGloveAPI(QObject *parent, QString configPath);
    ~CaptoGloveAPI();

    QVariant getDevices();                                                                  // xx
    QVariant getServices();                                                                 // xx
    QVariant getCharacteristics();                                                          // xx
    QString getUpdate();                                                                    // xx
    bool alive() const;
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
    void addLowEnergyService (const QBluetoothUuid &uuid);                                  // xx
    void deviceConnected();                                                                 // xx
    void deviceDisconnected();                                                              // xx
    void errorReceived();                                                                   // xx
    void serviceScanDone();                                                                 // xx

    // QLowEnergyService related
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);

Q_SIGNALS:
    void devicesUpdated();
    void servicesUpdated();
    void characteristicsUpdated();
    void updateChanged();
    void stateChanged();
    void disconnected();
    void aliveChanged();

private:
    // QLowEnergyController
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void checkServiceStatus(const QBluetoothUuid &uuid);

    // Generic Access
    void GAServiceStateChanged(QLowEnergyService::ServiceState s);

    // Battery service
    void batteryServiceStateChanged (QLowEnergyService::ServiceState s);
    void updateBatteryLevelValue(const QLowEnergyCharacteristic &c,
                                 const QByteArray &value);
    void confirmedBatteryDescWrite(const QLowEnergyDescriptor &d,
                                  const QByteArray &value);




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
    QMap<QBluetoothUuid, CharacteristicInfo*> m_characteristics;

    int m_scanTimeout;

    // Service flags
    bool m_randomAdress;
    bool m_foundGAService;
    bool m_foundBatteryLevelService;
    bool m_foundScanParamsService;
    bool m_foundHIDService;
    bool m_foundHIDControlPoint;

    // Control params
    bool m_reconnect = true;

    // Services -> it's better to have only one list which contains those services
    QLowEnergyService *m_batteryLevelService=nullptr;
    QLowEnergyService *m_GAService = nullptr;

    // Characteristics
    QLowEnergyDescriptor m_batteryNotificationDesc;
    bool m_connected;
    bool m_deviceScanState;
    QString m_message;
    DeviceInfo m_peripheralDevice;
};

#endif // CAPTOGLOVEAPI_H

