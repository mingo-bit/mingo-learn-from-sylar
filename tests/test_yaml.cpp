#include "mingo/config.h"
#include "mingo/log.h"
#include "mingo/util.h"
#include <yaml-cpp/yaml.h>

void print_yaml(mingo::Logger::ptr logger, const YAML::Node& node, int level) {

    mingo::LogEvent::ptr event(new mingo::LogEvent(__FILE__, __LINE__, mingo::getThreadId(), mingo::getFiberId(), time(0), "HHH", mingo::LogLevel::INFO));
    if (node.IsScalar()) {
        event->setContent(std::string(level * 4, ' ') + node.Scalar() + " - " + std::to_string(node.Type()) + " - " + std::to_string(level));
        logger->log(event);
    } else if (node.IsNull()) {
        event->setContent(std::string(level * 4, ' ') + "NULL - " + std::to_string(node.Type()) + " - " + std::to_string(level));
        logger->log(event);
    } else if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            event->setContent(std::string(level *4, ' ') + it->first.Scalar() + " - " + std::to_string(it->second.Type()) + " - " + std::to_string(level));
            logger->log(event);
            print_yaml(logger, it->second, level + 1);
        }
    } else if (node.IsSequence()) {
        for (size_t i = 0; i < node.size(); i++) {
            event->setContent(std::string(level * 4, ' ') + std::to_string(i) + " - " + std::to_string(node[i].Type()) + " - " + std::to_string(level));
            logger->log(event);
            print_yaml(logger, node[i], level + 1);
        }
    }

}

void test_yaml(mingo::Logger::ptr logger)
{
    YAML::Node root = YAML::LoadFile("/home/mingo/workspace/mingo_server_copy_sylar/tests/configure/log.yml");
    print_yaml(logger, root, 0);
}

int main(int argc, char *argv[])
{
    mingo::Logger::ptr logger(new mingo::Logger);
    logger->addAppender(mingo::LogAppender::ptr(new mingo::StdoutLogAppender));

    mingo::LogFormatter::ptr fmtr = mingo::LogFormatter::ptr(new mingo::LogFormatter("[%d]  [T:%t F:%F]  [%c]  [%p]  <%f: %l>  %m%n"));
    mingo::LogAppender::ptr apdr = mingo::LogAppender::ptr(new mingo::FileLogAppender("../test1.txt"));
    apdr->setFormatter(fmtr);
    apdr->setLevel(mingo::LogLevel::DEBUG);
    logger->addAppender(apdr);

    test_yaml(logger);
    return 0;
}
