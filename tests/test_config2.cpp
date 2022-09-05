#include "mingo/log.h"
#include "mingo/config.h"
#include "mingo/util.h"
#include <yaml-cpp/yaml.h>

mingo::ConfigVar<int>::ptr init_value_config = 
    mingo::Config::Lookup("system.port", (int)8080, "system port");

mingo::ConfigVar<float>::ptr init_value_config2 = 
    mingo::Config::Lookup("system.value", (float)1.25, "system value");

mingo::ConfigVar<int>::ptr init_value_config4 = 
    mingo::Config::Lookup("system.value", (int)1, "system value");

mingo::ConfigVar<std::vector<int> >::ptr init_value_config3 = 
    mingo::Config::Lookup("system.vec", std::vector<int>{1, 2}, "system vec");

int main(int argc, char *argv[])
{
    mingo::Logger::ptr logger(new mingo::Logger);
    logger->addAppender(mingo::LogAppender::ptr(new mingo::StdoutLogAppender));

    mingo::LogEvent::ptr event(new mingo::LogEvent(__FILE__, __LINE__, mingo::getThreadId(), mingo::getFiberId(), time(0), "before: " + init_value_config->toString()));

    logger->log(event);

    event->setContent("before: " + init_value_config2->toString());

    logger->log(event);

    auto v = init_value_config3->getValue();
    for (auto &i : v) {
        event->setContent("before vec: " + std::to_string(i));
        logger->log(event);
    }

    YAML::Node root = YAML::LoadFile("/home/mingo/workspace/mingo_server_copy_sylar/tests/configure/test.yml");
    mingo::Config::LoadFromYaml(root);

    v = init_value_config3->getValue();
    for (auto &i : v) {
        event->setContent("after vec: " + std::to_string(i));
        logger->log(event);
    }

    event->setContent("after: " + init_value_config->toString());

    logger->log(event);

    event->setContent("after: " + init_value_config2->toString());

    logger->log(event);

    return 0;
}
