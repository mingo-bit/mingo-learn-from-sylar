#include <unistd.h>
#include "mingo/log.h"
#include "mingo/thread.h"
#include "mingo/util.h"

mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

int count = 0;
//sylar::RWMutex s_mutex;
mingo::Mutex s_mutex;

void fun1() {
    LOG_INFO(g_logger, "name: " + mingo::Thread::GetName()
                             + " this.name: " + mingo::Thread::GetThis()->getName()
                             + " id: " + std::to_string(mingo::getThreadId())
                             + " this.id: " + std::to_string(mingo::Thread::GetThis()->getId()))
    for(int i = 0; i < 100000; ++i) {
        //sylar::RWMutex::WriteLock lock(s_mutex);
        mingo::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while(true) {
        LOG_INFO(g_logger, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
    }
}

void fun3() {
    while(true) {
        LOG_INFO(g_logger, "========================================");
    }
}

int main(int argc, char** argv) {
    LOG_INFO(g_logger, "thread test begin");

    std::vector<mingo::Thread::ptr> thrs;
    for(int i = 0; i < 1; ++i) {
        mingo::Thread::ptr thr(new mingo::Thread(&fun2, "name_" + std::to_string(i * 2)));
        thrs.push_back(thr);
    }

    for(size_t i = 0; i < thrs.size(); ++i) {
        thrs[i]->join();
    }
    LOG_INFO(g_logger, "thread test end");

    return 0;
}

