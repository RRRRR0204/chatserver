#ifndef REDIS_H
#define REDIS_H

#include <hiredis/hiredis.h>
#include <thread>
#include <functional>

class Redis
{
public:
    Redis();
    ~Redis();

    // 连接redis服务器
    bool connect();

    // 向reids指定通道发布消息
    bool publish(int channel, std::string message);

    // 向redis指定通道订阅消息
    bool subscribe(int channel);

    // 向redis指定通道取消订阅
    bool unsubscribe(int channel);

    // 在独立的线程中接收订阅通道中的消息
    void observer_channel_message();

    // 初始化向业务层上报通道消息的回调对象
    void init_notify_handler(std::function<void(int, std::string)> fn);

private:
    // hiredis同步上下文对象（redis-cli），负责publish消息
    redisContext *_publish_context;

    // hiredis同步上下文对象（redis-cli），负责subscribe消息
    redisContext *_subscribe_context;

    // 回调操作，收到订阅消息后给service层上报
    std::function<void(int, std::string)> _notify_message_handler;
};

#endif