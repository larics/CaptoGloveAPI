#include "captogloveapi.h"


CaptoGloveAPI::CaptoGloveAPI(QObject *parent,  QString configPath) : QObject(parent), m_configPath(configPath)
{

    // Load or create default config file
    QFile configFile(m_configPath);
    if (m_configPath == "") m_configPath = tr("%1/%2").arg(PROJECT_PATH).arg("config.ini");

    // initialize Bluetooth Discovery agent
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    m_discoveryAgent->setLowEnergyDiscoveryTimeout(m_BLEScanTimeout);

    // Signal that reports found device
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(addDevice(QBluetoothDeviceInfo))); // Same  as 3rd one
    connect(m_discoveryAgent, static_cast<void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)>(&QBluetoothDeviceDiscoveryAgent::error),
                                               this, &CaptoGloveAPI::deviceScanError);
    //TODO: Add slots for cancelling descovery procedure and for finishing discovery procedure
    //connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
    //        this, &CaptoGloveAPI::deviceScanFinished);  // Same as 1st one, only difference is implementation (add them once, or add one by one)




}

CaptoGloveAPI::~CaptoGloveAPI(){

}



void CaptoGloveAPI::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    //TODO: Add error handling
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        qDebug() << (tr("The Bluetooth adaptor is powered off."));
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        qDebug() << (tr("Writing or reading from the device resulted in an error."));
    else
        qDebug() << (tr("An unknown error has occurred."));
}

void CaptoGloveAPI::deviceScanFinished()
{
    // This method just emits signals!
    //emit deviceScanFinished();

}

void CaptoGloveAPI::addDevice(const QBluetoothDeviceInfo &device){

    qDebug() << "Device name:" << device.name();
    qDebug() << "Device address:" << device.address();
    // DeviceInfo foundDevice;

    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration){
        m_devices.append(new DeviceInfo(device));
        qDebug() << "found device\n" << "==========\n" << device.name() << " " << device.address() << "\n" ;
    }
}

void CaptoGloveAPI::initializeController(DeviceInfo device)
{

    if (m_controller){
        m_controller->disconnectFromDevice();
        delete m_controller;
        m_controller = nullptr;
    }

    if (!m_controller) {
    // Connecting signals and slots for connecting to LE services.
    m_controller = QLowEnergyController::createCentral(device.getDevice());
    connect(m_controller, &QLowEnergyController::connected,
            this, &CaptoGloveAPI::deviceConnected);
    connect(m_controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
            this, &CaptoGloveAPI::errorReceived);
    connect(m_controller, &QLowEnergyController::disconnected,
            this, &CaptoGloveAPI::deviceDisconnected);
    connect(m_controller, &QLowEnergyController::serviceDiscovered,
            this, &CaptoGloveAPI::addLowEnergyService);
    connect(m_controller, &QLowEnergyController::discoveryFinished,
            this, &CaptoGloveAPI::serviceScanDone);
    }

    m_controller->setRemoteAddressType(QLowEnergyController::RandomAddress);


}

// LOW ENERGY CONTROLLER SLOTS
void CaptoGloveAPI::deviceConnected()
{
    m_connected = true;
    m_controller->discoverServices();
}

void CaptoGloveAPI::errorReceived()
{

}

void CaptoGloveAPI::deviceDisconnected()
{

}

void CaptoGloveAPI::addLowEnergyService()
{

}

void CaptoGloveAPI::serviceScanDone()
{

}





void CaptoGloveAPI::start(){

}


void CaptoGloveAPI::saveSettings(QString path){
    if (path == "") return;

    QSettings Setting(path, QSettings::IniFormat);

    //Setting.beginGroup("Bluetooth");
    //Logger::instance()->writeParameters(&Setting);
    //Setting.endGroup();


}

void CaptoGloveAPI::loadSettings(QString path){
    if (path == "") return;

    QSettings Setting(path, QSettings::IniFormat);

    //Setting.beginGroup("BluetoothParams");
    //BluetoothController::instance()->readParameters(&Setting);
    //Setting.endGroup();

    //m_controlSystem->readParameters(&Setting);
}


