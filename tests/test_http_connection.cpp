#include <iostream>
#include "mingo/http/http_connection.h"
#include "mingo/log.h"
#include "mingo/iomanager.h"
#include "mingo/http/http_parser.h"
//#include "mingo/streams/zlib_stream.h"
#include <fstream>

static mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

void test_pool() {
    mingo::http::HttpConnectionPool::ptr pool(new mingo::http::HttpConnectionPool(
                "www.sylar.top", "", 80, false, 10, 1000 * 30, 5));

    mingo::IOManager::GetThis()->addTimer(1000, [pool](){
            auto r = pool->doGet("/", 300);
            LOG_INFO(g_logger, r->toString());
    }, true);
}

void run() {
	/*
    mingo::Address::ptr addr = mingo::Address::LookupAnyIPAddress("www.baidu.com:80");
    if(!addr) {
        LOG_INFO(g_logger, "get addr error");
        return;
    }

    mingo::Socket::ptr sock = mingo::Socket::CreateTCP(addr);
    bool rt = sock->connect(addr);
    if(!rt) {
        LOG_INFO(g_logger, "connect " + addr->toString() + " failed");
        return;
    }

    mingo::http::HttpConnection::ptr conn(new mingo::http::HttpConnection(sock));
    mingo::http::HttpRequest::ptr req(new mingo::http::HttpRequest);
    req->setPath("/");
    req->setHeader("host", "www.baidu.com");
    LOG_INFO(g_logger, "req:\n" + req->toString());

    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if(!rsp) {
        LOG_INFO(g_logger, "recv response error");
        return;
    }
    LOG_INFO(g_logger, "rsp:\n" + rsp->toString());

    //std::ofstream ofs("rsp.dat");
    //ofs << *rsp;

    LOG_INFO(g_logger, "=========================");

    auto r = mingo::http::HttpConnection::DoGet("http://www.baidu.com", 300);
	
    LOG_INFO(g_logger, "result=" + std::to_string(r->result)
        + " error=" + r->error 
        + " rsp=" + (r->response ? r->response->toString() : ""));

    LOG_INFO(g_logger, "=========================");
	*/
    //test_pool();
}

/*
void test_https() {
    auto r = sylar::http::HttpConnection::DoGet("http://www.baidu.com/", 300, {
                        {"Accept-Encoding", "gzip, deflate, br"},
                        {"Connection", "keep-alive"},
                        {"User-Agent", "curl/7.29.0"}
            });
    SYLAR_LOG_INFO(g_logger) << "result=" << r->result
        << " error=" << r->error
        << " rsp=" << (r->response ? r->response->toString() : "");

    //sylar::http::HttpConnectionPool::ptr pool(new sylar::http::HttpConnectionPool(
    //            "www.baidu.com", "", 80, false, 10, 1000 * 30, 5));
    auto pool = sylar::http::HttpConnectionPool::Create(
                    "https://www.baidu.com", "", 10, 1000 * 30, 5);
    sylar::IOManager::GetThis()->addTimer(1000, [pool](){
            auto r = pool->doGet("/", 3000, {
                        {"Accept-Encoding", "gzip, deflate, br"},
                        {"User-Agent", "curl/7.29.0"}
                    });
            SYLAR_LOG_INFO(g_logger) << r->toString();
    }, true);
}

void test_data() {
    sylar::Address::ptr addr = sylar::Address::LookupAny("www.baidu.com:80");
    auto sock = sylar::Socket::CreateTCP(addr);

    sock->connect(addr);
    const char buff[] = "GET / HTTP/1.1\r\n"
                "connection: close\r\n"
                "Accept-Encoding: gzip, deflate, br\r\n"
                "Host: www.baidu.com\r\n\r\n";
    sock->send(buff, sizeof(buff));

    std::string line;
    line.resize(1024);

    std::ofstream ofs("http.dat", std::ios::binary);
    int total = 0;
    int len = 0;
    while((len = sock->recv(&line[0], line.size())) > 0) {
        total += len;
        ofs.write(line.c_str(), len);
    }
    std::cout << "total: " << total << " tellp=" << ofs.tellp() << std::endl;
    ofs.flush();
}

void test_parser() {
    std::ifstream ifs("http.dat", std::ios::binary);
    std::string content;
    std::string line;
    line.resize(1024);

    int total = 0;
    while(!ifs.eof()) {
        ifs.read(&line[0], line.size());
        content.append(&line[0], ifs.gcount());
        total += ifs.gcount();
    }

    std::cout << "length: " << content.size() << " total: " << total << std::endl;
    sylar::http::HttpResponseParser parser;
    size_t nparse = parser.execute(&content[0], content.size(), false);
    std::cout << "finish: " << parser.isFinished() << std::endl;
    content.resize(content.size() - nparse);
    std::cout << "rsp: " << *parser.getData() << std::endl;

    auto& client_parser = parser.getParser();
    std::string body;
    int cl = 0;
    do {
        size_t nparse = parser.execute(&content[0], content.size(), true);
        std::cout << "content_len: " << client_parser.content_len
                  << " left: " << content.size()
                  << std::endl;
        cl += client_parser.content_len;
        content.resize(content.size() - nparse);
        body.append(content.c_str(), client_parser.content_len);
        content = content.substr(client_parser.content_len + 2);
    } while(!client_parser.chunks_done);

    std::cout << "total: " << body.size() << " content:" << cl << std::endl;

    sylar::ZlibStream::ptr stream = sylar::ZlibStream::CreateGzip(false);
    stream->write(body.c_str(), body.size());
    stream->flush();

    body = stream->getResult();

    std::ofstream ofs("http.txt");
    ofs << body;
}
*/

int main(int argc, char** argv) {
    mingo::IOManager iom(2);
    iom.schedule(test_pool);
    //iom.schedule(test_https);
    return 0;
}

