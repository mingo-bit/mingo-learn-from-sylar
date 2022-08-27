#include "mingo/log.h"
#include "mingo/config.h"
#include "mingo/util.h"

mingo::ConfigVar<int>::ptr init_value_config = 
    mingo::Config::Lookup("system.port", (int)8080, "system port");

int main(int argc, char *argv[])
{
    mingo::Logger::ptr logger(new mingo::Logger);
    logger->addAppender(mingo::LogAppender::ptr(new mingo::StdoutLogAppender));


    mingo::LogFormatter::ptr fmtr = mingo::LogFormatter::ptr(new mingo::LogFormatter("[%d]  [T:%t F:%F]  [%c]  [%p]  <%f: %l>  %m%n"));
    mingo::LogAppender::ptr apdr = mingo::LogAppender::ptr(new mingo::FileLogAppender("../test1.txt"));
    apdr->setFormatter(fmtr);
    apdr->setLevel(mingo::LogLevel::DEBUG);
    logger->addAppender(apdr);

    mingo::LogEvent::ptr event(new mingo::LogEvent(__FILE__, __LINE__, mingo::getThreadId(), mingo::getFiberId(), time(0), init_value_config->toString()));


    logger->log(event);


    return 0;
}
