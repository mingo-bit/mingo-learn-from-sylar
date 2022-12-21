#include "mingo/tcp_server.h"
#include "mingo/iomanager.h"
#include "mingo/log.h"

mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

void run() {
    auto addr = mingo::Address::LookupAny("0.0.0.0:8033");
    //auto addr2 = sylar::UnixAddress::ptr(new sylar::UnixAddress("/tmp/unix_addr"));
    std::vector<mingo::Address::ptr> addrs;
    addrs.push_back(addr);
    //addrs.push_back(addr2);

    mingo::TcpServer::ptr tcp_server(new mingo::TcpServer);
    std::vector<mingo::Address::ptr> fails;
    while(!tcp_server->bind(addrs, fails)) {
        sleep(2);
    }
    tcp_server->start();
    
}
int main(int argc, char** argv) {
    mingo::IOManager iom(2);
    iom.schedule(run);
    return 0;
}

