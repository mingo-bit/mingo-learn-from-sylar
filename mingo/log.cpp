#include "log.h" 
#include "config.h"

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

LogLevel::Level LogLevel::stringToLevel(const std::string &str) {
#define XX(level, v) if(str == #v) { return LogLevel::level; }
    XX(FATAL, fatal);
    XX(ERROR, error);
    XX(WARN, warn);
    XX(INFO, info);
    XX(DEBUG, debug);

    XX(FATAL, FATAL);
    XX(ERROR, ERROR);
    XX(WARN, WARN);
    XX(INFO, INFO);
    XX(DEBUG, DEBUG);
#undef XX

    return LogLevel::UNKNOW;
}

Logger::Logger() : m_name("default"), m_level(LogLevel::DEBUG), m_format_pattern("[%d] [%p] [%c] <%f: %l> [T:%t F:%F]%T%m%n")
{
    m_formatter.reset(new LogFormatter(m_format_pattern));
}

Logger::Logger(const std::string& name, LogLevel::Level level, const std::string& pattern)
    : m_name(name), m_level(level), m_format_pattern(pattern)
{
    m_formatter.reset(new LogFormatter(pattern));
}

Logger::Logger(const std::string& name) : m_name(name), m_level(mingo::LogLevel::INFO) {
}

