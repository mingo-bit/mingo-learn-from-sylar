#include "log.h" 
namespace mingo {

class PlainFormatItem : public LogFormatter::FormatItem
{
public:
    explicit PlainFormatItem(const std::string& str) : m_str(str) {}
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << m_str;
    }

private:
    std::string m_str;
};

class LevelFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << LogLevel::levelToString(event->getLevel());
    }
};

class NameFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << logger->getName(); 
    }
};

class FilenameFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << event->getFile();
    }
};

class LineFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << event->getLine();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << event->getThreadId();
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << event->getFiberId();
    }
};

class TimeFormatItem : public LogFormatter::FormatItem
{
public:
    explicit TimeFormatItem (const std::string& str = "%Y-%m-%d %H:%M:%S")
        : m_time_pattern(str)
    {
        if (m_time_pattern.empty())
        {
            m_time_pattern = "%Y-%m-%d %H:%M:%S";
        }
    }
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        struct tm time_struct
        {
        };
        time_t time_l = event->getTime();
        localtime_r(&time_l, &time_struct);
        char buffer[64]{0};
        strftime(buffer, sizeof(buffer),
                m_time_pattern.c_str(), &time_struct);
        os << buffer;
    }

private:
    std::string m_time_pattern;
};

class ContentFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << event->getContent();
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << std::endl;
    }
};

class PercentSignFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << '%';
    }
};

class TabFormatItem : public LogFormatter::FormatItem
{
public:
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) override
    {
        os << '\t';
    }
};
/**
 * %p 输出日志等级
 * %f 输出文件名
 * %l 输出行号
 * %d 输出日志时间
 * %t 输出线程号
 * %F 输出协程号
 * %m 输出日志消息
 * %n 输出换行
 * %% 输出百分号
 * %T 输出制表符
 * %c 输出日志名称
 * */
thread_local static std::map<char, LogFormatter::FormatItem::ptr> format_item_map{
#define FN(CH, ITEM_NAME)                 \
    {                                     \
        CH, std::make_shared<ITEM_NAME>() \
    }
    FN('p', LevelFormatItem),
    FN('f', FilenameFormatItem),
    FN('l', LineFormatItem),
    FN('d', TimeFormatItem),
    FN('t', ThreadIdFormatItem),
    FN('F', FiberIdFormatItem),
    FN('m', ContentFormatItem),
    FN('n', NewLineFormatItem),
    FN('%', PercentSignFormatItem),
    FN('T', TabFormatItem),
    FN('c', NameFormatItem),
#undef FN
};

const char* LogLevel::levelToString(LogLevel::Level level)
{
    switch(level) {
    case LogLevel::DEBUG:
        return "DEBUG";
        break;
    case LogLevel::INFO:
        return "INFO";
        break;
    case LogLevel::WARN:
        return "WARN";
        break;
    case LogLevel::ERROR:
        return "ERROR";
        break;
    case LogLevel::FATAL:
        return "FATAL";
        break;
    default:
        return "UNKNOW";
        break;
    }
}

Logger::Logger() : m_name("default"), m_level(LogLevel::DEBUG), m_format_pattern("[%d] [%p] <%f: %l> [T:%t F:%F]%T%m%n")
{
    m_formatter.reset(new LogFormatter(m_format_pattern));
}

Logger::Logger(const std::string& name, LogLevel::Level level, const std::string& pattern)
    : m_name(name), m_level(level), m_format_pattern(pattern)
{
    m_formatter.reset(new LogFormatter(pattern));
}

void Logger::log(LogEvent::ptr event)
{
    if (event->getLevel() >= m_level)
    {
        auto self = shared_from_this();
        for (auto& i : m_appenders)
            i->log(self, event->getLevel(), event);
    }
}

void Logger::debug(LogEvent::ptr event) {
    event->setLevel(LogLevel::DEBUG);
    log(event);
}

void Logger::info(LogEvent::ptr event){
    event->setLevel(LogLevel::INFO);
    log(event);
}

void Logger::warn(LogEvent::ptr event){
    event->setLevel(LogLevel::WARN);
    log(event);
}
   
void Logger::error(LogEvent::ptr event){
    event->setLevel(LogLevel::ERROR);
    log(event);
}

