#include "mingo/env.h"
#include <unistd.h>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    std::cout << "argc=" << argc << std::endl;
    mingo::EnvMgr::getInstance()->addHelp("s", "start with the terminal");
    mingo::EnvMgr::getInstance()->addHelp("d", "run as daemon");
    mingo::EnvMgr::getInstance()->addHelp("p", "print help");
    if(!mingo::EnvMgr::getInstance()->init(argc, argv)) {
        mingo::EnvMgr::getInstance()->printHelp();
        return 0;
    }

    if(mingo::EnvMgr::getInstance()->has("p")) {
        mingo::EnvMgr::getInstance()->printHelp();
    }
    return 0;
}

