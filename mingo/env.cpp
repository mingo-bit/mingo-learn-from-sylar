#include "env.h"
#include "mingo/log.h"
#include <string.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>
#include "config.h"

namespace mingo {

static mingo::Logger::ptr g_logger = GET_LOGGER("system");

bool Env::init(int argc, char** argv) {

    m_program = argv[0];
    // -config /path/to/config -file xxxx -d
    const char* now_key = nullptr;
    for(int i = 1; i < argc; ++i) {
        if(argv[i][0] == '-') {
            if(strlen(argv[i]) > 1) {
                if(now_key) {
                    add(now_key, "");
                }
                now_key = argv[i] + 1;
            } else {
                LOG_ERROR(g_logger, "invalid arg idx=" + std::to_string(i)
                    + " val=" + std::to_string(0) + argv[i]);
                return false;
            }
        } else {
            if(now_key) {
                add(now_key, argv[i]);
                now_key = nullptr;
            } else {
                LOG_ERROR(g_logger, "invalid arg idx=" + std::to_string(i)
                    + " val=" + std::to_string(0) + argv[i]);
                return false;
            }
        }
    }
    if(now_key) {
        add(now_key, "");
    }
    return true;
}

void Env::add(const std::string& key, const std::string& val) {
    RWMutexType::WriteLock lock(m_mutex);
    m_args[key] = val;
}

bool Env::has(const std::string& key) {
    RWMutexType::ReadLock lock(m_mutex);
    auto it = m_args.find(key);
    return it != m_args.end();
}

void Env::del(const std::string& key) {
    RWMutexType::WriteLock lock(m_mutex);
    m_args.erase(key);
}

std::string Env::get(const std::string& key, const std::string& default_value) {
    RWMutexType::ReadLock lock(m_mutex);
    auto it = m_args.find(key);
    return it != m_args.end() ? it->second : default_value;
}

void Env::addHelp(const std::string& key, const std::string& desc) {
    removeHelp(key);
    RWMutexType::WriteLock lock(m_mutex);
    m_helps.push_back(std::make_pair(key, desc));
}

void Env::removeHelp(const std::string& key) {
    RWMutexType::WriteLock lock(m_mutex);
    for(auto it = m_helps.begin();
            it != m_helps.end();) {
        if(it->first == key) {
            it = m_helps.erase(it);
        } else {
            ++it;
        }
    }
}

void Env::printHelp() {
    RWMutexType::ReadLock lock(m_mutex);
    std::cout << "Usage: " << m_program << " [options]" << std::endl;
    for(auto& i : m_helps) {
        std::cout << std::setw(5) << "-" << i.first << " : " << i.second << std::endl;
    }
}

bool Env::setEnv(const std::string& key, const std::string& val) {
    return !setenv(key.c_str(), val.c_str(), 1);
}

std::string Env::getEnv(const std::string& key, const std::string& default_value) {
    const char* v = getenv(key.c_str());
    if(v == nullptr) {
        return default_value;
    }
    return v;
}

}

