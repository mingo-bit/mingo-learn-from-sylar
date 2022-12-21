#ifndef MINGO_BYTEARRAY_H
#define MINGO_BYTEARRAY_H

#include <memory>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>

namespace mingo {

class ByteArray {
public:
    typedef std::shared_ptr<ByteArray> ptr;

    struct Node {
        Node(size_t s);

        Node();

        ~Node();

        char* ptr;
        Node* next;
        size_t size;
    };

    ByteArray(size_t base_size = 4096);

    ~ByteArray();

	// write
    void writeFint8  (int8_t value);
    void writeFuint8 (uint8_t value);
    void writeFint16 (int16_t value);
    void writeFuint16(uint16_t value);

    void writeFint32 (int32_t value);

    void writeFuint32(uint32_t value);

    void writeFint64 (int64_t value);

    void writeFuint64(uint64_t value);

    void writeInt32  (int32_t value);
    void writeUint32 (uint32_t value);

    void writeInt64  (int64_t value);

     // 写入无符号Varint64类型的数据
    void writeUint64 (uint64_t value);

    void writeFloat  (float value);

    void writeDouble (double value);

    void writeStringF16(const std::string& value);

    void writeStringF32(const std::string& value);

    void writeStringF64(const std::string& value);

     // 写入std::string类型的数据,用无符号Varint64作为长度类型
    void writeStringVint(const std::string& value);

     // 写入std::string类型的数据,无长度
    void writeStringWithoutLength(const std::string& value);

	// read
    int8_t   readFint8();

    uint8_t  readFuint8();

    int16_t  readFint16();

    uint16_t readFuint16();

	int32_t readFint32();

    uint32_t readFuint32();

    int64_t  readFint64();

    uint64_t readFuint64();

    int32_t  readInt32();

    // 读取无符号Varint32类型的数据
    uint32_t readUint32();

    // 读取有符号Varint64类型的数据
    int64_t  readInt64();

    uint64_t readUint64();

    float    readFloat();

    double   readDouble();

    std::string readStringF16();

    std::string readStringF32();

    std::string readStringF64();

    std::string readStringVint();

     // 清空ByteArray
    void clear();

    /**
     * @brief 写入size长度的数据
     * @param[in] buf 内存缓存指针
     * @param[in] size 数据大小
     * @post m_position += size, 如果m_position > m_size 则 m_size = m_position
     */
    void write(const void* buf, size_t size);

    /**
     * @brief 读取size长度的数据
     * @param[out] buf 内存缓存指针
     * @param[in] size 数据大小
     * @post m_position += size, 如果m_position > m_size 则 m_size = m_position
     * @exception 如果getReadSize() < size 则抛出 std::out_of_range
     */
    void read(void* buf, size_t size);

    /**
     * @brief 读取size长度的数据
     * @param[out] buf 内存缓存指针
     * @param[in] size 数据大小
     * @param[in] position 读取开始位置
     * @exception 如果 (m_size - position) < size 则抛出 std::out_of_range
     */
    void read(void* buf, size_t size, size_t position) const;

     // 返回ByteArray当前位置
    size_t getPosition() const { return m_position;}

    // 设置ByteArray当前位置
    void setPosition(size_t v);

     // 把ByteArray的数据写入到文件中
    bool writeToFile(const std::string& name) const;

     // 从文件中读取数据
    bool readFromFile(const std::string& name);

     // 返回内存块的大小
    size_t getBaseSize() const { return m_baseSize;}

     // 返回可读取数据大小
    size_t getReadSize() const { return m_size - m_position;}

     // 是否是小端
    bool isLittleEndian() const;

     // 设置是否为小端
    void setIsLittleEndian(bool val);

    // 将ByteArray里面的数据[m_position, m_size)转成std::string
    std::string toString() const;

    // 将ByteArray里面的数据[m_position, m_size)转成16进制的std::string(格式:FF FF FF)
    std::string toHexString() const;

    /**
     * @brief 获取可读取的缓存,保存成iovec数组
     * @param[out] buffers 保存可读取数据的iovec数组
     * @param[in] len 读取数据的长度,如果len > getReadSize() 则 len = getReadSize()
     * @return 返回实际数据的长度
     */
    uint64_t getReadBuffers(std::vector<iovec>& buffers, uint64_t len = ~0ull) const;

    /**
     * @brief 获取可读取的缓存,保存成iovec数组,从position位置开始
     * @param[out] buffers 保存可读取数据的iovec数组
     * @param[in] len 读取数据的长度,如果len > getReadSizek() 则 len = getReadSize()
     * @param[in] position 读取数据的位置
     * @return 返回实际数据的长度
     */
    uint64_t getReadBuffers(std::vector<iovec>& buffers, uint64_t len, uint64_t position) const;

    /**
     * @brief 获取可写入的缓存,保存成iovec数组
     * @param[out] buffers 保存可写入的内存的iovec数组
     * @param[in] len 写入的长度
     * @return 返回实际的长度
     * @post 如果(m_position + len) > m_capacity 则 m_capacity扩容N个节k点以容纳len长度
     */
    uint64_t getWriteBuffers(std::vector<iovec>& buffers, uint64_t len);

     // 返回数据的长度
    size_t getSize() const { return m_size;}
private:
    
     // 扩容ByteArray,使其可以容纳size个数据(如果原本可以可以容纳,则不扩容)
    void addCapacity(size_t size);

    // 获取当前的可写入容量
    size_t getCapacity() const { return m_capacity - m_position;}
private:
    /// 内存块的大小
    size_t m_baseSize;
    /// 当前操作位置
    size_t m_position;
    /// 当前的总容量
    size_t m_capacity;
    /// 当前数据的大小
    size_t m_size;
    /// 字节序,默认大端
    int8_t m_endian;
    /// 第一个内存块指针
    Node* m_root;
    /// 当前操作的内存块指针
    Node* m_cur;
};


}

#endif
