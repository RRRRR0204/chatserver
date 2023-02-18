#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

// 群组用户，继承User，多了一个role属性
class GroupUser : public User
{
public:
    void setRole(std::string role) { this->role = role; }
    std::string getRole() { return this->role; }

private:
    std::string role;
};

#endif