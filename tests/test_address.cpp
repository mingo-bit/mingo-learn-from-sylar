#include "mingo/address.h"
#include "mingo/log.h"

mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();

void test() {
    std::vector<mingo::Address::ptr> addrs;

    LOG_INFO(g_logger, "begin");
    //bool v = mingo::Address::Lookup(addrs, "localhost:3080");
    bool v = mingo::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    //bool v = mingo::Address::Lookup(addrs, "www.sylar.top", AF_INET);
    LOG_INFO(g_logger, "end");
    if(!v) {
        LOG_ERROR(g_logger, "lookup fail");
        return;
    }

    for(size_t i = 0; i < addrs.size(); ++i) {
        LOG_INFO(g_logger, std::to_string(i) + " - " + addrs[i]->toString());
    }

    auto addr = mingo::Address::LookupAny("localhost:4080");
    if(addr) {
        LOG_INFO(g_logger, addr->toString());
    } else {
        LOG_ERROR(g_logger, "error");
    }
}

void test_iface() {
    std::multimap<std::string, std::pair<mingo::Address::ptr, uint32_t> > results;

    bool v = mingo::Address::GetInterfaceAddresses(results);
    if(!v) {
        LOG_ERROR(g_logger, "GetInterfaceAddresses fail");
        return;
    }

    for(auto& i: results) {
        LOG_INFO(g_logger,  i.first + " - " + i.second.first->toString() + " - "
            + std::to_string(i.second.second));
    }
}

void test_ipv4() {
    //auto addr = mingo::IPAddress::Create("www.sylar.top");
    auto addr = mingo::IPAddress::Create("127.0.0.8");
    if(addr) {
        LOG_INFO(g_logger, addr->toString());
    }
}

int main(int argc, char** argv) {
    //test_ipv4();
    //test_iface();
    test();
    return 0;
}

