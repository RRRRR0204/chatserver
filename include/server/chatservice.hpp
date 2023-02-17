#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include "json.hpp"
#include "model/usermodel.hpp"
using json = nlohmann::json;

#include <functional>
#include <unordered_map>
#include <muduo/net/TcpConnection.h>
#include <mutex>
using namespace muduo;
using namespace muduo::net;
using namespace std::placeholders;

// 处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

// 聊天服务器业务类，单例模式
class ChatService
{
public:
    // 获取单例对象
    static ChatService *instance();

    // 处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);

    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);

private:
    ChatService();

    std::unordered_map<int, MsgHandler> _msgHandlerMap;

    // 存储在线用户的通信连接
    std::unordered_map<int, TcpConnectionPtr> _userConnMap;
    // 定义互斥锁，保证_userConnMap的线程安全
    std::mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;
};

#endif