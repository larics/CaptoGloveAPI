#include "captogloveapi.h"


CaptoGloveAPI::CaptoGloveAPI(QObject *parent,  QString configPath) : QObject(parent), m_configPath(configPath)
{

    // Initial setup --> TODO: Add reading from config.ini file
    m_randomAdress = true;
    m_discoveredServices = false;
    m_foundGAService = false;
    m_foundBatteryLevelService = false;
    m_foundScanParamsService = false;
    m_foundHIDService = false;
    //m_foundHIDInformation = false;
    //m_foundProtocolMode = false;
    m_foundHIDControlPointService = false;

    m_connected = false;


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

    connect(this,SIGNAL(servicesDiscovered()), this, SLOT(serviceScanDone()));
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
            this, &CaptoGloveAPI::discoverServices);
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

    // Battery service
    if (m_foundBatteryLevelService){
        qDebug() << "Battery Level service found!";
        m_batteryLevelService = m_controller->createServiceObject(QBluetoothUuid::BatteryService);
        connect(m_batteryLevelService, &QLowEnergyService::stateChanged, this, &CaptoGloveAPI::batteryServiceStateChanged);
        connect(m_batteryLevelService, &QLowEnergyService::characteristicChanged, this, &CaptoGloveAPI::updateBatteryLevelValue);
        connect(m_batteryLevelService, &QLowEnergyService::descriptorWritten, this, &CaptoGloveAPI::confirmedBatteryDescWrite);
    }
    if (m_batteryLevelService){
        m_batteryLevelService->discoverDetails();
    }

    // Generic access service
    if (m_foundGAService){
        m_GAService = m_controller->createServiceObject(QBluetoothUuid::GenericAccess);
        connect(m_GAService, &QLowEnergyService::stateChanged, this, &CaptoGloveAPI::genericAccessServiceStateChanged);
    }
    if (m_GAService){
        qDebug() << "Discovering GA details";
        m_GAService->discoverDetails();
    }

    // Scan parameters service
    if (m_foundScanParametersService){
        m_ScanParametersService = m_controller->createServiceObject(QBluetoothUuid::ScanParameters);
        connect(m_ScanParametersService, &QLowEnergyService::stateChanged, this, &CaptoGloveAPI::scanParamsServiceStateChanged);
    }
    if (m_ScanParametersService){
        qDebug() << "Discovering Scan Parameters details";
        m_ScanParametersService->discoverDetails();
    }

    // Human interface device service
    if (m_foundHIDService){
        m_HIDService = m_controller->createServiceObject(QBluetoothUuid::HumanInterfaceDevice);
        connect(m_HIDService, &QLowEnergyService::stateChanged, this, &CaptoGloveAPI::HIDserviceStateChanged);
    }
    if (m_HIDService){
        qDebug() << "Current HID service state is: " << m_HIDService->state();
        m_HIDService->discoverDetails();

    }

    // Finger position service
    if (m_foundFingerPositionService){
        m_FingerPositionsService = m_controller->createServiceObject(QBluetoothUuid(QString("ff005")));
    }
    if (m_FingerPositionsService)
    {
        m_FingerPositionsService->discoverDetails();

        m_connected = true;
    }

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

    // Discovery already done
    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    for (const QLowEnergyCharacteristic &ch : chars){
        auto cInfo = new CharacteristicInfo(ch);
        m_characteristics.insert(service->serviceUuid(), cInfo);
    }

    QTimer::singleShot(0, this, &CaptoGloveAPI::characteristicsUpdated);


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

void CaptoGloveAPI::addLowEnergyService(const QBluetoothUuid &uuid)
{
    QLowEnergyService *service = m_controller->createServiceObject(uuid);       // This part already creates service object in controller

    if (!service){
        qWarning() << "Cannot create service for uuid";
        return;
    }
    qDebug() << "Adding service" << uuid;
    auto serv = new ServiceInfo(service);
    m_services.append(serv);

    if(!service)
        return;

    checkServiceStatus(uuid);

    emit servicesUpdated();
}

