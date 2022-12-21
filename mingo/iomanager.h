#ifndef MINGO_IOMANAGER_H_
#define MINGO_IOMANAGER_H_

#include "scheduler.h"
#include "thread.h"
#include "timer.h"

namespace mingo {

// 基于Epoll的IO协程调度器
class IOManager : public Scheduler, public TimerManager{
public:
    typedef std::shared_ptr<IOManager> ptr;
    typedef RWLock RWMutexType;

     // IO事件
    enum Event {
        /// 无事件
        NONE    = 0x0,
        /// 读事件(EPOLLIN)
        READ    = 0x1,
        /// 写事件(EPOLLOUT)
        WRITE   = 0x4,
    };
private:
     // Socket事件上下文类
    struct FdContext {
        typedef Mutex MutexType;
        
         // 事件上线文类
        struct EventContext {
            /// 事件执行的调度器
            Scheduler* scheduler = nullptr;
            /// 事件协程
            Fiber::ptr fiber;
            /// 事件的回调函数
            std::function<void()> cb;
        };

         // 获取事件上下文类
         // event 事件类型
         // 返回对应事件的上下文
        EventContext& getContext(Event event);

         // 重置事件上下文
         // ctx 待重置的上下文类
        void resetContext(EventContext& ctx);

         // 触发事件
         // event 事件类型
        void triggerEvent(Event event);

        /// 读事件上下文
        EventContext read;
        /// 写事件上下文
        EventContext write;
        /// 事件关联的句柄
        int fd = 0;
        /// 当前的事件
        Event events = NONE;
        /// 事件的Mutex
        MutexType mutex;
    };

public:
     // threads 线程数量
     // use_caller 是否将调用线程包含进去
     // name 调度器的名称
    IOManager(size_t threads = 1, bool use_caller = true, const std::string& name = "");

    ~IOManager();

     // 添加事件
     // fd socket句柄
     // event 事件类型
     // cb 事件回调函数
     // 添加成功返回0,失败返回-1
    int addEvent(int fd, Event event, std::function<void()> cb = nullptr);

     // 删除事件
     // fd socket句柄
     // event 事件类型
     // attention 不会触发事件
    bool delEvent(int fd, Event event);

     // 取消事件
     // fd socket句柄
     // event 事件类型
     // attention 如果事件存在则触发事件
    bool cancelEvent(int fd, Event event);

     // 取消所有事件
     // fd socket句柄
    bool cancelAll(int fd);

     // 返回当前的IOManager
    static IOManager* GetThis();
protected:
    void tickle() override;
    bool stopping() override;
    void idle() override;
    
    void onTimerInsertedAtFront() override;

     // 重置socket句柄上下文的容器大小
     // size 容量大小
    void contextResize(size_t size);

     // 判断是否可以停止
     // timeout 最近要出发的定时器事件间隔
     //  返回是否可以停止
    bool stopping(uint64_t& timeout);
private:
    /// epoll 文件句柄
    int m_epfd = 0;
    /// pipe 文件句柄
    int m_tickleFds[2];
    /// 当前等待执行的事件数量
    std::atomic<size_t> m_pendingEventCount = {0};
    /// IOManager的Mutex
    RWMutexType m_mutex;
    /// socket事件上下文的容器
    std::vector<FdContext*> m_fdContexts;
};

}


#endif
