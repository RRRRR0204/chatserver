#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"

// 维护群组信息的操作接口方法
class GroupModel
{
public:
    // 创建群组
    bool createGroup(Group &group);

    // 加入群组
    void addGroup(int userid, int groupid, std::string role);

    // 查询用户所在群组
    std::vector<Group> queryGroups(int userid);

    // 根据指定的groupid查询群组内所有用户id，除了userid用户
    std::vector<int> queryGroupUsers(int userid, int groupid);
};

#endif