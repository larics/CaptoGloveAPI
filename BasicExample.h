#pragma once

#include <string>

#include "GSdkCore.h"

using namespace std;
using namespace GSdk;
using namespace Board;
using namespace BoardTools;
using namespace Peripheral;

class BasicExample {
private:
	std::string m_name;

public:
	virtual void start() = 0;
	virtual void stop() = 0;

	BasicExample(const std::string &name);
	std::string name() const;
	void printInfo(const std::string &message) const;
	void printWarning(const std::string &message) const;
	void printError(const std::string &message) const;
	void waitForInput() const;
};
