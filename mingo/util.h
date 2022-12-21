#ifndef MINGO_UTIL_H
#define MINGO_UTIL_H

#include <pthread.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <execinfo.h>
#include "fiber.h"
#include <string>
#include <vector>
    
namespace mingo{

//获取线程id
uint32_t getThreadId();

//获取协程id
uint32_t getFiberId();
/*
 * @brief 获取当前的调用栈
 * @param[out] bt 保存调用栈
 * @param[in] size 最多返回层数
 * @param[in] skip 跳过栈顶的层数
 */
void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1);

/**
 * @brief 获取当前栈信息的字符串
 * @param[in] size 栈的最大层数
 * @param[in] skip 跳过栈顶的层数
 * @param[in] prefix 栈信息前输出的内容
 */
std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");

// 时间相关

uint64_t GetCurrentMS();
uint64_t GetCurrentUS();

std::string Time2Str(time_t ts = time(0), const std::string& format = "%Y-%m-%d %H:%M:%S");
time_t Str2Time(const char* str, const char* format = "%Y-%m-%d %H:%M:%S");

}

#endif

