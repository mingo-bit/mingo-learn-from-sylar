#include "mingo/daemon.h"
#include "mingo/iomanager.h"
#include "mingo/log.h"

static mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

int server_main(int argc, char** argv) {
    LOG_INFO(g_logger, mingo::ProcessInfoMgr::getInstance()->toString());
    mingo::IOManager iom(1);
    iom.addTimer(1000, [](){
            LOG_INFO(g_logger, "onTimer");
            //static int count = 0;
            //if(++count > 10) {
                //exit(0);
            //}
    }, true);
    return 0;
}

int main(int argc, char** argv) {
    return mingo::start_daemon(argc, argv, server_main, argc != 1);
}

