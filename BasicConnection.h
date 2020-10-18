#pragma once

#include "BasicExample.h"


class BasicConnection: public BasicExample {
private:
	int m_propertyChangedSlot;
	PeripheralCentralPtr m_central;
	BoardPeripheralPtr m_peripheral;
public:
	BasicConnection();
	void start();
	void stop();
};
