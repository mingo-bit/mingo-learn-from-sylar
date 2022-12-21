#include "http_server.h"
#include "mingo/log.h"

namespace mingo {
namespace http {

static mingo::Logger::ptr g_http_server_logger = GET_LOGGER("system");

HttpServer::HttpServer(bool keepalive
               ,mingo::IOManager* worker
               ,mingo::IOManager* io_worker
               ,mingo::IOManager* accept_worker)
    :TcpServer(worker, io_worker, accept_worker)
    ,m_isKeepalive(keepalive) {
    m_dispatch.reset(new ServletDispatch);

    m_type = "http";
    //m_dispatch->addServlet("/_/status", Servlet::ptr(new StatusServlet));
    //m_dispatch->addServlet("/_/config", Servlet::ptr(new ConfigServlet));
}

void HttpServer::setName(const std::string& v) {
    TcpServer::setName(v);
    m_dispatch->setDefault(std::make_shared<NotFoundServlet>(v));
}

void HttpServer::handleClient(Socket::ptr client) {
    LOG_DEBUG(g_http_server_logger, "handleClient " + std::to_string(client->getSocket()));
    HttpSession::ptr session(new HttpSession(client));
    do {
        auto req = session->recvRequest();
        if(!req) {
            LOG_DEBUG(g_http_server_logger, "recv http request fail, errno="
                + std::to_string(errno) + " errstr=" + strerror(errno)
                + " cliet:" + std::to_string(client->getSocket()) + " keep_alive=" + std::to_string(m_isKeepalive));
            break;
        }

        HttpResponse::ptr rsp(new HttpResponse(req->getVersion()
                            ,req->isClose() || !m_isKeepalive));
        rsp->setHeader("Server", getName());
        m_dispatch->handle(req, rsp, session);
        session->sendResponse(rsp);
        if(!m_isKeepalive || req->isClose()) {
			std::cout << "Connection: " << m_isKeepalive << std::endl;
            break;
        }
    } while(true);
    session->close();
}

}
}

