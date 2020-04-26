/*
应用缓冲区，参考了muduo
*/
#ifndef IO_BUFFER_H
#define IO_BUFFER_H

#include <string.h>
#include <arpa/inet.h>
#include <endian.h>
#include <algorithm>
#include <vector>

class IOBuffer
{
public:
    // 缓冲区头部预留大小
    static const size_t PREPEND_SIZE = 8;
    // 初始大小
    static const size_t INITIAL_SIZE = 1024;
    // 回车换行符，标识一行结束
    static const char   CRLF[];

    explicit IOBuffer(size_t initial_size = INITIAL_SIZE)
        : buffer_(PREPEND_SIZE + INITIAL_SIZE),
          read_index_(PREPEND_SIZE),
          write_index_(PREPEND_SIZE)
    {}

    // 交换缓冲区，效率较高
    void Swap(IOBuffer& rhs)
    {
        buffer_.swap(rhs.buffer_);
        std::swap(read_index_, rhs.read_index_);
        std::swap(write_index_, rhs.write_index_);
    }

    // 获取可读、可写、预留区的大小
    size_t GetReadableSize() const { return write_index_ - read_index_; }
    size_t GetWritableSize() const { return buffer_.size() - write_index_; }
    size_t GetPrependSize() const { return read_index_; }

    // 获取可读、可写区的首指针
    const char* GetReadablePtr() const { return GetBegin() + read_index_; }
    const char* GetWritablePtr() const { return GetBegin() + write_index_; }
          char* GetWritablePtr() { return GetBegin() + write_index_; }

    const char* FindCRLF() const 
    {
        const char* crlf = std::search(GetReadablePtr(), GetWritablePtr(), CRLF, CRLF + 2);
        return crlf == GetWritablePtr() ? nullptr : crlf;
    }

    const char* FindCRLF(const char* start_ptr) const 
    {
        const char* crlf = std::search(start_ptr, GetWritablePtr(), CRLF, CRLF + 2);
        return crlf == GetWritablePtr() ? nullptr : crlf;
    }

    const char* FindEOL() const
    {
        const void* eol = memchr(GetReadablePtr(), '\n', GetReadableSize());
        return static_cast<const char*>(eol);
    }

    const char* FindEOL(const char* start_ptr) const
    {
        const void* eol = memchr(start_ptr, '\n', GetWritablePtr() - start_ptr);
        return static_cast<const char*>(eol);
    }

    // 更新read_index到实际位置
    void Retrieve(size_t len)
    {
        if (len < GetReadableSize())
        {
            read_index_ += len;
        }
        else // len == GetReadableSize()
        {
            RetrieveAll();
        }
    }

    void RetrieveUntil(const char* end)
    {
        Retrieve(end - GetReadablePtr());
    }

    void RetrieveAll()
    {
        read_index_ = PREPEND_SIZE;
        write_index_ = PREPEND_SIZE;
    }

    std::string RetrieveAllAsString()
    {
        return RetrieveAsString(GetReadableSize());
    }

    std::string RetrieveAsString(size_t len)
    {
        std::string result(GetReadablePtr(), len);
        Retrieve(len);
        return result;
    }

    
    // 确保有足够空间可写
    void EnsureWritable(size_t len)
    {
        if (GetWritableSize() < len)
        {
            MakeSpace(len);
        }
    }

    // 添加数据进缓冲区
    void Append(const char* data, size_t len)
    {
        if (GetWritableSize() < len)
        {
            MakeSpace(len);
        }
        std::copy(data, data + len, GetWritablePtr());
        write_index_ += len;
    }

    void Append(const void* data, size_t len)
    {
        Append(static_cast<const char*>(data), len);
    }

    void Append(const std::string& str)
    {
        Append(str.data(), str.size());
    }

    

    // 添加数据进预留区
    void Prepend(const void* data, size_t len)
    {
        read_index_ -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d, d + len, GetBegin() + read_index_);
    }

    

    // 收缩缓冲区大小
    void Shrink(size_t reserve)
    {
        IOBuffer other;
        other.EnsureWritable(GetReadableSize() + reserve);
        other.Append(GetReadablePtr(), GetReadableSize());
        Swap(other);
    }

    size_t GetCapacity() const
    {
        return buffer_.capacity();
    }

    // 读取数据进缓冲区
    ssize_t ReadFd(int fd, int* saved_errno);
    //ssize_t ReadFdRepeatedly(int fd, int* saved_errno);
private:
    // 取缓冲区起始位置指针
    char* GetBegin() { return &*buffer_.begin(); }
    const char* GetBegin() const { return &*buffer_.begin(); }

    // 对缓冲区操作使得有足够空间
    void MakeSpace(size_t len)
    {
        if (GetWritableSize() + GetPrependSize() < len + PREPEND_SIZE)
        {
            // 直接开辟新空间
            buffer_.resize(write_index_ + len);
        }
        else
        {
            // 通过数据腾挪以留出足够可写空间
            size_t readable = GetReadableSize();
            std::copy(GetBegin() + read_index_, GetBegin() + write_index_, GetBegin() + PREPEND_SIZE);
            read_index_ = PREPEND_SIZE;
            write_index_ = read_index_ + readable;
        }
    }

    // 存放数据的缓冲区
    std::vector<char> buffer_;
    // 可读位置
    size_t read_index_;
    // 可写位置
    size_t write_index_;
};

#endif // IO_BUFFER_H