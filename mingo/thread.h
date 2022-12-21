#ifndef MINGO_THREAD_H_
#define MINGO_THREAD_H_

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <string>
#include <semaphore.h>
#include <atomic>
#include "noncopyable.h"

namespace mingo {

class Semaphore : public Noncopyable {
public:
    explicit Semaphore(uint32_t count = 0);
    ~Semaphore();
    // -1，值为零时阻塞
    void wait();
    //  +1
    void notify();

private:
    sem_t m_semaphore;
};

template <typename T>
class ScopedLockImpl
{
public:
    explicit ScopedLockImpl(T& mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
        m_locked = true;
    }

    ~ScopedLockImpl() { unlock(); }

    void lock()
    {
        if (!m_locked)
        {
            m_mutex.lock();
            m_locked = true;
        }
    }

    void unlock()
    {
        if (m_locked)
        {
            m_locked = false;
            m_mutex.unlock();
        }
    }

private:
    T& m_mutex;
    bool m_locked;
};

template <typename T>
class ReadScopedLockImpl
{
public:
    explicit ReadScopedLockImpl(T& mutex)
        : m_mutex(mutex)
    {
        m_mutex.rdlock();
        m_locked = true;
    }

    ~ReadScopedLockImpl() { unlock(); }

    void lock()
    {
        if (!m_locked)
        {
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    void unlock()
    {
        if (m_locked)
        {
            m_locked = false;
            m_mutex.unlock();
        }
    }

private:
    T& m_mutex;
    bool m_locked;
};

template <typename T>
class WriteScopedLockImpl
{
public:
    explicit WriteScopedLockImpl(T& mutex)
        : m_mutex(mutex)
    {
        m_mutex.wrlock();
        m_locked = true;
    }

    ~WriteScopedLockImpl() { unlock(); }

    void lock()
    {
        if (!m_locked)
        {
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    void unlock()
    {
        if (m_locked)
        {
            m_locked = false;
            m_mutex.unlock();
        }
    }

private:
    T& m_mutex;
    bool m_locked;
};

class Mutex : public Noncopyable {
public:
    typedef ScopedLockImpl<Mutex> Lock;
    Mutex()
    {
        pthread_mutex_init(&m_mutex, nullptr);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    int lock()
    {
        return pthread_mutex_lock(&m_mutex);
    }

    int unlock()
    {
        return pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
};

//读写锁
class RWLock : public Noncopyable {
public:
    typedef WriteScopedLockImpl<RWLock> WriteLock;
    typedef ReadScopedLockImpl<RWLock> ReadLock;

    RWLock()
    {
        pthread_rwlock_init(&m_lock, nullptr);
    }

    ~RWLock()
    {
        pthread_rwlock_destroy(&m_lock);
    }

    int rdlock()
    {
        return pthread_rwlock_rdlock(&m_lock);
    }

    int wrlock()
    {
        return pthread_rwlock_wrlock(&m_lock);
    }

    int unlock()
    {
        return pthread_rwlock_unlock(&m_lock);
    }

private:
    pthread_rwlock_t m_lock;
};

//自旋锁
class Spinlock : Noncopyable {
public:
    /// 局部锁
    typedef ScopedLockImpl<Spinlock> Lock;

    Spinlock() {
        pthread_spin_init(&m_mutex, 0);
    }

    ~Spinlock() {
        pthread_spin_destroy(&m_mutex);
    }

    void lock() {
        pthread_spin_lock(&m_mutex);
    }

    void unlock() {
        pthread_spin_unlock(&m_mutex);
    }
private:
    /// 自旋锁
    pthread_spinlock_t m_mutex;
};

// 原子锁
class CASLock : Noncopyable {
public:
    // 局部锁
    typedef ScopedLockImpl<CASLock> Lock;

    CASLock() {
        m_mutex.clear();
    }

    ~CASLock() {
    }

    void lock() {
        while(std::atomic_flag_test_and_set_explicit(&m_mutex, std::memory_order_acquire));
    }

    void unlock() {
        std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
    }
private:
    // 原子状态
    volatile std::atomic_flag m_mutex;
};

//空锁
class NullMutex : public Noncopyable {
public:
    /// 局部锁
    typedef ScopedLockImpl<NullMutex> Lock;

    NullMutex() {}
    ~NullMutex() {}

    void lock() {}
    void unlock() {}
};

class NullRWMutex : public Noncopyable {
public:
    /// 局部读锁
    typedef ReadScopedLockImpl<NullMutex> ReadLock;
    /// 局部写锁
    typedef WriteScopedLockImpl<NullMutex> WriteLock;

    NullRWMutex() {}
    ~NullRWMutex() {}

    void rdlock() {}
    void wrlock() {}
    void unlock() {}
};

class Thread {
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()> cb, const std::string& name);
    ~Thread();


    pid_t getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    void join();

    static const std::string& GetName();
    static Thread* GetThis();
    static void SetName(const std::string& name); 

private:
    Thread(const Thread&) = delete;
    Thread(const Thread&&) = delete;
    Thread& operator=(const Thread&) = delete;

    static void* run(void* arg);
private:
    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;

    Semaphore m_semaphore;

};


}

#endif
