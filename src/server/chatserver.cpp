#include "chatserver.hpp"

#include <functional>

ChatServer::ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg)
    : _server(loop, listenAddr, nameArg)
    , _loop(loop)
{
    // 注册新连接事件回调
    _server.setConnectionCallback(
        std::bind(&ChatServer::onConnection, this, std::placeholders::_1)
    );
    // 注册消息回调
    _server.setMessageCallback(
        std::bind(&ChatServer::onMessage, this, std::placeholders::_1, 
                                std::placeholders::_2, std::placeholders::_3)
    );

    // 设置线程数量
    _server.setThreadNum(2);
}

// 启动服务
void ChatServer::start()
{
    _server.start();
}

// 上报新连接的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &)
{

}

// 上报读写事件的回调
void ChatServer::onMessage(const TcpConnectionPtr &, Buffer *, Timestamp)
{
    
}
