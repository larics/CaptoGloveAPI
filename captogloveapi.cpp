#include "captogloveapi.h"

// ############# INIT ###############
CaptoGloveAPI::CaptoGloveAPI(QObject *parent,  QString configPath) : QObject(parent), m_configPath(configPath)
{

    m_connected = false;
    m_deviceState = DeviceNotFound;

    // Load or create default config file
    QFile configFile(m_configPath);
    if (m_configPath == "") m_configPath = tr("%1%2").arg(PROJECT_PATH).arg("config.ini");

    m_config = new QSettings(m_configPath, QSettings::IniFormat);
    m_config->beginGroup("InitialSetup");
    loadSettings(m_config);
    m_config->endGroup();

    // initialize Bluetooth Discovery agent
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    m_discoveryAgent->setLowEnergyDiscoveryTimeout(m_scanTimeoutMs);

    // Signal that reports found device
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(addDevice(QBluetoothDeviceInfo))); // Same  as 3rd one
    connect(m_discoveryAgent, static_cast<void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)>(&QBluetoothDeviceDiscoveryAgent::error),
                                               this, &CaptoGloveAPI::deviceScanError);
    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(startConnection()));

    // Set up local to check states and connectivity often
    localDevice = new QBluetoothLocalDevice();
    localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);

    connect(this,SIGNAL(servicesDiscovered()), this, SLOT(serviceScanDone()));

    connect(this, SIGNAL(initialized()), this, SLOT(processLoop()));

    //connect(this, SIGNAL(diagSignal()), this, SLOT(diagSlot()));

    qRegisterMetaType<CaptoGloveAPI::DeviceState>("CaptoGloveAPI::DeviceState");
    connect(this, SIGNAL(setDeviceStateSignal(CaptoGloveAPI::DeviceState)),
            this, SLOT(setDeviceStateSlot(CaptoGloveAPI::DeviceState)));

    // Update corresponding protobuffer msgs
    // connect(this, SIGNAL(updateFingerState()), this, SLOT(setFingerMsg()));
    // connect(this, SIGNAL(updateBatteryState()), this, SLOT(setBatteryMsg()));

    // Initialize logger
    Logger::instance() -> start();
    m_config->beginGroup("Logger");
    Logger::instance() -> loadSettings(m_config);
    m_config->endGroup();



}

CaptoGloveAPI::~CaptoGloveAPI() {
}

void CaptoGloveAPI::loadSettings(QSettings* Setting)
{

    m_deviceName    = Setting->value("DeviceName",      m_deviceName).toString();
    m_deviceAddress = Setting->value("DeviceAddress",   m_deviceAddress).toString();
    m_scanTimeoutMs = Setting->value("ScanTimeoutMs",   m_scanTimeoutMs).toInt();
    m_randomAdress  = Setting->value("RandomAddress",   m_randomAdress).toBool();

    qDebug() << "Loaded m_deviceName" << m_deviceName;
    qDebug() << "Loaded m_deviceAddress" << m_deviceAddress;
    qDebug() << "Loaded m_scanTimeoutMs" << m_scanTimeoutMs;
    qDebug() << "Loaded m_radnomAddress" << m_randomAdress;
}

void CaptoGloveAPI::saveSettings(QSettings* Setting)
{

    Setting->setValue("DeviceName",      m_deviceName);
    Setting->setValue("DeviceAddress",   m_deviceAddress);
    Setting->setValue("ScanTimeoutMs",   m_scanTimeoutMs);
    Setting->setValue("RandomAddress",   m_randomAdress);

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
    }else{
        Logger::instance() -> sendMessageToLog(QString(tr("Discovery agent failed to start: %1").arg(m_discoveryAgent->errorString())));
        //qDebug() << "Discovery agent failed to start:" << m_discoveryAgent->errorString();
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

void CaptoGloveAPI::addDevice(const QBluetoothDeviceInfo &device){


    // DeviceInfo foundDevice;
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration){
        m_devices.append(new DeviceInfo(device));

        qDebug() << "Found device: " << device.name();

        if (device.address().toString() == m_deviceAddress){
            emit setDeviceStateSignal(DeviceFound);
        }
        // TODO: Add check if device is found?

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
    Logger::instance()->sendMessageToLog("Device connected. Scanning services.");
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
        m_FingerPositionsService = m_controller->createServiceObject(QBluetoothUuid(QString("0000ff05-3333-acda-0000-ff522ee73921")));
        connect(m_FingerPositionsService, &QLowEnergyService::stateChanged, this,  &CaptoGloveAPI::fingerPoseServiceStateChanged);
        connect(m_FingerPositionsService, &QLowEnergyService::characteristicChanged, this, &CaptoGloveAPI::fingerPoseCharacteristicChanged);
        connect(m_FingerPositionsService, &QLowEnergyService::descriptorWritten, this, &CaptoGloveAPI::confirmedDescriptorWrite);
    }
    if (m_FingerPositionsService)
    {
        m_FingerPositionsService->discoverDetails();

        m_connected = true;
        m_initialized = true;
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

    else if (uuid == QBluetoothUuid::ScanParameters && !m_foundScanParametersService){

        qDebug() << "Discovered ScanParameters Service!";
        m_foundScanParametersService = true;
    }

    else if (uuid == QBluetoothUuid::HumanInterfaceDevice && !m_foundHIDService){

        qDebug() << "Discovered HID Service";
        m_foundHIDService = true;
    }

    else if (uuid == QBluetoothUuid::DeviceInformation && !m_foundDeviceInfoService)
    {
        qDebug() << "Found Device information service.";
        m_foundDeviceInfoService = true;
    }

    else if (uuid == QBluetoothUuid(QString("0000ff05-3333-acda-0000-ff522ee73921")) && !m_foundFingerPositionService)  // Check how to use custom service!
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
            const QLowEnergyCharacteristic scanIntervalChar = m_ScanParametersService->characteristic(QBluetoothUuid::ScanIntervalWindow);
            const QLowEnergyCharacteristic scanRefreshChar = m_ScanParametersService->characteristic(QBluetoothUuid::ScanRefresh);
            if (!scanRefreshChar.isValid() || !scanIntervalChar.isValid()) {
                qDebug("scan Interval data not found.");
                break;
            }else{
                m_ScanParametersService->readCharacteristic(scanIntervalChar);
                m_ScanParametersService->readCharacteristic(scanRefreshChar);

            }
        }
        break;


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
    // TODO: Check this switch!
    switch(s)
    {
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
                m_deviceName = m_deviceNameChar.value().data();
                qDebug() << "Device name is: " << m_deviceNameChar.value().data();
            }
        }
        break;


    }

    }
}