void CaptoGloveAPI::checkServiceStatus(const QBluetoothUuid &uuid)
{

    if (uuid == QBluetoothUuid::GenericAccess && !m_foundGAService){
        qDebug() << "Discovered Generic Access Service!";
        m_foundGAService = true;
    }

    else if (uuid == QBluetoothUuid::BatteryService && !m_foundBatteryLevelService){
        qDebug() << "Discovered Battery Level Service!";
        m_foundBatteryLevelService = true;
    }

    else if (uuid == QBluetoothUuid::ScanParameters && !m_foundScanParamsService){

        qDebug() << "Discovered ScanParameters Service!";
        m_foundScanParamsService = true;
    }

    else if (uuid == QBluetoothUuid::HumanInterfaceDeviceService && !m_foundHIDService){

        qDebug() << "Discovered HID Service";
        m_foundHIDService = true;
    }

    else if (uuid == QBluetoothUuid::DeviceInformation && !m_foundDeviceInfoService)
    {
        qDebug() << "Found Device information service.";
        m_foundDeviceInfoService = true;
    }

    else if (uuid == QBluetoothUuid(QString("ff05")) && !m_foundFingerPositionService)
    {
        qDebug() << "Found finger position service.";
        m_foundFingerPositionService = true;
    }

    else
    {
        qDebug() << "Service hasn't been found!";
    }



}

void CaptoGloveAPI::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Discovering  GA details in state: " << s ;
    switch(s){
    case QLowEnergyService::DiscoveringServices:
    {
        qDebug() << "Discovering services...";
        break;


    }
    case QLowEnergyService::ServiceDiscovered:
    {
            const QList<QLowEnergyCharacteristic> chars = m_batteryLevelService->characteristics();
            for (const QLowEnergyCharacteristic &ch : chars){
                auto cInfo = new CharacteristicInfo(ch);
                m_characteristics.insert(m_batteryLevelService->serviceUuid(), cInfo);
                qDebug() << "Characteristic uuid is: " << cInfo->getUuid();
                qDebug() << "Characteristic name is: " << cInfo->getName();

        }
    }

    default:
        qDebug() << "Not wanted  state!";

    emit aliveChanged();
    }
}

