#ifndef MINGO_ENDIAN_H
#define MINGO_ENDIAN_H

#define MINGO_LITTLE_ENDIAN 1
#define MINGO_BIG_ENDIAN 2

#include <byteswap.h>
#include <stdint.h>

namespace mingo {

// 8字节类型的字节序转化
template<class T>
typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
byteswap(T value) {
    return (T)bswap_64((uint64_t)value);
}

// 4字节类型的字节序转化
template<class T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t), T>::type
byteswap(T value) {
    return (T)bswap_32((uint32_t)value);
}

// 2字节类型的字节序转化
template<class T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t), T>::type
byteswap(T value) {
    return (T)bswap_16((uint16_t)value);
}

#if BYTE_ORDER == BIG_ENDIAN
#define MINGO_BYTE_ORDER MINGO_BIG_ENDIAN
#else
#define MINGO_BYTE_ORDER MINGO_LITTLE_ENDIAN
#endif

#if MINGO_BYTE_ORDER == MINGO_BIG_ENDIAN

// 只在小端机器上执行byteswap, 在大端机器上什么都不做
template<class T>
T byteswapOnLittleEndian(T t) {
    return t;
}

// 只在大端机器上执行byteswap, 在小端机器上什么都不做
template<class T>
T byteswapOnBigEndian(T t) {
    return byteswap(t);
}
#else

// 只在小端机器上执行byteswap, 在大端机器上什么都不做
template<class T>
T byteswapOnLittleEndian(T t) {
    return byteswap(t);
}

 // 只在大端机器上执行byteswap, 在小端机器上什么都不做
template<class T>
T byteswapOnBigEndian(T t) {
    return t;
}
#endif

}

#endif
