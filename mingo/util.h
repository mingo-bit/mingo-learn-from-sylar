#include <pthread.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace mingo{

//获取线程id
uint32_t getThreadId();

//获取协程id
uint32_t getFiberId();

}

