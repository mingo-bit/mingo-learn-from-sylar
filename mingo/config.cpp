#include "config.h"
#include "log.h"

namespace mingo {

ConfigVarBase::ptr Config::LookupBase(const std::string& name) {
    auto it = get_datas().find(name);
    return it == get_datas().end() ? nullptr : it->second;
}

static void ListAllMember(const std::string& prefix, 
        const YAML::Node& node, std::list<std::pair<std::string, const YAML::Node> >& output)
{
    if (prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678") != std::string::npos)
    {
        std::cerr << "Config name invalid" << prefix << " : " << node.Scalar();
        return;
    }
    output.push_back(std::make_pair(prefix, node));
    if (node.IsMap()) {
        for (auto it = node.begin(); it !=  node.end(); ++it)
        {
            ListAllMember(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(), it->second, output);
        }
    }
}

void Config::Visit(std::function<void(ConfigVarBase::ptr)> cb) {
    RWLock::ReadLock lock(get_mutex());
    ConfigVarMap& m = get_datas();
    for (auto it = m.begin(); it != m.end(); ++it) {
        cb(it->second);
    }
}

void Config::LoadFromYaml(const YAML::Node& root) {
    std::list<std::pair<std::string, const YAML::Node> > all_nodes;
    ListAllMember("", root, all_nodes);

    for (auto& i : all_nodes) {
        std::string key = i.first;
        if (key.empty())
            continue;

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        ConfigVarBase::ptr var = mingo::Config::LookupBase(key);

        if (var) {
            if (i.second.IsScalar()) {
                var->fromString(i.second.Scalar());
            } else {
                std::stringstream ss;
                ss << i.second;
                var->fromString(ss.str());
            }
        }
    }
}

}
