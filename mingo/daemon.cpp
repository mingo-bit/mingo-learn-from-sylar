#include "daemon.h"
#include "mingo/log.h"
#include "mingo/config.h"
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace mingo {

static mingo::Logger::ptr g_daemon_logger = GET_LOGGER("system");
static mingo::ConfigVar<uint32_t>::ptr g_daemon_restart_interval
    = mingo::Config::Lookup("daemon.restart_interval", (uint32_t)5, "daemon restart interval");

std::string ProcessInfo::toString() const {
    std::stringstream ss;
    ss << "[ProcessInfo parent_id=" << parent_id
       << " main_id=" << main_id
       << " parent_start_time=" << mingo::Time2Str(parent_start_time)
       << " main_start_time=" << mingo::Time2Str(main_start_time)
       << " restart_count=" << restart_count << "]";
    return ss.str();
}

static int real_start(int argc, char** argv,
                     std::function<int(int argc, char** argv)> main_cb) {
    ProcessInfoMgr::getInstance()->main_id = getpid();
    ProcessInfoMgr::getInstance()->main_start_time = time(0);
    return main_cb(argc, argv);
}

static int real_daemon(int argc, char** argv,
                     std::function<int(int argc, char** argv)> main_cb) {
    //daemon(1, 0);
    ProcessInfoMgr::getInstance()->parent_id = getpid();
    ProcessInfoMgr::getInstance()->parent_start_time = time(0);
    while(true) {
        pid_t pid = fork();
        if(pid == 0) {
            //子进程返回
            ProcessInfoMgr::getInstance()->main_id = getpid();
            ProcessInfoMgr::getInstance()->main_start_time  = time(0);
            LOG_INFO(g_daemon_logger, "process start pid=" + std::to_string(getpid()));
            return real_start(argc, argv, main_cb);
        } else if(pid < 0) {
            LOG_ERROR(g_daemon_logger, "fork fail return=" + std::to_string(pid)
                + " errno=" + std::to_string(errno) + " errstr=" + strerror(errno));
            return -1;
        } else {
            //父进程返回
            int status = 0;
            waitpid(pid, &status, 0);
            if(status) {
                if(status == 9) {
                    LOG_INFO(g_daemon_logger, "killed");
                    break;
                } else {
                    LOG_ERROR(g_daemon_logger, "child crash pid=" + std::to_string(pid)
                        + " status=" + std::to_string(status));
                }
            } else {
                LOG_INFO(g_daemon_logger, "child finished pid=" + std::to_string(pid));
                break;
            }
            ProcessInfoMgr::getInstance()->restart_count += 1;
            sleep(g_daemon_restart_interval->getValue());
        }
    }
    return 0;
}

int start_daemon(int argc, char** argv
                 , std::function<int(int argc, char** argv)> main_cb
                 , bool is_daemon) {
    if(!is_daemon) {
        ProcessInfoMgr::getInstance()->parent_id = getpid();
        ProcessInfoMgr::getInstance()->parent_start_time = time(0);
        return real_start(argc, argv, main_cb);
    }
    return real_daemon(argc, argv, main_cb);
}

}

