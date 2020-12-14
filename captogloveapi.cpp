#include "captogloveapi.h"


CaptoGloveAPI::CaptoGloveAPI(QObject *parent,  QString configPath) : QObject(parent), m_configPath(configPath)
{

    m_randomAdress = true;
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

    // Set up local to check states and connectivity often

    localDevice = new QBluetoothLocalDevice();
    localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);


}

CaptoGloveAPI::~CaptoGloveAPI() {
}

// ##############  DEVICES ##############
void CaptoGloveAPI::startDeviceDiscovery()
{
    qDeleteAll(m_devices);
    m_devices.clear();
    emit devicesUpdated();

    m_discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    if (m_discoveryAgent->isActive()) {
        m_deviceScanState = true;
        Q_EMIT stateChanged();
    }
}

void CaptoGloveAPI::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    //TODO: Add error handling
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        setUpdate(tr("The Bluetooth adaptor is powered off."));
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        setUpdate(tr("Writing or reading from the device resulted in an error."));
    else{
        static QMetaEnum qme = m_discoveryAgent->metaObject()->enumerator(
                    m_discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        setUpdate("Error: " + QLatin1String(qme.valueToKey(error)));
    }

    m_deviceScanState = false;
    emit devicesUpdated();
    emit stateChanged();
}

void CaptoGloveAPI::deviceScanFinished()
{
    // This method just emits signals!
    //emit deviceScanFinished();

    const QList<QBluetoothDeviceInfo> foundDevices = m_discoveryAgent->discoveredDevices();
    for (auto nextDevice : foundDevices)
        if (nextDevice.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
            m_devicesBTInfo.append(nextDevice);

    emit devicesUpdated();
    m_deviceScanState = false;
    emit stateChanged();
    if (m_devicesBTInfo.isEmpty())
        setUpdate("No Low Energy devices found...");
    else
        setUpdate("Done! Scan Again!");
}

void CaptoGloveAPI::addDevice(const QBluetoothDeviceInfo &device){


    // DeviceInfo foundDevice;
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration){
        m_devices.append(new DeviceInfo(device));
        qDebug() << "Device name:" << device.name();
        qDebug() << "Device address:" << device.address();
    }
}

void CaptoGloveAPI::disconnectFromDevice(){

    if (m_controller->state() != QLowEnergyController::UnconnectedState)
        m_controller->disconnectFromDevice();
    else
        deviceDisconnected();
}

// ############## INITIALIZE CONTROLLER ##############
void CaptoGloveAPI::initializeController(const QBluetoothDeviceInfo &info)
{

    if (m_controller){
        m_controller->disconnectFromDevice();
        delete m_controller;
        m_controller = nullptr;
    }

    if (!m_controller) {
    // Connecting signals and slots for connecting to LE services. (arg is remoteAdress)
    m_controller = QLowEnergyController::createCentral(info);
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

    // Set remote address to random
    if (isRandomAddress())
        m_controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    else
        m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);

    qDebug() << "Connecting to device";
    m_controller->connectToDevice();


}

// ############## LOW ENERGY CONTROLLER SLOTS ##############
void CaptoGloveAPI::deviceConnected()
{
    setUpdate("Back\n(Discovering services...)");
    m_connected = true;
    m_controller->discoverServices();
}

void CaptoGloveAPI::deviceDisconnected()
{
    qWarning() << "Disconnected from the device!";
    emit disconnected();
}

void CaptoGloveAPI::errorReceived()
{
    qWarning() << "Error: " << m_controller->errorString();
    setUpdate(QString("Back\n(%1)").arg(m_controller->errorString()));
}

void CaptoGloveAPI::serviceScanDone(){

    setUpdate("Back\n(Service scan done!)");
    if (m_services.isEmpty())
        emit servicesUpdated();
}

bool CaptoGloveAPI::hasControllerError() const
{
    return (m_controller && m_controller->error() != QLowEnergyController::NoError);
}

