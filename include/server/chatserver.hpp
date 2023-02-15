#ifndef CHATSERVER_H
#define CHARSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;
using namespace std::placeholders;

// 聊天服务器的主类
class ChatServer
{
public:
    // 初始化服务器对象
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);

    // 启动服务
    void start();

private:
    // 新连接事件的回调
    void onConnection(const TcpConnectionPtr &);

    // 读写事件的回调
    void onMessage(const TcpConnectionPtr &, Buffer *, Timestamp);

    TcpServer _server;
    EventLoop *_loop;
};

#endif