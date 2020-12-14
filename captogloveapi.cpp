#include "captogloveapi.h"


CaptoGloveAPI::CaptoGloveAPI(QObject *parent,  QString configPath) : QObject(parent), m_configPath(configPath)
{

    // Initial setup --> TODO: Add reading from config.ini file
    m_randomAdress = true;
    m_foundGAService = false;
    m_foundBatteryLevelService = false;
    m_foundScanParamsService = false;
    m_foundHIDService = false;
    //m_foundHIDInformation = false;
    //m_foundProtocolMode = false;
    m_foundHIDControlPoint = false;


    m_reconnect = true;
    m_scanTimeout = 5000;

    // Load or create default config file
    QFile configFile(m_configPath);
    if (m_configPath == "") m_configPath = tr("%1/%2").arg(PROJECT_PATH).arg("config.ini");

    // initialize Bluetooth Discovery agent
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    m_discoveryAgent->setLowEnergyDiscoveryTimeout(m_scanTimeout);

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
    // Stop discovery agent
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

    qDebug() << tr("Connecting to device: %1").arg(info.name());


}


// ############## LOW ENERGY CONTROLLER SLOTS ##############
void CaptoGloveAPI::deviceConnected()
{
    qDebug() << "Device connected. Scanning services.";
    setUpdate("Back\n(Discovering services...)");
    m_connected = true;
    m_controller->discoverServices();
}

void CaptoGloveAPI::deviceDisconnected()
{
    qWarning() << "Disconnected from the device!";
    emit disconnected();
    // TODO: Add  reconnection logic

    if (m_reconnect && m_controller)
        m_controller->connectToDevice();
}

void CaptoGloveAPI::errorReceived()
{
    qWarning() << "Error: " << m_controller->errorString();
    setUpdate(QString("Back\n(%1)").arg(m_controller->errorString()));
}

void CaptoGloveAPI::serviceScanDone(){

    qDebug() << "Service scan done!!!!!";
    if (m_services.isEmpty())
        emit servicesUpdated();

    if (m_foundBatteryLevelService)
        qDebug() << "Battery Level service found!";
        m_batteryLevelService = m_controller->createServiceObject(QBluetoothUuid::BatteryService);
        m_batteryLevelService->discoverDetails();

    if (m_batteryLevelService){
        connect(m_batteryLevelService, &QLowEnergyService::stateChanged,  this, &CaptoGloveAPI::batteryServiceStateChanged);
        connect(m_batteryLevelService, &QLowEnergyService::characteristicChanged, this, &CaptoGloveAPI::updateBatteryLevelValue);
        connect(m_batteryLevelService, &QLowEnergyService::descriptorWritten, this, &CaptoGloveAPI::confirmedBatteryDescWrite);
        connectToService("0x180f");
    }else {
        qDebug() << "Battery Level service not found.";
    }

    if (m_foundGAService){
        m_GAService = m_controller->createServiceObject(QBluetoothUuid::GenericAccess);
        m_GAService->discoverDetails();
    }
    if (m_GAService){
        connectToService(QString(QBluetoothUuid::GenericAccess));
    }

    processLoop();

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

    m_controller->connectToDevice();

}

void CaptoGloveAPI::connectToService(const QString &uuid)
{
    QLowEnergyService *service = nullptr;
    for (auto s: qAsConst(m_services)){
        auto serviceInfo = qobject_cast<ServiceInfo *>(s);
        qDebug() << "Current uuid is"<< serviceInfo->getUuid();
        if(!serviceInfo)
            continue;
        if (serviceInfo->getUuid() == uuid){
            service = serviceInfo->service();
            break;
        }
    }

    if(!service)
        return;


    if(service->state() == QLowEnergyService::DiscoveryRequired){
        connect(service, &QLowEnergyService::stateChanged,
                this, &CaptoGloveAPI::serviceDetailsDiscovered);
        service->discoverDetails();
        setUpdate("Back\n(Discovering details...)");
        return;
    }

    // ### NASTY ADDITION!
    service->discoverDetails();

    // Discovery already done
    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    for (const QLowEnergyCharacteristic &ch : chars){
        auto cInfo = new CharacteristicInfo(ch);
        m_characteristics.insert(service->serviceUuid(), cInfo);
    }

    QTimer::singleShot(0, this, &CaptoGloveAPI::characteristicsUpdated);


}

void CaptoGloveAPI::addLowEnergyService(const QBluetoothUuid &uuid)
{
    QLowEnergyService *service = m_controller->createServiceObject(uuid);       // This part already creates service object in controller
    if (!service){
        qWarning() << "Cannot create service for uuid";
        return;
    }

    auto serv = new ServiceInfo(service);
    m_services.append(serv);

    if(!service)
        return;

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();

    checkServiceStatus(uuid);

    emit servicesUpdated();
}

