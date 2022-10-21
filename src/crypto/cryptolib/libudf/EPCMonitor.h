

#include "EnclaveManager.h"


#include<functional>
#include <atomic>

class EPCMonitor {

using std::atomic;

typedef std::function<void(void)> Task_t;

public:
    static EPCMonitor* getInstance() {
        static EPCMonitor monitor;
        return &monitor;
    }


private:
    EPCMonitor() {

    }
    EPCMonitor(const EPCMonitor &r);

private:
    atomic<bool> m_isSwapping = false;
    atomic<bool> m_isRunning = false;
    Task_t m_doTask;
};