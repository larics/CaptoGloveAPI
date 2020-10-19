#ifndef ADAPTERIMPL_LINUX_H
#define ADAPTERIMPL_LINUX_H
#include "GSdkCore.h"
#include "BLE/Adapter.h"


namespace GSdk{
namespace BLE{
namespace Impl{

class AdapterImpl: public Adapter{

public:
    void setScan(bool const&);
    const bool& scan(void) const;
    AdapterImpl();
    ~AdapterImpl();

};
}
}
}

#endif // ADAPTERIMPL_LINUX_H
