#include "mingo/http/http_parser.h"
#include "mingo/log.h"

static mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

const char test_request_data[] = "POST / HTTP/1.1\r\n"
                                "Host: www.baidu.com\r\n"
                                "Content-Length: 10\r\n\r\n"
                                "1234567890";

void test_request() {
    mingo::http::HttpRequestParser parser;
    std::string tmp = test_request_data;
    size_t s = parser.execute(&tmp[0], tmp.size());
    LOG_ERROR(g_logger, "execute rt=" + std::to_string(s)
        + "has_error=" + std::to_string(parser.hasError())
        + " is_finished=" + std::to_string(parser.isFinished())
        + " total=" + std::to_string(tmp.size())
        + " content_length=" + std::to_string(parser.getContentLength()));
    tmp.resize(tmp.size() - s);
    LOG_INFO(g_logger, parser.getData()->toString());
    LOG_INFO(g_logger, tmp);
}

const char test_response_data[] = "HTTP/1.1 200 OK\r\n"
        "Date: Tue, 04 Jun 2019 15:43:56 GMT\r\n"
        "Server: Apache\r\n"
        "Last-Modified: Tue, 12 Jan 2010 13:48:00 GMT\r\n"
        "ETag: \"51-47cf7e6ee8400\"\r\n"
        "Accept-Ranges: bytes\r\n"
        "Content-Length: 81\r\n"
        "Cache-Control: max-age=86400\r\n"
        "Expires: Wed, 05 Jun 2019 15:43:56 GMT\r\n"
        "Connection: Close\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html>\r\n"
        "<meta http-equiv=\"refresh\" content=\"0;url=http://www.baidu.com/\">\r\n"
        "</html>\r\n";

void test_response() {
    mingo::http::HttpResponseParser parser;
    std::string tmp = test_response_data;
    size_t s = parser.execute(&tmp[0], tmp.size(), true);
    LOG_ERROR(g_logger, "execute rt=" + std::to_string(s)
        + " has_error=" + std::to_string(parser.hasError())
        + " is_finished=" + std::to_string(parser.isFinished())
        + " total=" + std::to_string(tmp.size())
        + " content_length=" + std::to_string(parser.getContentLength())
        + " tmp[s]=" + tmp[s]);

    tmp.resize(tmp.size() - s);

    LOG_INFO(g_logger, parser.getData()->toString());
    LOG_INFO(g_logger, tmp);
}

int main(int argc, char** argv) {
    test_request();
    LOG_INFO(g_logger, "--------------");
    test_response();
    return 0;
}