// ############## SERVICES ##############
void CaptoGloveAPI::scanServices(DeviceInfo &device)        // TODO: Check why would I use address param
{

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();
    qDeleteAll(m_services);
    m_services.clear();
    emit servicesUpdated();

    setUpdate("Back\n(Connecting to device...)");

    if (m_controller)
    {
        m_controller->disconnectFromDevice();
        delete m_controller;
        m_controller = nullptr;
    }

    if (!m_controller)
    {
        QBluetoothDeviceInfo currentDevice = device.getDevice();
        initializeController(currentDevice);
    }



}

void CaptoGloveAPI::connectToService(const QString &uuid)
{
    QLowEnergyService *service = nullptr;
    for (auto s: qAsConst(m_services)){
        auto serviceInfo = qobject_cast<ServiceInfo *>(s);
        if(!serviceInfo)
            continue;
        if (serviceInfo->getUuid() == uuid){
            service = serviceInfo->service();
            break;
        }
    }

    if(!service)
        return;

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();

    if(service->state() == QLowEnergyService::DiscoveryRequired){
        connect(service, &QLowEnergyService::stateChanged,
                this, &CaptoGloveAPI::serviceDetailsDiscovered);
        service->discoverDetails();
        setUpdate("Back\n(Discovering details...)");
        return;
    }

    // Discovery already done
    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    for (const QLowEnergyCharacteristic &ch : chars){
        auto cInfo = new CharacteristicInfo(ch);
        m_characteristics.append(cInfo);
    }

    QTimer::singleShot(0, this, &CaptoGloveAPI::characteristicsUpdated);


}

void CaptoGloveAPI::addLowEnergyService(const QBluetoothUuid &uuid)
{
    QLowEnergyService *service = m_controller->createServiceObject(uuid);
    if (!service){
        qWarning() << "Cannot create service for uuid";
        return;
    }

    auto serv = new ServiceInfo(service);
    m_services.append(serv);

    emit servicesUpdated();
}

void CaptoGloveAPI::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered) {
        // do not hang in "Scanning for characteristics" mode forever
        // in case the service discovery failed
        if (newState != QLowEnergyService::DiscoveringServices){
            QMetaObject::invokeMethod(this, "characteristicsUpdated",
                                      Qt::QueuedConnection);
        }
        return;
    }
    auto service = qobject_cast<QLowEnergyService *>(sender());
    if(!service)
        return;

    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    for (const QLowEnergyCharacteristic &ch : chars){
        auto cInfo = new CharacteristicInfo(ch);
        m_characteristics.append(cInfo);
    }

    emit characteristicsUpdated();
}
// ############## CHARACTERISTICS ##############


// ############## FUNCTIONAL ##############
void CaptoGloveAPI::start(){


    m_discoveryAgent->start();

    QString choosenDevice("CaptoGlove3159");

    DeviceInfo *m_devicePtr;
    // TODO: Think of stopping if haven't discovered / connected to wanted device
    foreach(m_devicePtr, m_devices)
    {
        qDebug() << QString("Current device: %1").arg(m_devicePtr->getName());

        if (m_devicePtr->getName().contains(choosenDevice))
        {
            m_peripheralDevice.setDevice(m_devicePtr->getDevice());
            break;
        }else{
            qDebug() << "Wanted Peripheral is not found!";
            // TODO: Add error handling if device hasn't been found
        }
    }

    // Start Service discovery
    scanServices(m_peripheralDevice);



    // InitializeController


}

void CaptoGloveAPI::setUpdate(const QString &message)
{
    m_message = message;
    emit updateChanged();
}

// ############## SETTINGS ##############
void CaptoGloveAPI::saveSettings(QString path){
    if (path == "") return;

    QSettings Setting(path, QSettings::IniFormat);

    //Setting.beginGroup("BluetoothParams");
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

// ############## GETTERS ##############
QVariant CaptoGloveAPI::getDevices(){

    return QVariant::fromValue(m_devices);
}

QVariant CaptoGloveAPI::getServices(){

    return QVariant::fromValue(m_services);
}

QVariant CaptoGloveAPI::getCharacteristics(){

    return QVariant::fromValue(m_characteristics);
}

bool CaptoGloveAPI::isRandomAddress() const
{
    return m_randomAdress;
}

bool CaptoGloveAPI::state()
{
    return m_deviceScanState;
}

