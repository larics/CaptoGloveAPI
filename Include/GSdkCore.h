#pragma once

#include "Board/BoardMotionLink.h"
#include "Board/BoardDescriptor.h"
#include "Board/CalibrationConfig.h"
#include "Board/FirmwareVersion.h"
#include "Board/MotionControllerConfig.h"
#include "Board/EmulationModes.h"
#include "Board/KeyboardHidKey.h"
#include "Board/KeyTrigger.h"
#include "Board/SensorDescriptor.h"
#include "Board/StreamTimeslots.h"
#include "Board/BoardAdapterPeripheralSource.h"
#include "Board/BoardEmulatorPeripheralSource.h"
#include "Board/BoardColor.h"
#include "Board/BoardPeripheral.h"
#include "Board/BoardStream.h"
#include "Board/BoardTypes.h"
#include "Board/AccelAxisInfo.h"
#include "Board/ImuConfig.h"
#include "Board/InputAxisInfo.h"

#include "BoardTools/BoardOrientation.h"
#include "BoardTools/WearingPosition.h"
#include "BoardTools/ExternalSensor.h"
#include "BoardTools/ExternalSensorAssembly.h"
#include "BoardTools/BoardUserInfo.h"
#include "BoardTools/BoardUserInfoCoder.h"

#include "Base/AssemblyInfo.h"
#include "Base/AssemblyTypes.h"
#include "Base/Event.h"
#include "Base/Logger.h"
#include "Base/Result.h"
#include "Base/VectorTypes.h"
#include "Base/Vector3.h"
#include "Base/Quaternion.h"
#include "Base/Matrix3.h"
#include "Base/VectorArithmetic.h"
 
#include "BLE/Adapter.h"
#include "BLE/Characteristic.h"
#include "BLE/Descriptor.h"
#include "BLE/DescriptorValue.h"
#include "BLE/Device.h"
#include "BLE/Guid.h"
#include "BLE/Service.h"
#include "BLE/Types.h"

#include "Peripheral/AdapterCentral.h"
#include "Peripheral/EmulatorCentral.h"
#include "Peripheral/PeripheralBase.h"
#include "Peripheral/PeripheralCentral.h"
#include "Peripheral/PeripheralRegister.h"

#include "Tatto/TattoPeripheral.h"
#include "Tatto/TattoAdapterPeripheralSource.h"
