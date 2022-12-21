#include "mingo/http/http.h"
#include "mingo/log.h"

void test_request() {
    mingo::http::HttpRequest::ptr req(new mingo::http::HttpRequest);
    req->setHeader("host" , "www.baidu.com");
    req->setBody("hello baidu");
    req->dump(std::cout) << std::endl;
}

void test_response() {
    mingo::http::HttpResponse::ptr rsp(new mingo::http::HttpResponse);
    rsp->setHeader("X-X", "sylar");
    rsp->setBody("hello baidu");
    rsp->setStatus((mingo::http::HttpStatus)400);
    rsp->setClose(false);

    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char** argv) {
    test_request();
    test_response();
    return 0;
}

