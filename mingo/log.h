#ifndef MINGO_LOG_H_
#define MINGO_LOG_H_

#include <string.h>
#include <stdint.h>
#include <list>
#include <map>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>

namespace mingo {
//日志等级
class LogLevel {
public:
    enum Level {
        DEBUG = 1;
        INFO = 2;
        WARN = 3;
        ERROR = 4;
        FATAL = 5;
    };
};

//日志器
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event); 
    
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    
    void setLevel(LogLevel::Level level) { m_level = level; }
    LogLevel::Level GetLevel() const { return m_level; }
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    
private:
    std::string m_name;    //日志名称;
    LogLevel::Level m_level;        //日志级别;
    std::list<LogAppender::ptr> m_appenders    //Appender集合
};

//日志输出
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr; 
    virtual  ~LogAppender() {}
    virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr formatter) { m_formatter = formatter; }
    LogFormatter::ptr getFormatter() const { return m_formatter; } 
protected:
    LogFormatter::ptr m_formatter; 
    LogLevel::Level m_level; 
};

//输出到控制台的日志输出器
class StdoutLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
   
};

//输出到文件的日志输出器
class FileLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
    FileLogAppender(const std::string& file_name);
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};

//日志格式化器
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;  
    std::string format(LogEvent::ptr event); 
private:

};

//日志事件
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr; 
    LogEvent();
private:
    const char* m_file = nullptr;  //文件名
    int32_t m_line = 0; //行号
    uint32_t m_elapse = 0; //从程序开始到现在的毫秒数
    uint32_t m_threadId = 0; //线程id
    uint32_t m_fiberId = 0; //协程id
    uint32_t m_time = 0; //时间戳
    std::string m_content;
};
}

#endif
