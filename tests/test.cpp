#include <iostream>
#include "../mingo/log.h"

int main(int argv, char** argc)
{
    mingo::Logger::ptr logger(new mingo::Logger);
    logger->addAppender(mingo::LogAppender::ptr(new mingo::StdoutLogAppender));

    mingo::LogEvent::ptr event(new mingo::LogEvent(__FILE__, __LINE__, 0, 1, time(0), "Hello World"));

    logger->log(event);
    return 0;
}
