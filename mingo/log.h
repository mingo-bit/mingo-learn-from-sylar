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
#include "singleton.h"
#include "util.h"
#include "thread.h"

#define MAKE_LOG_EVENT(level, message) \
    std::make_shared<mingo::LogEvent>(__FILE__, __LINE__, mingo::getThreadId(), mingo::getFiberId(), time(0), message, level)

#define LOG_LEVEL(logger, level, message) \
    logger->log(MAKE_LOG_EVENT(level, message));

#define LOG_DEBUG(logger, message) LOG_LEVEL(logger, mingo::LogLevel::DEBUG, message)
#define LOG_INFO(logger, message) LOG_LEVEL(logger, mingo::LogLevel::INFO, message)
#define LOG_WARN(logger, message) LOG_LEVEL(logger, mingo::LogLevel::WARN, message)
#define LOG_ERROR(logger, message) LOG_LEVEL(logger, mingo::LogLevel::ERROR, message)
#define LOG_FATAL(logger, message) LOG_LEVEL(logger, mingo::LogLevel::FATAL, message)

#define GET_GLOBAL_LOGGER() mingo::LoggerMgr::getInstance()->getGlobal()
#define GET_LOGGER(name) mingo::LoggerMgr::getInstance()->getLogger(name)

namespace mingo {

//日志等级
class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* levelToString(LogLevel::Level level);
    static LogLevel::Level stringToLevel(const std::string &str);
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
    void setContent(const std::string& content) { m_content  = content; }
    LogLevel::Level getLevel() const { return m_level; }
    
    void setLevel(LogLevel::Level level) { m_level = level; }

private:
    std::string m_file;  //文件名
    LogLevel::Level m_level;    //事件等级
    uint32_t m_line = 0; //行号
    uint32_t m_elapse = 0; //从程序开始到现在的毫秒数
    uint32_t m_threadId = 0; //线程id
    uint32_t m_fiberId = 0; //协程id
    uint32_t m_time = 0; //时间戳
    std::string m_content;
};

class Logger;

//日志格式化器
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;  
    std::string format(std::shared_ptr<Logger> logger, LogEvent::ptr event);
    explicit LogFormatter(const std::string& pattern);
    LogFormatter();
    void init(); 
    bool isError() const { return m_error; } 
    std::string getPattern() const { return m_pattern; }

    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr; 
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) = 0;
    }; 
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error = false; // 格式是否出错
};

//日志输出
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr; 

    explicit LogAppender(LogLevel::Level level = LogLevel::DEBUG);
    virtual  ~LogAppender() {}
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr formatter); 
    void setLevel(LogLevel::Level level) { m_level = level; }
    LogLevel::Level getLevel() const { return m_level; }
    LogFormatter::ptr getFormatter(); 
    // 将日志输出目标的配置转化成YAML配置
    virtual std::string toYamlString() = 0;

protected:
    LogFormatter::ptr m_formatter; 
    LogLevel::Level m_level; 
    mingo::Spinlock m_mutex;
};

//输出到控制台的日志输出器
class StdoutLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYamlString() override;

    explicit StdoutLogAppender(LogLevel::Level level = LogLevel::DEBUG);
   
};

//输出到文件的日志输出器
class FileLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

    explicit FileLogAppender(const std::string& file_name, LogLevel::Level level = LogLevel::DEBUG);
    bool reopen();
    std::string toYamlString() override;
private:
    std::string m_filename;
    std::ofstream m_filestream;
};

//日志器
class Logger : public std::enable_shared_from_this<Logger>{
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(); 
    Logger(const std::string& name, LogLevel::Level level, const std::string& pattern);
    Logger(const std::string& name);

    //日志输出
    void log(LogEvent::ptr event); 
    
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    
    void setLevel(LogLevel::Level level) { m_level = level; }
    LogLevel::Level getLevel() const { return m_level; }

    void setFormatter(LogFormatter::ptr val);
    void setFormatter(const std::string& val);
    LogFormatter::ptr getFormatter();
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAppenders();
    
    std::string getName() const { return m_name; }
    std::string toYamlString(); // 将日志的配置输出成为Yaml配置
    
private:
    std::string m_name;    //日志名称;
    LogLevel::Level m_level;        //日志级别;
    std::string m_format_pattern;   //日志输出格式化器的默认pattern
    LogFormatter::ptr m_formatter;  //日志默认格式化器
    std::list<LogAppender::ptr> m_appenders;    //Appender列表
    mingo::Spinlock m_mutex;
};

//日志管理器
class LoggerManager{
public:
    typedef std::shared_ptr<LoggerManager> ptr;

    LoggerManager();
    // 传入日志器名称来获取日志器,如果不存在,返回全局日志器
    Logger::ptr getLogger(const std::string& name);
    //获取全局日志器
    Logger::ptr getGlobal() { return m_global; }
    std::string toYamlString();
    void init();

private:
    mingo::Spinlock m_mutex;
    std::map<std::string, Logger::ptr> m_loggers;
    Logger::ptr m_global; // 全局日志器
};

typedef mingo::Singleton<LoggerManager> LoggerMgr;

}

#endif
