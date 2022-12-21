#include "mingo/log.h"
#include "mingo/util.h"
#include "mingo/macro.h"
#include "mingo/config.h"

void test_backtrace_assert() {
    MINGO_ASSERT2(1 == 0, "hhhhh");
}

int main(int argc, char *argv[])
{
    YAML::Node root = YAML::LoadFile("/home/mingo/workspace/mingo_server_copy_sylar/tests/configure/system.yml");
    mingo::Config::LoadFromYaml(root);
    test_backtrace_assert();
    return 0;
}
