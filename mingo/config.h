#ifndef MINGO_CONFIG_H_
#define MINGO_CONFIG_H_

#include <boost/lexical_cast.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <sstream>
#include <string>
#include <yaml-cpp/yaml.h>
#include <functional>

namespace mingo {

//配置项基类
class ConfigVarBase
{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;

    ConfigVarBase(const std::string& name, const std::string& description)
        : m_name(name), m_description(description)
    {
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    }
    virtual ~ConfigVarBase() {}

    const std::string& getName() const { return m_name;}
    const std::string& getDescription() const { return m_description;}
    // 将配置项的值转化为字符串
    virtual std::string toString() const = 0;
    // 通过字符串来设置配置项的值
    virtual bool fromString(const std::string& val) = 0;
    // 获取配置项值的类型
    virtual std::string getType() const = 0;

protected:
    std::string m_name;    // 配置项的名称
    std::string m_description;  // 配置项的备注
};

// F from_type T to_type
template <class F, class T>
class LexicalCast {
public:
    T operator() (const F& v) {
       return boost::lexical_cast<T>(v);
    }

};

template <class T>
class LexicalCast<std::string, std::vector<T> > {
public:
    std::vector<T> operator() (const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); i++)
        {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>() (ss.str()));
        }
        return vec;
    }
};
        
template <class T>
class LexicalCast<std::vector<T>, std::string> {
public:
    std::string operator() (const std::vector<T> & v) {
        YAML::Node node;
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, 
                        std::string>() (i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::list<T> > {
public:
    std::list<T> operator() (const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::list<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); i++)
        {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>() (ss.str()));
        }
        return vec;
    }
};
        
template <class T>
class LexicalCast<std::list<T>, std::string> {
public:
    std::string operator() (const std::list<T> & v) {
        YAML::Node node;
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, 
                        std::string>() (i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::set<T> > {
public:
    std::set<T> operator() (const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::set<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); i++)
        {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>() (ss.str()));
        }
        return vec;
    }
};
        
template <class T>
class LexicalCast<std::set<T>, std::string> {
public:
    std::string operator() (const std::set<T> & v) {
        YAML::Node node;
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, 
                        std::string>() (i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::map<std::string, T> > {
public:
    std::map<std::string, T> operator() (const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T> vec;
        std::stringstream ss;
        for (auto it = node.begin(); it != node.end(); ++it)
        {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(), LexicalCast<std::string, T>() (ss.str())));
        }
        return vec;
    }
};
        
template <class T>
class LexicalCast<std::map<std::string, T>, std::string> {
public:
    std::string operator() (const std::map<std::string, T> & v) {
        YAML::Node node;
        for (auto &i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>() (i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::unordered_set<T> > {
public:
    std::unordered_set<T> operator() (const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); i++)
        {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>() (ss.str()));
        }
        return vec;
    }
};
        
template <class T>
class LexicalCast<std::unordered_set<T>, std::string> {
public:
    std::string operator() (const std::unordered_set<T> & v) {
        YAML::Node node;
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, 
                        std::string>() (i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::unordered_map<std::string, T> > {
public:
    std::unordered_map<std::string, T> operator() (const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T> vec;
        std::stringstream ss;
        for (auto it = node.begin(); it != node.end(); ++it)
        {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(), LexicalCast<std::string, T>() (ss.str())));
        }
        return vec;
    }
};
        
template <class T>
class LexicalCast<std::unordered_map<std::string, T>, std::string> {
public:
    std::string operator() (const std::unordered_map<std::string, T> & v) {
        YAML::Node node;
        for (auto &i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>() (i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T, class FromStr = LexicalCast<std::string, T>, class ToStr = LexicalCast<T, std::string> >
class ConfigVar : public ConfigVarBase
{
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void (const T& old_val, const T& new_val)> on_change_cb;
    
    ConfigVar(const std::string& name, const T& value, const std::string& description)
        : ConfigVarBase(name, description), m_value(value) {}

    T getValue() const
    {
        return m_value;
    }

    void setValue(const T value)
    {
        if (m_value == value)
            return;
        for (auto &i: m_cbs)
            i.second(m_value, value);
        m_value = value;
    }

    std::string toString() const override
    {
        try {
           return ToStr() (m_value);
        }
        catch (std::exception& e)
        {
            std::cerr << "ConfigVar::toString exception "
                      << e.what()
                      << " convert: "
                      << typeid(m_value).name()
                      << " to string" << std::endl;
        }
        return "<error>";
    }
    
    bool fromString(const std::string& val) override
    {
        try {
            setValue(FromStr() (val));
        }
        catch (std::exception& e)
        {
            std::cerr << "ConfigVar::fromString exception "
                      << e.what()
                      << " convert: "
                      << typeid(m_value).name() <<  std::endl;
        }
        return false;
    } 

    std::string getType() const override { return typeid(T).name(); }
    void addListener(uint64_t key, on_change_cb cb) {
        m_cbs[key] = cb; 
    }

    void delListener(uint64_t key) {
        m_cbs.erase(key);
    }

    on_change_cb getListener(uint64_t key) {
        auto it = m_cbs.find(key);
        return it == m_cbs.end() ? nullptr : it->second;
    }

private:
    T m_value; // 配置项的值
    std::map<uint64_t, on_change_cb> m_cbs;
};

class Config
{
public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    // 创建配置项
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name, const T& value, const std::string description = "")
    {
        auto it = s_datas.find(name);
        if (it != s_datas.end())
        {
            auto temp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
            if (temp) {
                std::cout << "Lookup name find: ConfigVar exists : name : " << name
                    << std::endl;
                return temp;
            } else {
                std::cerr << "Lookup ConfigVar name not find because type is not " << typeid(T).name() << " real_type: " << it->second->getType()
                    << " real_var: " << it->second->toString() << std::endl; 
                return nullptr;
            }
        }

        if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678") != std::string::npos)
        {
            std::cerr << "Lookup name invalid: " << name;
            throw std::invalid_argument(name);
        }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, value, description));
        s_datas[name] = v;

        return v;
    }

    // 查找配置项，返回指定类型的ConfigVar智能指针
    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name)
    {
        auto base_ptr = s_datas.find(name);
        if (base_ptr == s_datas.end())
        {
            return nullptr;
        }
        // 配置项存在，尝试转换成指定的类型
        return std::dynamic_pointer_cast<ConfigVar<T> >(base_ptr->second);
    }

    static ConfigVarBase::ptr LookupBase(const std::string& name);

    static void LoadFromYaml(const YAML::Node& root);

private:
    static ConfigVarMap s_datas; // 配置项表
};
}

#endif
