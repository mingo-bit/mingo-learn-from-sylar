#include "mingo/http/http_server.h"
#include "mingo/log.h"

static mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

void run() {
    g_logger->setLevel(mingo::LogLevel::INFO);
    mingo::http::HttpServer::ptr server(new mingo::http::HttpServer);
    mingo::Address::ptr addr = mingo::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while(!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/mingo/xx", [](mingo::http::HttpRequest::ptr req
                ,mingo::http::HttpResponse::ptr rsp
                ,mingo::http::HttpSession::ptr session) {
            rsp->setBody(req->toString());
            return 0;
    });

    sd->addGlobServlet("/mingo/*", [](mingo::http::HttpRequest::ptr req
                ,mingo::http::HttpResponse::ptr rsp
                ,mingo::http::HttpSession::ptr session) {
            rsp->setBody("Glob:\r\n" + req->toString());
            return 0;
    });

    server->start();
}

int main(int argc, char** argv) {
	mingo::Logger::ptr g_sys_logger = GET_LOGGER("system");
    mingo::IOManager iom(2);
    iom.schedule(run);
    return 0;
}

