#include "mingo/hook.h"
#include "mingo/log.h"
#include "mingo/util.h"
#include "mingo/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

void test_sleep() {
    mingo::IOManager iom(1);
    iom.schedule([](){
        sleep(2);
        LOG_INFO(g_logger, "sleep 2");
    });

    iom.schedule([](){
        sleep(3);
        LOG_INFO(g_logger, "sleep 3");
    });
    LOG_INFO(g_logger, "test_sleep");
}

void test_sock() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "39.156.66.14", &addr.sin_addr.s_addr);

    LOG_INFO(g_logger, "begin connect");
    int rt = connect(sock, (const sockaddr*)&addr, sizeof(addr));
    LOG_INFO(g_logger, "connect rt=" + std::to_string(rt) + " errno=" + std::to_string(errno));

    if(rt) {
        return;
    }

    const char data[] = "GET / HTTP/1.0\r\n\r\n";
    rt = send(sock, data, sizeof(data), 0);
    LOG_INFO(g_logger, "send rt=" + std::to_string(rt) + " errno=" + std::to_string(errno));

    if(rt <= 0) {
        return;
    }

    std::string buff;
    buff.resize(4096);

    rt = recv(sock, &buff[0], buff.size(), 0);
    LOG_INFO(g_logger, "recv rt=" + std::to_string(rt) + " errno=" + std::to_string(errno));

    if(rt <= 0) {
        return;
    }

    buff.resize(rt);
    LOG_INFO(g_logger, buff);
}

int main(int argc, char** argv) {
    //test_sleep();
    mingo::IOManager iom;
    iom.schedule(test_sock);
    return 0;
}
