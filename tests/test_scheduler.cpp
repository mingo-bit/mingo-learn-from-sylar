#include "mingo/log.h"
#include "mingo/scheduler.h"

static mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

 // 演示协程睡眠对主程序的影响
void test_fiber2() {
    static int s_count = 5;
    LOG_INFO(g_logger, "test_fiber2 s_count = " + std::to_string(s_count));

    sleep(1);
    if (--s_count >= 0)
        mingo::Scheduler::GetThis()->schedule(&test_fiber2, mingo::getThreadId());

}

 // 演示指定执行线程的情况
void test_fiber4() {
    LOG_INFO(g_logger, "test_fiber4 begin");
    
    for (int i = 0; i < 3; i++) {
        mingo::Scheduler::GetThis()->schedule(test_fiber2, mingo::getThreadId());
    }

    LOG_INFO(g_logger, "test_fiber4 end");
}

int main() {
    LOG_INFO(g_logger, "main begin");

    mingo::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    LOG_INFO(g_logger, "schedule");
    sc.schedule(&test_fiber2);
    sc.stop();

    LOG_INFO(g_logger, "main end");
    return 0;
}

