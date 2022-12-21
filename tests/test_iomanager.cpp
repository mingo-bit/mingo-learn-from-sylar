#include <iostream>

#include "mingo/iomanager.h"
#include "mingo/log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>

mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

int sock = 0;

void test_fiber() {
    LOG_INFO(g_logger, "test_fiberi sock = " + std::to_string(sock));

    //sleep(3);

    //close(sock);
    //sylar::IOManager::GetThis()->cancelAll(sock);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "10.28.227.12", &addr.sin_addr.s_addr);

    if(!connect(sock, (const sockaddr*)&addr, sizeof(addr))) {
    } else if(errno == EINPROGRESS) {
        LOG_INFO(g_logger,  "add event errno=" + std::to_string(errno));
        mingo::IOManager::GetThis()->addEvent(sock, mingo::IOManager::READ, [](){
            LOG_INFO(g_logger, "read callback");
        });
        mingo::IOManager::GetThis()->addEvent(sock, mingo::IOManager::WRITE, [](){
            LOG_INFO(g_logger, "write callback");
            //close(sock);
            mingo::IOManager::GetThis()->cancelEvent(sock, mingo::IOManager::READ);
            close(sock);
        });
    } else {
        LOG_INFO(g_logger, "else " + std::to_string(errno));
    }

}

void test1() {
    std::cout << "EPOLLIN=" << EPOLLIN
              << " EPOLLOUT=" << EPOLLOUT << std::endl;
    mingo::IOManager iom(2, false);
    iom.schedule(&test_fiber);
}

mingo::Timer::ptr s_timer;
void test_timer() {
    mingo::IOManager iom(2);
    s_timer = iom.addTimer(1000, [](){
        static int i = 0;
        LOG_INFO(g_logger, "hello timer i=" + std::to_string(i));
        if(++i == 3) {
            s_timer->reset(2000, true);
            //s_timer->cancel();
        }
    }, true);
}

int main(int argc, char** argv) {
    test1();
    //test_timer();
    return 0;
}