// HID SERVICE STATE CHANGE
void CaptoGloveAPI::HIDserviceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Discovering HID details in state: " << s ;
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
        break;
    }
    default:
        // nothing for now
        break;
    }

    //processLoop();
    emit aliveChanged();

}

// FINGER SERVICE CHANGE (check characteristic changes -> F001-F004 UUIDS
void CaptoGloveAPI::fingerPoseServiceStateChanged(QLowEnergyService::ServiceState s){
    switch(s){
    case QLowEnergyService::DiscoveringServices:
    {
        qDebug() << "Discovering services...";
        break;
    }
    case QLowEnergyService::ServiceDiscovered:
    {
        const QList<QLowEnergyCharacteristic> chars = m_FingerPositionsService->characteristics();
        for (const QLowEnergyCharacteristic &ch : chars){
            auto cInfo = new CharacteristicInfo(ch);
            //m_characteristics.insert(m_Service->serviceUuid(), cInfo);
            m_FingerPositionsService->characteristic(QBluetoothUuid(cInfo->getUuid()));
            qDebug() << "Characteristic uuid is: " << cInfo->getUuid();
            qDebug() << "Characteristic name is: " << cInfo->getName();
        }
        if (!chars.empty())
        {
            // NOT Neccessary
            QLowEnergyCharacteristic m_fingerReq = m_FingerPositionsService->characteristic(QBluetoothUuid(QString("0000f001-3333-acda-0000-ff522ee73921")));
            QLowEnergyCharacteristic m_fingerFirst = m_FingerPositionsService->characteristic(QBluetoothUuid(QString("000f002-3333-acda-0000-ff522ee73921")));
            QLowEnergyCharacteristic m_fingerSecond = m_FingerPositionsService->characteristic(QBluetoothUuid(QString("000f003-333-acda-0000-ff522ee73921")));
            QLowEnergyCharacteristic m_fingerThird = m_FingerPositionsService->characteristic(QBluetoothUuid(QString("000f004-333-acda-0000-ff522ee73921")));

            QLowEnergyDescriptor desc = m_fingerSecond.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
            m_FingerPositionsService->writeDescriptor(desc, QByteArray::fromHex("0100"));

            emit initialized();
        }
        break;
    }
    case QLowEnergyService::InvalidService:
    {
        qDebug() << "Finger service is invalid! ";
        break;
    }
    default:
        qDebug() << "Default in switch";


}

}

void CaptoGloveAPI::fingerPoseCharacteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value){


    // Set current finger value
    m_currentFingerPosition = value;

    int dataLength = value.size();

    QVector<quint8> result;
    for (int i = 0; i < dataLength; ++i )
    {

        QByteArray temp;
        temp.append(m_currentFingerPosition.at(i));
        quint8 currentValue = *reinterpret_cast<const quint8 *>(temp.constData());
        result.append(currentValue);
    }

    emit updateFingerState(setFingerMsg(result));

}

void CaptoGloveAPI::confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value){

    qDebug() << "Written descriptor!!!";
}