void Logger::log(LogEvent::ptr event)
{
    if (event->getLevel() >= m_level)
    {
        auto self = shared_from_this();
        mingo::Spinlock::Lock lock(m_mutex);
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

void Logger::setFormatter(LogFormatter::ptr val) {
    mingo::Spinlock::Lock lock(m_mutex);
    m_formatter = val;
}

void Logger::setFormatter(const std::string& val) {
    mingo::LogFormatter::ptr new_val(new mingo::LogFormatter(val));
    if(new_val->isError()) {
        std::cout << "Logger setFormatter name=" << m_name
                  << " value=" << val << " invalid formatter"
                  << std::endl;
        return;
    }
    setFormatter(new_val);
}

LogFormatter::ptr Logger::getFormatter() {
    mingo::Spinlock::Lock lock(m_mutex);
    return m_formatter;
}

void Logger::addAppender(LogAppender::ptr appender)
{
    mingo::Spinlock::Lock lock(m_mutex);
    if (!appender->getFormatter())
    {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender); 
}

void Logger::delAppender(LogAppender::ptr appender)
{
    mingo::Spinlock::Lock lock(m_mutex);
    for (auto it = m_appenders.begin(); it != m_appenders.end(); it++)
        if (*it == appender)
        {
            m_appenders.erase(it);
            break;
        }       
}    

std::string Logger::toYamlString() {
    mingo::Spinlock::Lock lock(m_mutex);
    YAML::Node node;
    node["name"] = m_name;
    if(m_level != LogLevel::UNKNOW) {
        node["level"] = LogLevel::levelToString(m_level);
    }
    if(m_formatter) {
        node["formatter"] = m_formatter->getPattern();
    }
    for(auto &i : m_appenders) {
        node["appenders"].push_back(YAML::Load(i->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

LogFormatter::ptr LogAppender::getFormatter()
{
    mingo::Spinlock::Lock lock(m_mutex);
    return m_formatter;
}

void Logger::clearAppenders() {
    mingo::Spinlock::Lock lock(m_mutex);
    m_appenders.clear();
}

void LogAppender::setFormatter(LogFormatter::ptr formatter)
{
    mingo::Spinlock::Lock lock(m_mutex);
    m_formatter = std::move(formatter);
}

StdoutLogAppender::StdoutLogAppender(LogLevel::Level level)
    : LogAppender(level) {}

void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{ 
    if (level >= m_level)
    {
        mingo::Spinlock::Lock lock(m_mutex);
        if (!m_formatter->isError()) {
            std::cout << m_formatter->format(logger, event); 
            std::cout.flush();
        } else {
            event->setContent(event->getContent() + " logger: " + logger->getName() + " StdoutLogAppender formatter error. ");
            GET_GLOBAL_LOGGER()->log(event); // 当前日志器的输出器的格式错误，转为让全局日志器输出
        }
    }
}

std::string StdoutLogAppender::toYamlString() {
    mingo::Spinlock::Lock lock(m_mutex);
    YAML::Node node;
    node["type"] = "StdoutLogAppender";
    if(m_level != LogLevel::UNKNOW) {
        node["level"] = LogLevel::levelToString(m_level);
    }
    if(m_formatter) {
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
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
        mingo::Spinlock::Lock lock(m_mutex);
        if (!m_formatter->isError()) {
            m_filestream << m_formatter->format(logger, event);
            m_filestream.flush();
        } else {
            event->setContent(event->getContent() + " logger: " + logger->getName() + " FileLogAppender formatter error. ");
            GET_GLOBAL_LOGGER()->log(event); // 当前日志器的输出器的格式错误，转为让全局日志器输出
        }
    }
}

bool FileLogAppender::reopen()
{
    mingo::Spinlock::Lock lock(m_mutex);
    if (m_filestream)
        m_filestream.close();
    m_filestream.open(m_filename, std::ios_base::out | std::ios_base::app);
    return !!m_filestream;
} 

std::string FileLogAppender::toYamlString() {
    mingo::Spinlock::Lock lock(m_mutex);
    YAML::Node node;
    node["type"] = "FileLogAppender";
    node["file"] = m_filename;
    if(m_level != LogLevel::UNKNOW) {
        node["level"] = LogLevel::levelToString(m_level);
    }
    if(m_formatter) {
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
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
                    //转义字符出错， formatter格式错误， 设置error标识符
                    std::cout << "LogFormatter::init() error, pattern: " << m_pattern[i] << " is not exist. " << std::endl;
                    m_error = true;
                    return;
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
    m_global.reset(new Logger("global", LogLevel::DEBUG, "[%d] [%p] [%c] <%f: %l> [T:%t F:%F]%T%m%n"));
    m_global->addAppender(LogAppender::ptr(new StdoutLogAppender));
    m_loggers[m_global->getName()] = m_global;
    init();
}

void LoggerManager::init()
{

}

Logger::ptr LoggerManager::getLogger(const std::string& name)
{
    mingo::Spinlock::Lock lock(m_mutex);
    auto iter = m_loggers.find(name);
    if (iter != m_loggers.end())
    {
        // 日志器存在，则返回日志器 
        return iter->second;
    }
    // 日志器不存在，则创建新日志器，并将指针指向全局日志器
    Logger::ptr new_logger(new Logger(name, LogLevel::UNKNOW, "[%d] [%p] [%c] <%f: %l> [T:%t F:%F]%T%m%n"));
    new_logger->addAppender(LogAppender::ptr(new StdoutLogAppender));
    m_loggers[name] = new_logger;
    return new_logger;
}

std::string LoggerManager::toYamlString() {
    mingo::Spinlock::Lock lock(m_mutex);
    YAML::Node node;
    for(auto& i : m_loggers) {
        node.push_back(YAML::Load(i.second->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

//!!!!!!从日志文件中加载日志配置

// LogAppender配置定义
struct LogAppenderDefine {
    int type = 0; // 1 File, 2 Stdout
    std::string formatter;
    std::string file;
    LogLevel::Level level = LogLevel::UNKNOW;

    bool operator==(const LogAppenderDefine &oth) const {
        return type == oth.type && formatter == oth.formatter && file == oth.file && level == oth.level;
    }
};

// Logger配置定义
struct LogDefine {
    std::string name;
    LogLevel::Level level = LogLevel::UNKNOW;
    std::string formatter;
    std::vector<LogAppenderDefine> appenders;

    bool operator==(const LogDefine &oth) const {
        return name == oth.name && level == oth.level && appenders == appenders && formatter == oth.formatter;
    }

    bool operator<(const LogDefine &oth) const {
        return name < oth.name;
    }

    bool isValid() const {
        return !name.empty();
    }
};

template<>
class LexicalCast<std::string, LogDefine> {
public:
    LogDefine operator()(const std::string &v) {
        YAML::Node n = YAML::Load(v);
        LogDefine ld;
        if(!n["name"].IsDefined()) {
            std::cout << "log config error: name is null, " << n << std::endl;
            throw std::logic_error("log config name is null");
        }
        ld.name = n["name"].as<std::string>();
        ld.level = LogLevel::stringToLevel(n["level"].IsDefined() ? n["level"].as<std::string>() : "");
        if(n["formatter"].IsDefined()) {
            ld.formatter = n["formatter"].as<std::string>();
        }

        if(n["appenders"].IsDefined()) {
            for(size_t i = 0; i < n["appenders"].size(); i++) {
                auto a = n["appenders"][i];
                if(!a["type"].IsDefined()) {
                    std::cout << "log appender config error: appender type is null, " << a << std::endl;
                    continue;
                }
                std::string type = a["type"].as<std::string>();
                LogAppenderDefine lad;
                if(type == "FileLogAppender") {
                    lad.type = 1; 
                    if(!a["file"].IsDefined()) {
                        std::cout << "log appender config error: file appender file is null, " << a << std::endl;
                        continue;
                    }
                    lad.file = a["file"].as<std::string>();
                    if(a["formatter"].IsDefined()) {
                        lad.formatter = a["formatter"].as<std::string>();
                    }
                } else if(type == "StdoutLogAppender") {
                    lad.type = 2;
                    if(a["formatter"].IsDefined()) {
                        lad.formatter = a["formatter"].as<std::string>();
                    }
                } else {
                    std::cout << "log appender config error: appender type is invalid, " << a << std::endl;
                    continue;
                }
                if (a["level"].IsDefined()) {
                    lad.level = mingo::LogLevel::stringToLevel(a["level"].Scalar());
                } else {
                    lad.level = mingo::LogLevel::DEBUG;
                }
                ld.appenders.push_back(lad);
            }
        } // end for
        return ld;
    }
};

template<>
class LexicalCast<LogDefine, std::string> {
public:
    std::string operator()(const LogDefine &i) {
        YAML::Node n;
        n["name"] = i.name;
        n["level"] = LogLevel::levelToString(i.level);
        if(!i.formatter.empty()) {
            n["formatter"] = i.formatter;
        }
        for(auto &a : i.appenders) {
            YAML::Node na;
            if(a.type == 1) {
                na["type"] = "FileLogAppender";
                na["file"] = a.file;
            } else if(a.type == 2) {
                na["type"] = "StdoutLogAppender";
            }
            if(a.level != LogLevel::UNKNOW) {
                na["level"] = LogLevel::levelToString(a.level);
            }
            if(!a.formatter.empty()) {
                na["formatter"] = a.formatter;
            }
            n["appenders"].push_back(na);
        }
        std::stringstream ss;
        ss << n;
        return ss.str();
    }
};

mingo::ConfigVar<std::set<LogDefine>>::ptr g_log_defines = 
    mingo::Config::Lookup("logs", std::set<LogDefine>(), "logs config");

struct LogIniter {
    LogIniter() {
        g_log_defines->addListener([](const std::set<LogDefine> &old_value, const std::set<LogDefine> &new_value){
            LOG_INFO(GET_GLOBAL_LOGGER(), "on LogConfig changed");
            for(auto &i : new_value) {
                auto it = old_value.find(i);
                mingo::Logger::ptr logger;
                if(it == old_value.end()) {
                    // 新增logger
                    logger = GET_LOGGER(i.name);
                } else {
                    if(!(i == *it)) {
                        // 修改的logger
                        logger == GET_LOGGER(i.name);
                    } else {
                        continue;
                    }
                }
                logger->setLevel(i.level);
                if(!i.formatter.empty()) {
                    logger->setFormatter(i.formatter);
                }
                logger->clearAppenders();
                for(auto &a : i.appenders) {
                    mingo::LogAppender::ptr ap;
                    if(a.type == 1) {
                        ap.reset(new FileLogAppender(a.file));
                    } else if(a.type == 2) {
                        ap.reset(new StdoutLogAppender);
                    }
                    ap->setLevel(a.level);
                    if(!a.formatter.empty()) {
                        LogFormatter::ptr fmt(new LogFormatter(a.formatter));
                        if(!fmt->isError()) {
                            ap->setFormatter(fmt);
                        } else {
                            std::cout << "log.name=" << i.name << " appender type=" << a.type
                                      << " formatter=" << a.formatter << " is invalid" << std::endl;
                        }
                    }

                    logger->addAppender(ap);
                }
            }

            // 以配置文件为主，如果程序里定义了配置文件中未定义的logger，那么把程序里定义的logger设置成无效
            for(auto &i : old_value) {
                auto it = new_value.find(i);
                if(it == new_value.end()) {
                    auto logger = GET_LOGGER(i.name);
                    logger->setLevel(LogLevel::Level(800));
                    logger->clearAppenders();
                }
            }
        });
    }
};

//在main函数之前注册配置更改的回调函数
//用于在更新配置时将log相关的配置加载到Config
static LogIniter __log_init;

}
