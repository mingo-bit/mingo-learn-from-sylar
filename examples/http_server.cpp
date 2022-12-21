#include "mingo/http/http_server.h"
#include "mingo/log.h"

mingo::Logger::ptr g_logger = GET_LOGGER("system");
mingo::IOManager::ptr worker;
void run() {
    g_logger->setLevel(mingo::LogLevel::INFO);
    mingo::Address::ptr addr = mingo::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr) {
        LOG_ERROR(g_logger, "get address error");
        return;
    }

    mingo::http::HttpServer::ptr http_server(new mingo::http::HttpServer(true, worker.get()));
    //mingo::http::HttpServer::ptr http_server(new mingo::http::HttpServer(true));
    while(!http_server->bind(addr)) {
        LOG_ERROR(g_logger, "bind " + addr->toString() + " fail");
        sleep(1);
    }

    http_server->start();
}

int main(int argc, char** argv) {
    mingo::IOManager iom(1);
    worker.reset(new mingo::IOManager(2, false));
    iom.schedule(run);
    return 0;
}