// BATTERY SERVICE
void CaptoGloveAPI::batteryServiceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Discovering Battery details in state: " << s ;
    switch(s){
    case QLowEnergyService::DiscoveringServices:
    {
        qDebug() << "Discovering services...";
        break;

    }
    case QLowEnergyService::ServiceDiscovered:
    {
        const QList<QLowEnergyCharacteristic> chars = m_batteryLevelService->characteristics();
        for (const QLowEnergyCharacteristic &ch : chars){
            auto cInfo = new CharacteristicInfo(ch);
            m_characteristics.insert(m_batteryLevelService->serviceUuid(), cInfo);
            qDebug() << "Characteristic uuid is: " << cInfo->getUuid();
            qDebug() << "Characteristic name is: " << cInfo->getName();
        }


        if (!chars.empty())
        {
            const QLowEnergyCharacteristic batteryLevelChar = m_batteryLevelService->characteristic(QBluetoothUuid::BatteryLevel);
            if (!batteryLevelChar.isValid()) {
                qDebug("Battery level data not found.");
                break;
            }else{
                m_batteryLevelService->readCharacteristic(batteryLevelChar);
                qDebug() << "Current battery level: " << batteryLevelChar.value().toHex();
            }
        }


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

int CaptoGloveAPI::readBatteryLevel()
{
    QLowEnergyCharacteristic batteryLevel;
    batteryLevel = m_batteryLevelService->characteristic(QBluetoothUuid::BatteryLevel);

    m_batteryLevelService->readCharacteristic(batteryLevel);

    int Value;
    Value = QString(batteryLevel.value().toHex()).toUInt(nullptr, 16);;

    return Value;

}

// SCAN PARAMS SERVICE
void CaptoGloveAPI::scanParamsServiceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Discovering scan parameters details in state: " << s ;
    switch(s){
    case QLowEnergyService::DiscoveringServices:
    {
        qDebug() << "Discovering services...";
        break;

    }
    case QLowEnergyService::ServiceDiscovered:
    {
        const QList<QLowEnergyCharacteristic> chars = m_ScanParametersService->characteristics();
        for (const QLowEnergyCharacteristic &ch : chars){
            auto cInfo = new CharacteristicInfo(ch);
            m_characteristics.insert(m_ScanParametersService->serviceUuid(), cInfo);
            qDebug() << "Characteristic uuid is: " << cInfo->getUuid();
            qDebug() << "Characteristic name is: " << cInfo->getName();
        }


        if (!chars.empty())
        {
            const QLowEnergyCharacteristic scanIntervalLevelChar = m_ScanParametersService->characteristic(QBluetoothUuid::ScanIntervalWindow);
            if (!scanIntervalLevelChar.isValid()) {
                qDebug("scan Interval data not found.");
                break;
            }else{
                m_ScanParametersService->readCharacteristic(scanIntervalLevelChar);
                qDebug() << "Current scan parameters: " << scanIntervalLevelChar.value().toHex();
            }
        }


    }
    default:
        // nothing for now
        break;
    }

    emit aliveChanged();

}

// GA SERVICE
void CaptoGloveAPI::genericAccessServiceStateChanged(QLowEnergyService::ServiceState s)
{
    switch(s){
    case QLowEnergyService::DiscoveringServices:
    {
        qDebug() << "Discovering services...";
        break;

    }
    case QLowEnergyService::ServiceDiscovered:
    {
        const QList<QLowEnergyCharacteristic> chars = m_GAService->characteristics();
        for (const QLowEnergyCharacteristic &ch : chars){
            auto cInfo = new CharacteristicInfo(ch);
            m_characteristics.insert(m_GAService->serviceUuid(), cInfo);
            qDebug() << "Characteristic uuid is: " << cInfo->getUuid();
            qDebug() << "Characteristic name is: " << cInfo->getName();
        }


        if (!chars.empty())
        {
            const QLowEnergyCharacteristic m_deviceNameChar = m_GAService->characteristic(QBluetoothUuid::DeviceName);
            const QLowEnergyCharacteristic m_deviceAppereance = m_GAService->characteristic(QBluetoothUuid::Appearance);
            const QLowEnergyCharacteristic m_devicePreferredConnectionParams = m_GAService->characteristic(QBluetoothUuid::PeripheralPreferredConnectionParameters);
            if (!m_deviceNameChar.isValid()) {
                qDebug("Battery level data not found.");
                break;
            }else{
                m_GAService->readCharacteristic(m_deviceNameChar);
                qDebug() << "Device name is: " << m_deviceNameChar.value().data();
            }
        }


    }

}
}

// HID SERVICE STATE CHANGE
void CaptoGloveAPI::HIDserviceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Discovering scan parameters details in state: " << s ;
    switch(s){
    case QLowEnergyService::DiscoveringServices:
    {
        qDebug() << "Discovering HID services...";
        break;

    }
    case QLowEnergyService::ServiceDiscovered:
    {
        QList<QBluetoothUuid> includedServices = m_HIDService->includedServices();
        const QList<QLowEnergyCharacteristic> chars = m_HIDService->characteristics();
        for (const QLowEnergyCharacteristic &ch : chars){
            auto cInfo = new CharacteristicInfo(ch);
            m_characteristics.insert(m_HIDService->serviceUuid(), cInfo);
            qDebug() << "Characteristic uuid is: " << cInfo->getUuid();
            qDebug() << "Characteristic name is: " << cInfo->getName();
        }


        if (!chars.empty())
        {
            qDebug() << "Found HID characteristics!";
            }
        }

    case QLowEnergyService::InvalidService:
    {
        qDebug() << "HID Service is invalid!";
    }
    default:
        // nothing for now
        break;
    }

    //processLoop();
    emit aliveChanged();

}

// FINGER SERVICE CHANGE (check characteristic changes -> F001-F004 UUIDS


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

    if (!m_connected){
        serviceScanDone();
    }

    //while(!m_discoveredServices)
    //{
    //    QThread::sleep(500);

    //}

}

void CaptoGloveAPI::processLoop(){

    while(true)
    {
        QThread::msleep(1000);
        if (m_discoveredServices){

            m_batteryLevelValue = readBatteryLevel();
        }
    }
}

void CaptoGloveAPI::setUpdate(const QString &message)
{
    m_message = message;
    emit updateChanged();
}

void CaptoGloveAPI::discoverServices()
{

    QList<bool> discoveredList;
    ServiceInfo *srvPtr;
        foreach(srvPtr, m_services)
        {
            QLowEnergyService* serv = srvPtr->service();
            QBluetoothUuid uuid = QBluetoothUuid(srvPtr->getUuid());
            if(serv->state() != QLowEnergyService::ServiceDiscovered){
                discoveredList.append(false);
                serv->discoverDetails();
            }else{
                m_controller->createServiceObject(uuid);
                discoveredList.append(true);
            }


        }

        if(!discoveredList.contains(false)){
            m_discoveredServices = false;
            discoverServices();
        }
        else{
            qDebug() << "Discovered all found services!";
            m_discoveredServices = true;
            emit servicesDiscovered();
        }

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

int CaptoGloveAPI::getBatteryLevel()
{

    return m_batteryLevelValue;

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
