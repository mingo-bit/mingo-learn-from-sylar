#ifndef __MINGO_ENV_H__
#define __MINGO_ENV_H__

#include "mingo/singleton.h"
#include "mingo/thread.h"
#include <map>
#include <vector>

namespace mingo {

class Env {
public:
    typedef RWLock RWMutexType;
    bool init(int argc, char** argv);

    void add(const std::string& key, const std::string& val);
    bool has(const std::string& key);
    void del(const std::string& key);
    std::string get(const std::string& key, const std::string& default_value = "");

    void addHelp(const std::string& key, const std::string& desc);
    void removeHelp(const std::string& key);
    void printHelp();

    bool setEnv(const std::string& key, const std::string& val);
    std::string getEnv(const std::string& key, const std::string& default_value = "");

private:
    RWMutexType m_mutex;
    std::map<std::string, std::string> m_args;
    std::vector<std::pair<std::string, std::string> > m_helps;

    std::string m_program;
};

typedef mingo::Singleton<Env> EnvMgr;

}

#endif