void Logger::fatal(LogEvent::ptr event){
    event->setLevel(LogLevel::FATAL);
    log(event);
}

void Logger::addAppender(LogAppender::ptr appender)
{
    if (!appender->getFormatter())
    {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender); 
}

void Logger::delAppender(LogAppender::ptr appender)
{
    for (auto it = m_appenders.begin(); it != m_appenders.end(); it++)
        if (*it == appender)
        {
            m_appenders.erase(it);
            break;
        }       
}    

LogFormatter::ptr LogAppender::getFormatter()
{
    return m_formatter;
}

void LogAppender::setFormatter(LogFormatter::ptr formatter)
{
    m_formatter = std::move(formatter);
}

StdoutLogAppender::StdoutLogAppender(LogLevel::Level level)
    : LogAppender(level) {}

void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{ 
    if (level >= m_level)
    {
        std::cout << m_formatter->format(logger, event); 
        std::cout.flush();
    }
}

LogAppender::LogAppender(LogLevel::Level level)
    : m_level(level) {}

FileLogAppender::FileLogAppender(const std::string& file_name, LogLevel::Level level)
    :m_filename(file_name), LogAppender(level) {
        reopen();
}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    if (level >= m_level)
    {
        m_filestream << m_formatter->format(logger, event);
        m_filestream.flush();
    }
}

bool FileLogAppender::reopen()
{
    if (m_filestream)
        m_filestream.close();
    m_filestream.open(m_filename, std::ios_base::out | std::ios_base::app);
    return !!m_filestream;
} 

LogFormatter::LogFormatter(const std::string& pattern) : 
    m_pattern(pattern) {
    init(); 
} 

void LogFormatter::init()
{

    enum PARSE_STATUS
    {
        SCAN_STATUS,    //扫描到普通字符
        CREATE_STATUS,  //扫描到%，处理占位符
    };
    PARSE_STATUS STATUS = SCAN_STATUS;
    size_t str_begin = 0, str_end = 0;
    for (size_t i = 0; i < m_pattern.length(); i++)
    {
        switch (STATUS)
        {
            case SCAN_STATUS:
                str_begin = i;
                for (str_end = i; str_end < m_pattern.length(); str_end++)
                {
                    if (m_pattern[str_end] == '%')
                    {
                        STATUS = CREATE_STATUS;
                        break;
                    }
                }
                i = str_end;
                m_items.push_back(
                        std::make_shared<PlainFormatItem>(
                            m_pattern.substr(str_begin, str_end - str_begin)));
                break;

            case CREATE_STATUS:
                assert(!format_item_map.empty() && "format_item_map 没有被正确初始化");
                auto itor = format_item_map.find(m_pattern[i]);
                if (itor == format_item_map.end())
                {
                    m_items.push_back(std::make_shared<PlainFormatItem>("<error format>"));
                }
                else
                {
                    m_items.push_back(itor->second);
                }
                STATUS = SCAN_STATUS;
                break;
        }

    }
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogEvent::ptr event) 
{
    std::stringstream ss;
    for (auto& i : m_items)
        i->format(ss, logger, event);
    return ss.str();
} 

LoggerManager::LoggerManager()
{
    init();
}

void LoggerManager::ensureGlobalLoggerExists()
{
    // std::map 不能够用 operator[] 来查询键是否存在，因为该操作符会获取不存在的键时会创建新节点
    auto iter = m_loggers.find("root");
    if (iter == m_loggers.end())
    { // 日志器 map 里不存在全局日志器
        auto global_logger = std::make_shared<Logger>();
        global_logger->addAppender(std::make_shared<StdoutLogAppender>());
        m_loggers.insert(std::make_pair("root", std::move(global_logger)));
    }
    else if (!iter->second)
    { // 存在同名的键，但指针为空
        iter->second = std::make_shared<Logger>();
        iter->second->addAppender(std::make_shared<StdoutLogAppender>());
    }
}

void LoggerManager::init()
{

}

Logger::ptr LoggerManager::getLogger(const std::string& name)
{
    auto iter = m_loggers.find(name);
    if (iter == m_loggers.end())
    {
        // 日志器不存在就返回全局默认日志器
        return m_loggers.find("root")->second;
    }
    return iter->second;
}

Logger::ptr LoggerManager::getGlobal()
{
    return getLogger("root");
}

}
