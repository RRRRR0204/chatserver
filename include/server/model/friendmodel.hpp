#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "user.hpp"

#include <vector>

// 维护好友信息的接口方法
class FriendMdodel
{
public:
    // 添加好友关系
    void insert(int userid, int friendid);

    // 返回好友列表
    std::vector<User> query(int userid);
};

#endif