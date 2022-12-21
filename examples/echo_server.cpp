#include "mingo/tcp_server.h"
#include "mingo/log.h"
#include "mingo/iomanager.h"
#include "mingo/bytearray.h"
#include "mingo/address.h"

static mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

class EchoServer : public mingo::TcpServer {
public:
    EchoServer(int type);
    void handleClient(mingo::Socket::ptr client);

private:
    int m_type = 0;
};

EchoServer::EchoServer(int type)
    :m_type(type) {
}

void EchoServer::handleClient(mingo::Socket::ptr client) {
    LOG_INFO(g_logger, "handleClient " + std::to_string(client->getSocket()));   
    mingo::ByteArray::ptr ba(new mingo::ByteArray);
    while(true) {
        ba->clear();
        std::vector<iovec> iovs;
        ba->getWriteBuffers(iovs, 1024);

        int rt = client->recv(&iovs[0], iovs.size());
        if(rt == 0) {
    		LOG_INFO(g_logger, "client close:  " + std::to_string(client->getSocket()));   
            break;
        } else if(rt < 0) {
            LOG_INFO(g_logger, "client error rt=" + std::to_string(rt)
                + " errno=" + std::to_string(errno) + " errstr=" + strerror(errno));
            break;
        }
        ba->setPosition(ba->getPosition() + rt);
        ba->setPosition(0);
        //SYLAR_LOG_INFO(g_logger) << "recv rt=" << rt << " data=" << std::string((char*)iovs[0].iov_base, rt);
        if(m_type == 1) {//text 
            std::cout << ba->toString();// << std::endl;
        } else {
            std::cout << ba->toHexString();// << std::endl;
        }
        std::cout.flush();
    }
}

int type = 1;

void run() {
    LOG_INFO(g_logger, "server type=" + std::to_string(type));
    EchoServer::ptr es(new EchoServer(type));
    auto addr = mingo::Address::LookupAny("0.0.0.0:8020");
    while(!es->bind(addr)) {
        sleep(2);
    }
    es->start();
}

int main(int argc, char** argv) {
    if(argc < 2) {
        LOG_INFO(g_logger, "need and only need argv -t or -b");
        return 0;
    }

    if(!strcmp(argv[1], "-b")) {
        type = 2;
    }

    mingo::IOManager iom(2);
    iom.schedule(run);
    return 0;
}