captoglove_v1::FingerFeedbackMsg CaptoGloveAPI::setFingerMsg(QVector<quint8> fingerVector)
{
    quint8 thumb, index, middle, ring, little;

    thumb = convertToPercentage(fingerVector.at(16));
    index = convertToPercentage(fingerVector.at(13));
    middle = convertToPercentage(fingerVector.at(10));
    ring = convertToPercentage(fingerVector.at(6));
    little = convertToPercentage(fingerVector.at(3));

    // TODO: transform to valid datta format currentFingerPosition
    m_fingerFeedbackMsg.set_thumb_finger(thumb);
    m_fingerFeedbackMsg.set_index_finger(index);
    m_fingerFeedbackMsg.set_middle_finger(middle);
    m_fingerFeedbackMsg.set_ring_finger(ring);
    m_fingerFeedbackMsg.set_little_finger(little);

    qDebug() << "THUMB: " << thumb << "\t INDEX: " << index << "\t MIDDLE: " << middle << "\t RING: " << ring << "\t PINKY: " << little ;

    return m_fingerFeedbackMsg;

}

captoglove_v1::BatteryLevelMsg CaptoGloveAPI::setBatteryMsg()
{
    m_batteryMsg.set_level(m_batteryLevelValue);

    return m_batteryMsg;
}

// ############## FUNCTIONAL ##############
void CaptoGloveAPI::startConnection(){


    DeviceInfo *m_devicePtr;
    // TODO: Think of stopping if haven't discovered / connected to wanted device

    if (m_deviceState == DeviceFound){

        foreach(m_devicePtr, m_devices)
        {
            qDebug() << QString("Current device: %1").arg(m_devicePtr->getName());

            if (m_devicePtr->getName().contains(m_deviceName))
            {
                m_peripheralDevice.setDevice(m_devicePtr->getDevice());
                break;
            }
        }
    }

    else{
        emit setDeviceStateSignal(DeviceNotFound);
    }

    // Start Service discovery
    scanServices(m_peripheralDevice); // TODO: Maybe break controller initialization and scanning for services in two method calls

    if (!m_connected){
        serviceScanDone();
    }




}

void CaptoGloveAPI::run(){
    std::cout << "2" << std::endl;

    qDebug() << "Starting device discovery";
    startDeviceDiscovery();
}

void CaptoGloveAPI::processLoop(){

    qDebug() << "Entered process loop!";
    QThread::msleep(1000);
    qDebug() << getFingers();
}

bool CaptoGloveAPI::getFingers()
{
    QLowEnergyCharacteristic m_fingerZero =  m_FingerPositionsService->characteristic(QBluetoothUuid(QString("0000f001-3333-acda-0000-ff522ee73921")));
    QLowEnergyCharacteristic m_fingerFirst = m_FingerPositionsService->characteristic(QBluetoothUuid(QString("0000f002-3333-acda-0000-ff522ee73921")));
    QLowEnergyCharacteristic m_fingerSecond = m_FingerPositionsService->characteristic(QBluetoothUuid(QString("0000f003-3333-acda-0000-ff522ee73921")));
    QLowEnergyCharacteristic m_fingerThird = m_FingerPositionsService->characteristic(QBluetoothUuid(QString("0000f004-3333-acda-0000-ff522ee73921")));

    m_FingerPositionsService->writeCharacteristic(m_fingerZero, QByteArray("83"));
    m_FingerPositionsService->readCharacteristic(m_fingerFirst);
    m_FingerPositionsService->readCharacteristic(m_fingerSecond); // DOES NOT CHANGE?!
    m_FingerPositionsService->readCharacteristic(m_fingerThird);

    QLowEnergyDescriptor desc = m_fingerSecond.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
    m_FingerPositionsService->writeDescriptor(desc, QByteArray::fromHex("0100"));

    // Find out properties
    qDebug() << "Properties for zero:" << m_fingerZero.properties();
    qDebug() << "Properties for first: " << m_fingerFirst.properties();
    qDebug() << "Properties for second:" << m_fingerSecond.properties();
    qDebug() << "Properties for third:" << m_fingerThird.properties();

    qDebug() << "m_fingerFirst" << m_fingerFirst.value();
    qDebug() << "m_fingerSecond" << m_fingerSecond.value();
    qDebug() << "m_fingerThird" << m_fingerThird.value();

    return m_foundFingerPositionService;
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

QByteArray CaptoGloveAPI::getCurrentFingerPosition()
{
    return m_currentFingerPosition;
}

QString CaptoGloveAPI::getDeviceName()
{
    return m_deviceName;
}

CaptoGloveAPI::DeviceState CaptoGloveAPI::getDeviceState()
{
    return m_deviceState;
}

void CaptoGloveAPI::setDeviceStateSlot(DeviceState state)
{
    m_deviceState = state;
}

bool CaptoGloveAPI::getInit()
{
    return m_initialized;
}

bool CaptoGloveAPI::isRandomAddress() const
{
    return m_randomAdress;
}

bool CaptoGloveAPI::alive() const
{
    if (m_batteryLevelService)
        return m_batteryLevelService->state() == QLowEnergyService::ServiceDiscovered;

    return false;
}

quint8 CaptoGloveAPI::convertToPercentage(quint8 value)
{
    float temp;
    temp = static_cast<float>(value)/255 * 100;

    quint8 percentage = static_cast<quint8>(temp);

    return percentage;
}
