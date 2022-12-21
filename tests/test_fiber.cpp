#include "mingo/log.h"
#include "mingo/fiber.h"
#include "mingo/thread.h"

mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

void run_in_fiber() {
    LOG_INFO(g_logger, "run_in_fiber begin");
    mingo::Fiber::YieldToHold();
    LOG_INFO(g_logger, "run_in_fiber end");
    mingo::Fiber::YieldToHold();
}

void test_fiber() {
    LOG_INFO(g_logger, "main begin -1");
    {
        mingo::Fiber::GetThis();
        LOG_INFO(g_logger, "main begin");
        mingo::Fiber::ptr fiber(new mingo::Fiber(run_in_fiber));
        fiber->swapIn();
        LOG_INFO(g_logger, "main after swapIn");
        fiber->swapIn();
        LOG_INFO(g_logger, "main after end");
        fiber->swapIn();
    }
    LOG_INFO(g_logger, "main after end2");
}

int main(int argc, char** argv) {
    mingo::Thread::SetName("master");

    std::vector<mingo::Thread::ptr> thrs;
    for(int i = 0; i < 3; ++i) {
        thrs.push_back(mingo::Thread::ptr(
                    new mingo::Thread(&test_fiber, "name_" + std::to_string(i))));
    }
    for(auto i : thrs) {
        i->join();
    }
    return 0;
}
