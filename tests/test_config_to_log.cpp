#include<iostream>
#include "mingo/log.h"
#include "mingo/config.h"

void test_log() {
    static mingo::Logger::ptr system_log = GET_LOGGER("system");
    LOG_INFO(system_log, "hello system");
    std::cout << mingo::LoggerMgr::getInstance()->toYamlString() << std::endl;
    YAML::Node root = YAML::LoadFile("/home/mingo/workspace/mingo_server_copy_sylar/tests/configure/log.yml");
    mingo::Config::LoadFromYaml(root);
    std::cout << "=============" << std::endl;
    std::cout << mingo::LoggerMgr::getInstance()->toYamlString() << std::endl;
    std::cout << "=============" << std::endl;
    std::cout << root << std::endl;
    LOG_INFO(system_log, "hello system");
}

int main(int argc, char *argv[]) {
    test_log();
    return 0;
}


