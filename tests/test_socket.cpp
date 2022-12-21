#include "mingo/socket.h"
#include "mingo/log.h"
#include "mingo/iomanager.h"

static mingo::Logger::ptr g_looger = GET_GLOBAL_LOGGER();

void test_socket() {
    mingo::IPAddress::ptr addr = mingo::Address::LookupAnyIPAddress("www.baidu.com");
    if(addr) {
        LOG_INFO(g_looger, "get address: " + addr->toString());
    } else {
        LOG_ERROR(g_looger, "get address fail");
        return;
    }

    mingo::Socket::ptr sock = mingo::Socket::CreateTCP(addr);
    addr->setPort(80);
    LOG_INFO(g_looger, "addr=" + addr->toString());
    if(!sock->connect(addr)) {
        LOG_ERROR(g_looger, "connect " + addr->toString() + " fail");
        return;
    } else {
        LOG_INFO(g_looger, "connect " + addr->toString() + " connected");
    }

    const char buff[] = "GET / HTTP/1.0\r\n\r\n";
    int rt = sock->send(buff, sizeof(buff));
    if(rt <= 0) {
        LOG_INFO(g_looger, "send fail rt=" + std::to_string(rt));
        return;
    }

    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());

    if(rt <= 0) {
        LOG_INFO(g_looger, "recv fail rt=" + std::to_string(rt));
        return;
    }

    buffs.resize(rt);
    LOG_INFO(g_looger, buffs);
}


/*
void test2() {
    mingo::IPAddress::ptr addr = mingo::Address::LookupAnyIPAddress("www.baidu.com:80");
    if(addr) {
        LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    mingo::Socket::ptr sock = mingo::Socket::CreateTCP(addr);
    if(!sock->connect(addr)) {
        LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    uint64_t ts = mingo::GetCurrentUS();
    for(size_t i = 0; i < 10000000000ul; ++i) {
        if(int err = sock->getError()) {
            LOG_INFO(g_looger) << "err=" << err << " errstr=" << strerror(err);
            break;
        }

        //struct tcp_info tcp_info;
        //if(!sock->getOption(IPPROTO_TCP, TCP_INFO, tcp_info)) {
        //    mingo_LOG_INFO(g_looger) << "err";
        //    break;
        //}
        //if(tcp_info.tcpi_state != TCP_ESTABLISHED) {
        //    mingo_LOG_INFO(g_looger)
        //            << " state=" << (int)tcp_info.tcpi_state;
        //    break;
        //}
        static int batch = 10000000;
        if(i && (i % batch) == 0) {
            uint64_t ts2 = mingo::GetCurrentUS();
            LOG_INFO(g_looger) << "i=" << i << " used: " << ((ts2 - ts) * 1.0 / batch) << " us";
            ts = ts2;
        }
    }
}
*/

int main(int argc, char** argv) {
    mingo::IOManager iom;
    iom.schedule(&test_socket);
    //iom.schedule(&test2);
    return 0;
}

