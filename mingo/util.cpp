#include "util.h"

namespace mingo{

uint32_t getThreadId()
{
    return ::syscall(SYS_gettid);
}

uint32_t getFiberId()
{
    return 1;
}

}
