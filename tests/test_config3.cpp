// 配置系统复杂类型和自定义类型支持测试
#include "mingo/log.h"
#include "mingo/config.h"
#include "mingo/util.h"
#include <yaml-cpp/yaml.h>

mingo::ConfigVar<std::vector<int> >::ptr int_vec_value_config = 
    mingo::Config::Lookup("system.vec", std::vector<int>{1, 2}, "system vec");

mingo::ConfigVar<std::list<int> >::ptr int_list_value_config = 
    mingo::Config::Lookup("system.list", std::list<int>{1, 2}, "system list");

mingo::ConfigVar<std::set<int> >::ptr int_set_value_config = 
    mingo::Config::Lookup("system.set", std::set<int>{1, 2}, "system set");

mingo::ConfigVar<std::map<std::string, int> >::ptr int_map_value_config = 
    mingo::Config::Lookup("system.map", std::map<std::string, int>{ {"xxhh", 2}, {"xxh", 3} }, "system map");
    
mingo::ConfigVar<std::unordered_set<int> >::ptr int_unordered_set_value_config = 
    mingo::Config::Lookup("system.unordered_set", std::unordered_set<int>{1, 2}, "system unordered_set");

mingo::ConfigVar<std::unordered_map<std::string, int> >::ptr int_unordered_map_value_config = 
    mingo::Config::Lookup("system.unordered_map", std::unordered_map<std::string, int>{ {"xxhh", 2}, {"xxh", 3} }, "system unordered_map");

class Person {
public:
    std::string name;
    int age = 0;
    bool sex = 1;
    std::string toString() const {
        std::stringstream ss;
        ss << "[Person name: " << name
           << " age: " << age
           << " sex: " << sex
           << "]";
        return ss.str();
    }
    bool operator== (const Person &p) const {
        return name == p.name && age == p.age && sex == p.sex;
    }
};

namespace mingo {
template<>
class LexicalCast<std::string, Person > {
public:
    Person operator() (const std::string& v) {
        YAML::Node node = YAML::Load(v);
        Person p;
        p.name = node["name"].as<std::string>();
        p.age = node["age"].as<int>();
        p.sex = node["sex"].as<bool>();
        return p;
    }
};
        
template<>
class LexicalCast<Person, std::string> {
public:
    std::string operator() (const Person &p) {
        YAML::Node node;
        node["name"] = p.name;
        node["age"] = p.age;
        node["sex"] = p.sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};
}

mingo::ConfigVar<Person>::ptr person_value_config = 
    mingo::Config::Lookup("class.person", Person(), "class person");

mingo::ConfigVar<std::vector<Person> >::ptr vec_person_value_config = 
    mingo::Config::Lookup("class.vec_person", std::vector<Person>{Person(), Person()}, "class vec person");
    
int main(int argc, char *argv[])
{
    mingo::Logger::ptr logger(new mingo::Logger);
    logger->addAppender(mingo::LogAppender::ptr(new mingo::StdoutLogAppender));

    mingo::LogEvent::ptr event(new mingo::LogEvent(__FILE__, __LINE__, mingo::getThreadId(), mingo::getFiberId(), time(0), "before vec: " + int_vec_value_config->toString()));

    logger->log(event);

    event->setContent("before list: " + int_list_value_config->toString());
    logger->log(event);

    event->setContent("before set: " + int_set_value_config->toString());
    logger->log(event);

    event->setContent("before m: " + int_map_value_config->toString());
    logger->log(event);

    event->setContent("before um: " + int_unordered_map_value_config->toString());
    logger->log(event);

    event->setContent("before us: " + int_unordered_set_value_config->toString());
    logger->log(event);

    event->setContent("before p: " + person_value_config->toString());
    logger->log(event);

    event->setContent("before vp: " + vec_person_value_config->toString());
    logger->log(event);

    YAML::Node root = YAML::LoadFile("/home/mingo/workspace/mingo_server_copy_sylar/tests/configure/log.yml");
    mingo::Config::LoadFromYaml(root);

    event->setContent("after: " + int_vec_value_config->toString());
    logger->log(event);

    event->setContent("after list: " + int_list_value_config->toString());
    logger->log(event);

    event->setContent("after set: " + int_set_value_config->toString());
    logger->log(event);

    event->setContent("after m: " + int_map_value_config->toString());
    logger->log(event);

    event->setContent("after um: " + int_unordered_map_value_config->toString());
    logger->log(event);

    event->setContent("after us: " + int_unordered_set_value_config->toString());
    logger->log(event);

    event->setContent("after p: " + person_value_config->toString());
    logger->log(event);

    event->setContent("after vp: " + vec_person_value_config->toString());
    logger->log(event);
    return 0;
}
