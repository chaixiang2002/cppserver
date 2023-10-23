#include "ThreadPool.h"

using  std::thread;
using  std::function;
using  std::unique_lock;
using  std::mutex;

ThreadPool::ThreadPool(unsigned int size)
{
    for (unsigned i = 0; i < size; i++)
    {
        workers.emplace_back(thread([this](){
            while (true)
            {
                // 1. 定义一个 任务A
                function<void()> A;
                // 2. 在拿到锁的情况下 给任务A赋值
                {
                    // 申请一个unique_lock锁
                    unique_lock<mutex> uqlock(tasks_mtx);
                    // 条件变量cv等待通知，释放uqlock??????
                    /**
                     * 这里的条件是stop为true或者任务队列tasks不为空。
                     * 如果满足这个条件，cv.wait()函数会继续执行后面的代码。
                     * 如果不满足条件，cv.wait()函数会阻塞当前线程，
                     * 并且自动释放互斥锁uqlock，直到其他线程调用notify系列函数来唤醒当前线程。
                    */
                    cv.wait(uqlock,[this](){
                        return stop || !tasks.empty();//stop为true）或者任务队列不为空。cv.wait()会自动释放锁
                    });
                    if(stop && tasks.empty()) return;
                    A=tasks.front();
                    tasks.pop();
                }
                // 执行 任务A
                A();
            }
        }));
    }
    
}

ThreadPool::~ThreadPool()
{
    // 获得锁并使stop=true
    {
        unique_lock<mutex> uplock(tasks_mtx);
        stop=true;
    }
    cv.notify_all();
    /**
     * 使用一个循环遍历线程池中的所有线程，并调用t.join()函数等待线程执行完毕。
     * 如果线程是可加入的（joinable），则会等待线程执行完毕后再继续执行析构函数的后续代码。
        这段代码的作用是停止线程池的运行，并等待所有线程执行完毕后再销毁线程池。
    */
    for (thread &t :workers)
    {
        if(t.joinable())
            t.join();
    }
    
}
// void ThreadPool::add(std::function<void()> func){
//     {
//         unique_lock<mutex> uqlock(tasks_mtx);
//         if (stop)
//             throw std::runtime_error("ThreadPool already stop, can't add task any more");
//         tasks.emplace(func);
//     }
//     cv.notify_one();
// }