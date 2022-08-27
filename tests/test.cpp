#include <iostream>
#include "mingo/log.h"
#include "mingo/util.h"
#include "mingo/config.h"

int main(int argv, char** argc)
{
    mingo::Logger::ptr logger(new mingo::Logger);
    logger->addAppender(mingo::LogAppender::ptr(new mingo::StdoutLogAppender));

    logger->addAppender(mingo::LogAppender::ptr(new mingo::FileLogAppender("../test.txt")));

    mingo::LogEvent::ptr event(new mingo::LogEvent(__FILE__, __LINE__, mingo::getThreadId(), mingo::getFiberId(), time(0), "Hello World"));

    logger->log(event);
    return 0;
}
