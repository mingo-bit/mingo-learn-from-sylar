#ifndef MINGO_CONFIG_H_
#define MINGO_CONFIG_H_

#include <boost/lexical_cast.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>

namespace mingo {

//配置项基类
class ConfigVarBase
{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;

    ConfigVarBase(const std::string& name, const std::string& description)
        : m_name(name), m_description(description)
    {
    }
    virtual ~ConfigVarBase() {}

    const std::string& getName() const { return m_name;}
    const std::string& getDescription() const { return m_description;}
    // 将配置项的值转化为字符串
    virtual std::string toString() const = 0;
    // 通过字符串来设置配置项的值
    virtual bool fromString(const std::string& val) = 0;

protected:
    std::string m_name;    // 配置项的名称
    std::string m_description;  // 配置项的备注
};

template <class T>
class ConfigVar : public ConfigVarBase
{
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    
    ConfigVar(const std::string& name, const T& value, const std::string& description)
        : ConfigVarBase(name, description), m_value(value) {}

    T getValue() const
    {
        return m_value;
    }

    void setValue(const T value)
    {
        m_value = value;
    }

    std::string toString() const override
    {
        try {
           return boost::lexical_cast<std::string>(m_value);
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
            m_value = boost::lexical_cast<T>(val);
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

private:
    T m_value; // 配置项的值
};

class Config
{
public:
    typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    // 创建配置项
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name, const T& value, const std::string description = "")
    {
        auto iter = Lookup<T>(name);
        if (iter)
        {
            std::cout << "Lookup-name = " << name << "exits.";
            return iter;
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

private:
    static ConfigVarMap s_datas; // 配置项表
};
}

#endif
