#include "mingo/bytearray.h"
#include "mingo/log.h"
#include "mingo/macro.h"

static mingo::Logger::ptr g_logger = GET_GLOBAL_LOGGER();
void test() {
#define XX(type, len, write_fun, read_fun, base_len) {\
    std::vector<type> vec; \
    for(int i = 0; i < len; ++i) { \
        vec.push_back(rand()); \
    } \
    mingo::ByteArray::ptr ba(new mingo::ByteArray(base_len)); \
    for(auto& i : vec) { \
        ba->write_fun(i); \
    } \
    ba->setPosition(0); \
    for(size_t i = 0; i < vec.size(); ++i) { \
        type v = ba->read_fun(); \
		std::cout << i << "-" << v << "-" << vec[i] << std::endl; \
        MINGO_ASSERT(v == vec[i]); \
    } \
    MINGO_ASSERT(ba->getReadSize() == 0); \
    LOG_INFO(g_logger, #write_fun "/" #read_fun \
                    " (" #type " ) len=" + std::to_string(len) \
                    + " base_len=" + std::to_string(base_len) \
                    + " size=" + std::to_string(ba->getSize())); \
}

    XX(int8_t,  100, writeFint8, readFint8, 1);
    XX(uint8_t, 100, writeFuint8, readFuint8, 1);
    XX(int16_t,  100, writeFint16,  readFint16, 1);
    XX(uint16_t, 100, writeFuint16, readFuint16, 1);
    XX(int32_t,  100, writeFint32,  readFint32, 1);
    XX(uint32_t, 100, writeFuint32, readFuint32, 1);
    XX(int64_t,  100, writeFint64,  readFint64, 1);
    XX(uint64_t, 100, writeFuint64, readFuint64, 1);

    XX(int32_t,  100, writeInt32,  readInt32, 1);
    XX(uint32_t, 100, writeUint32, readUint32, 1);
    XX(int64_t,  100, writeInt64,  readInt64, 1);
    XX(uint64_t, 100, writeUint64, readUint64, 1);
#undef XX

#define XX(type, len, write_fun, read_fun, base_len) {\
    std::vector<type> vec; \
    for(int i = 0; i < len; ++i) { \
        vec.push_back(rand()); \
    } \
    mingo::ByteArray::ptr ba(new mingo::ByteArray(base_len)); \
    for(auto& i : vec) { \
        ba->write_fun(i); \
    } \
    ba->setPosition(0); \
    for(size_t i = 0; i < vec.size(); ++i) { \
        type v = ba->read_fun(); \
        MINGO_ASSERT(v == vec[i]); \
    } \
    MINGO_ASSERT(ba->getReadSize() == 0); \
    LOG_INFO(g_logger, #write_fun "/" #read_fun \
                    " (" #type " ) len=" + std::to_string(len) \
                    + " base_len=" + std::to_string(base_len) \
                    + " size=" + std::to_string(ba->getSize())); \
    ba->setPosition(0); \
    MINGO_ASSERT(ba->writeToFile("/home/mingo/workspace/mingo_server_copy_sylar/tests/tests/" #type "_" #len "-" #read_fun ".dat")); \
    mingo::ByteArray::ptr ba2(new mingo::ByteArray(base_len * 2)); \
    MINGO_ASSERT(ba2->readFromFile("/home/mingo/workspace/mingo_server_copy_sylar/tests/tests/" #type "_" #len "-" #read_fun ".dat")); \
    ba2->setPosition(0); \
    MINGO_ASSERT(ba->toString() == ba2->toString()); \
    MINGO_ASSERT(ba->getPosition() == 0); \
    MINGO_ASSERT(ba2->getPosition() == 0); \
}
    XX(int8_t,  100, writeFint8, readFint8, 1);
    XX(uint8_t, 100, writeFuint8, readFuint8, 1);
    XX(int16_t,  100, writeFint16,  readFint16, 1);
    XX(uint16_t, 100, writeFuint16, readFuint16, 1);
    XX(int32_t,  100, writeFint32,  readFint32, 1);
    XX(uint32_t, 100, writeFuint32, readFuint32, 1);
    XX(int64_t,  100, writeFint64,  readFint64, 1);
    XX(uint64_t, 100, writeFuint64, readFuint64, 1);

    XX(int32_t,  100, writeInt32,  readInt32, 1);
    XX(uint32_t, 100, writeUint32, readUint32, 1);
    XX(int64_t,  100, writeInt64,  readInt64, 1);
    XX(uint64_t, 100, writeUint64, readUint64, 1);

#undef XX
}

int main(int argc, char** argv) {
    test();
    return 0;
}

