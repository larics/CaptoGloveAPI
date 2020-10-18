#include "BasicConnection.h"

BasicConnection::BasicConnection(): BasicExample("BasicConnection") {
	// NOP
}

void BasicConnection::start() {
	m_central = makeBoardAdapterCentral();
	m_central->scanChanged().connect([this](const CentralScanEventArgs &args) {
		this->printInfo("Central status changed: " + to_string(args.scan()));
	});
	m_central->peripheralsChanged().connect([this](const CentralPeripheralsEventArgs &args) {
		auto peripherals = args.inserted();
		for (auto peripheral : peripherals) {
			auto board = std::dynamic_pointer_cast<BoardPeripheral>(peripheral);
			this->printInfo("Trying to connect peripheral");
			this->printInfo("- ID: " + board->id());
			this->printInfo("- Name: " + board->name());
			this->m_peripheral = board;
			this->m_propertyChangedSlot = this->m_peripheral->propertyChanged().connect(
				[this](const PeripheralPropertyEventArgs &args) {
				if (args.name == PeripheralProperty::status) {
					this->printInfo("- Status changed " + to_string(this->m_peripheral->status()));
				}
			});
			if (board->start()) {
				this->printInfo("- Status: " + to_string(peripheral->status()));
				this->m_peripheral->emulationState().write(true);
			}
			else {
				this->printError("- Failed to connect, waiting for autoreconnect ");
			}
		}
	});
	m_central->startScan(GSdkScanOptionsMake(5));
}

void BasicConnection::stop() {
	m_central->stopScan();
	if (m_peripheral) {
		m_peripheral->propertyChanged().disconnect(m_propertyChangedSlot);
		m_peripheral->stop();
	}
}