void CaptoGloveAPI::checkServiceStatus(const QBluetoothUuid &uuid)
{

    if (uuid == QBluetoothUuid(QBluetoothUuid::GenericAccess) && !m_foundGAService){
        qDebug() << "Discovered Generic Access Service!";
        m_foundGAService = true;
    }

    if (uuid == QBluetoothUuid(QBluetoothUuid::BatteryService) && !m_foundBatteryLevelService){
            qDebug() << "Discovered Battery Level Service!";
            m_foundBatteryLevelService = true;
    }

    if (uuid == QBluetoothUuid(QBluetoothUuid::ScanParameters) && !m_foundScanParamsService){

        qDebug() << "Discovered ScanParameters Service!";
        m_foundScanParamsService = true;
    }

    if (uuid == QBluetoothUuid(QBluetoothUuid::HumanInterfaceDeviceService) && !m_foundHIDService){

        qDebug() << "Discovered HID Service";
        m_foundHIDService = true;
    }

    if (uuid == QBluetoothUuid(QBluetoothUuid::HIDControlPoint) && !m_foundHIDControlPoint)
    {
        qDebug() << "Found HID Control Point.";
        m_foundHIDControlPoint = true;
    }

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
        m_characteristics.insert(service->serviceUuid(), cInfo);
    }

    emit characteristicsUpdated();
}

// BATTERY SERVICE
void CaptoGloveAPI::batteryServiceStateChanged(QLowEnergyService::ServiceState s)
{
    switch(s){
    case QLowEnergyService::DiscoveringServices:
        qDebug() << "Discovering services...";
        break;

    case QLowEnergyService::ServiceDiscovered:
    {

        qInfo() << "Battery Service changed!";

        const QLowEnergyCharacteristic batteryLevelChar = m_batteryLevelService->characteristic(QBluetoothUuid(QBluetoothUuid::BatteryLevel));
        if (!batteryLevelChar.isValid()) {
            qDebug("Battery level data not found.");
            break;
        }
        m_batteryNotificationDesc = batteryLevelChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);

        if (m_batteryNotificationDesc.isValid())
            qDebug() << "Writing battery level notifications";
            m_batteryLevelService->writeDescriptor(m_batteryNotificationDesc, QByteArray::fromHex("0100"));
        break;

    }
    default:
        // nothing for now
        break;
    }

    emit aliveChanged();

}

void CaptoGloveAPI::updateBatteryLevelValue(const QLowEnergyCharacteristic &c, const QByteArray &value  )
{
    // ignore any other characteristic change -> shouldn't really happen though
    if (c.uuid() != QBluetoothUuid(QBluetoothUuid::BatteryLevel))
        return;

    auto data = reinterpret_cast<const quint8 *>(value.constData());
    quint8 flags = *data;

    //Heart Rate
    int blvalue = 0;
    if (flags & 0x1) // HR 16 bit? otherwise 8 bit
        blvalue = static_cast<int>(qFromLittleEndian<quint16>(data[1]));
    else
        blvalue = static_cast<int>(data[1]);

    qDebug() << "Battery level is: " << blvalue;
}

void CaptoGloveAPI::confirmedBatteryDescWrite(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    if (d.isValid() && d == m_batteryNotificationDesc && value == QByteArray::fromHex("0000")) {
        //disabled notifications -> assume disconnect intent
        m_controller->disconnectFromDevice();
        delete m_batteryLevelService;
        m_batteryLevelService = nullptr;
    }
}

void CaptoGloveAPI::readBatteryLevel()
{
    QLowEnergyCharacteristic batteryLevel;
    CharacteristicInfo *charPtr;

    if (m_batteryLevelService->state() == QLowEnergyService::DiscoveryRequired){
        qDebug() << "Discovery required for service!";
    }else {
        qDebug() << "Everything is ok!";
    }

    foreach(charPtr, m_characteristics)
    {
        qDebug() << "Entered here!";
        if (charPtr->getUuid() == QBluetoothUuid::BatteryLevel) batteryLevel = charPtr->getCharacteristic();
        m_batteryLevelService->readCharacteristic(batteryLevel);

    }

    qDebug() << "Battery value is: " << batteryLevel.value();

}

// ############## FUNCTIONAL ##############
void CaptoGloveAPI::start(){


    startDeviceDiscovery();
    QString choosenDevice("CaptoGlove3148");

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
    scanServices(m_peripheralDevice); // TODO: Maybe break controller initialization and scanning for services in two method calls


}

void CaptoGloveAPI::processLoop(){
    while(true)
    {
        QThread::msleep(1000);
        readBatteryLevel();
    }
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

bool CaptoGloveAPI::alive() const
{
    if (m_batteryLevelService)
        return m_batteryLevelService->state() == QLowEnergyService::ServiceDiscovered;

    return false;
}
