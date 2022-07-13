#ifndef MINGO_LOG_H_
#define MINGO_LOG_H_

#include <string.h>
#include <stdint.h>
#include <string>
#include <list>
#include <map>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <cassert>
#include <iostream>

namespace mingo {

//日志等级
class LogLevel {
public:
    enum Level {
        UNKOWN = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* levelToString(LogLevel::Level level);
};

//日志事件
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr; 

    LogEvent(const std::string& filename, uint32_t line, uint32_t thread_id, 
            uint32_t fiber_id, time_t time, const std::string& content,
            LogLevel::Level level = LogLevel::DEBUG)
        : m_level(level), m_file(filename), m_line(line), 
        m_threadId(thread_id), m_fiberId(fiber_id), m_time(time), m_content(content) {}

    const std::string& getFile() const { return m_file; }
    uint32_t getLine() const { return m_line; }
    uint32_t getElapse() const { return m_elapse; }
    uint32_t getThreadId() const { return m_threadId; }
    uint32_t getFiberId() const { return m_fiberId; }
    uint32_t getTime() const { return m_time; }
    const std::string& getContent() const { return m_content; }
    LogLevel::Level getLevel() const { return m_level; }
    
    void setLevel(LogLevel::Level level) { m_level = level; }

private:
    std::string m_file;  //文件名
    LogLevel::Level m_level;    //日志等级
    uint32_t m_line = 0; //行号
    uint32_t m_elapse = 0; //从程序开始到现在的毫秒数
    uint32_t m_threadId = 0; //线程id
    uint32_t m_fiberId = 0; //协程id
    uint32_t m_time = 0; //时间戳
    std::string m_content;
};

//日志格式化器
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;  
    std::string format(LogEvent::ptr event);
    explicit LogFormatter(const std::string& pattern);
    void init(); 

    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr; 
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, LogEvent::ptr event) = 0;
    }; 
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
};

//日志输出
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr; 

    explicit LogAppender(LogLevel::Level level = LogLevel::DEBUG);
    virtual  ~LogAppender() {}
    virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr formatter); 
    LogFormatter::ptr getFormatter(); 
protected:
    LogFormatter::ptr m_formatter; 
    LogLevel::Level m_level; 
};

//输出到控制台的日志输出器
class StdoutLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(LogLevel::Level level, LogEvent::ptr event) override;

    explicit StdoutLogAppender(LogLevel::Level level = LogLevel::DEBUG);
   
};

//输出到文件的日志输出器
class FileLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    void log(LogLevel::Level level, LogEvent::ptr event) override;

    explicit FileLogAppender(const std::string& file_name, LogLevel::Level level = LogLevel::DEBUG);
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};

//日志器
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(); 
    Logger(const std::string& name, LogLevel::Level level, const std::string& pattern);

    //日志输出
    void log(LogEvent::ptr event); 
    
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    
    void setLevel(LogLevel::Level level) { m_level = level; }
    LogLevel::Level getLevel() const { return m_level; }

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    
private:
    std::string m_name;    //日志名称;
    LogLevel::Level m_level;        //日志级别;
    std::string m_format_pattern;   //日志输出格式化器的默认pattern
    LogFormatter::ptr m_formatter;  //日志默认格式化器
    std::list<LogAppender::ptr> m_appenders;    //Appender列表
};

}
#endif
