#include "model/user.hpp"
#include "model/group.hpp"
#include "public.hpp"
#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
using namespace std;

User g_currentUser;                   //< 记录当前登录的用户
vector<User> g_currentUserFreindList; //< 记录当前登录用户的好友列表
vector<Group> g_currentUserGroupList; //< 记录当前登录用户的群组列表

// 显示当前登录成功的用户基本信息
void showCurrentUserData();

// 接受（读）线程
void readTaskHandler(int cliendfd);

// 获取系统时间（聊天信息需要添加时间）
string getCurrentTime();

// 主聊天页面程序
void mainMenu();

// 聊天客户端实现，主线程用作发送（写）线程，子线程用作接受（读）线程
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "Invalid Command! Usage: ./ChatClient 127.0.0.1 6000" << endl;
        exit(-1);
    }

    // ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    // client的socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd)
    {
        cerr << "socket create error" << endl;
        exit(-1);
    }

    // 服务器信息
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof servAddr);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ip);
    servAddr.sin_port = htons(port);

    // client和server进行连接
    if (-1 == connect(clientfd, (sockaddr *)&servAddr, sizeof(sockaddr_in)))
    {
        cerr << "connect server error" << endl;
        exit(-1);
    }

    // main线程用于接受用户输入，发送（write）线程
    for (;;)
    {
        // 显示选项，登录、注册、退出
        cout << "====================" << endl;
        cout << "1. login" << endl;
        cout << "2. register" << endl;
        cout << "3. quit" << endl;
        cout << "====================" << endl;
        cout << "your choice:";
        int choice = 0;
        cin >> choice;
        cin.get(); // 读取缓冲区残留的回车

        switch (choice)
        {
        case 1: // login业务
        {
            int id = 0;
            char pwd[50] = {0};
            cout << "userid:";
            cin >> id;
            cin.get(); // 读取缓冲区残留的回车
            cout << "password:";
            cin.getline(pwd, 50);

            json js;
            js["msgid"] = LOGIN_MSG;
            js["id"] = id;
            js["password"] = pwd;
            string request = js.dump();

            int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (-1 == len)
            {
                cerr << "send login msg error:" << request << endl;
            }
            else
            {
                char buffer[1024] = {0};
                len = recv(clientfd, buffer, 1024, 0);
                if (-1 == len)
                {
                    cerr << "recv login response error" << endl;
                }
                else
                {
                    json responsejs = json::parse(buffer);
                    if (0 != responsejs["errno"].get<int>()) // 登录失败
                    {
                        cerr << responsejs["errmsg"] << endl;
                    }
                    else // 登录成功
                    {
                        // 记录当前用户的id和name
                        g_currentUser.setId(responsejs["id"].get<int>());
                        g_currentUser.setName(responsejs["name"]);

                        // 记录当前用户的好友列表信息
                        if (responsejs.contains("friends"))
                        {
                            vector<string> vec = responsejs["friends"];
                            for (string &str : vec)
                            {
                                json js = json::parse(str);
                                User user;
                                user.setId(js["id"].get<int>());
                                user.setName(js["name"]);
                                user.setState(js["state"]);
                                g_currentUserFreindList.push_back(user);
                            }
                        }

                        // 记录当前用户的群组列表信息
                        if (responsejs.contains("groups"))
                        {
                            vector<string> vec1 = responsejs["groups"];
                            for (string &grpstr : vec1)
                            {
                                json js = json::parse(grpstr);
                                Group group;
                                group.setId(js["id"].get<int>());
                                group.setName(js["name"]);
                                group.setDesc(js["desc"]);

                                vector<string> vec2 = js["users"];
                                for (string &usrstr : vec2)
                                {
                                    json userjs = json::parse(usrstr);
                                    GroupUser user;
                                    user.setId(userjs["id"].get<int>());
                                    user.setName(userjs["name"]);
                                    user.setState(userjs["state"]);
                                    user.setRole(userjs["role"]);
                                    group.getUsers().push_back(user);
                                }
                                g_currentUserGroupList.push_back(group);
                            }
                        }

                        // 显示登录用户的基本信息
                        showCurrentUserData();

                        // 显示当前用户的离线消息，个人聊天信息或群组信息
                        if (responsejs.contains("offlinemsg"))
                        {
                            vector<string> msgVec = responsejs["offlinemsg"];
                            for (string &str : msgVec)
                            {
                                json js = json::parse(str);
                                // time [id]name said: xxx
                                cout << js["time"] << " [" << js["id"] << "]" << js["name"]
                                     << "said: " << js["msg"] << endl;
                            }
                        }

                        // 登录成功，启动接受线程
                        std::thread readTask(readTaskHandler, clientfd);
                        readTask.detach();

                        // 进入聊天主菜单页面
                        mainMenu();
                    }
                }
            }
        }
        break;
        case 2: // register业务
        {
            char name[50] = {0};
            char pwd[50] = {0};
            cout << "username:";
            cin.getline(name, 50);
            cout << "password:";
            cin.getline(pwd, 50);

            json js;
            js["msgid"] = REG_MSG;
            js["name"] = name;
            js["password"] = pwd;
            string request = js.dump();

            int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (-1 == len)
            {
                cerr << "send reg msg error" << endl;
            }
            else
            {
                char buffer[1024] = {0};
                len = recv(clientfd, buffer, 1024, 0);
                if (-1 == len)
                {
                    cerr << "recv reg msg error" << endl;
                }
                else
                {
                    json responsejs = json::parse(buffer);
                    if (0 != responsejs["errno"].get<int>()) // 注册失败
                    {
                        cerr << "name is already exist, register error!" << endl;
                    }
                    else // 注册成功
                    {
                        cout << "name register success, userid is " << responsejs["id"]
                             << ", do not forget it!" << endl;
                    }
                }
            }
        }
        break;
        case 3: // quit
            close(clientfd);
            exit(0);
        default:
            cerr << "invalid input!" << endl;
            break;
        }
    }

    return 0;
}

// 显示当前登录成功的用户基本信息
void showCurrentUserData()
{
    cout << "======================login user======================" << endl;
    cout << "current login user => id:" << g_currentUser.getId() << " name:" << g_currentUser.getName() << endl;
    cout << "----------------------friend list---------------------" << endl;
    if (!g_currentUserFreindList.empty())
    {
        for (auto &user : g_currentUserFreindList)
        {
            cout << user.getId() << " " << user.getName() << " " << user.getState() << endl;
        }
    }
    cout << "---------------------group list-----------------------" << endl;
    if (!g_currentUserGroupList.empty())
    {
        for (auto &group : g_currentUserGroupList)
        {
            cout << group.getId() << " " << group.getName() << " " << group.getDesc() << endl;
            for (GroupUser &user : group.getUsers())
            {
                cout << " " << user.getId() << " " << user.getName() << " " << user.getState()
                     << " " << user.getRole() << endl;
            }
        }
    }
    cout << "======================================================" << endl;
}

// 接受（读）线程
void readTaskHandler(int cliendfd)
{

}

// 获取系统时间（聊天信息需要添加时间）
string getCurrentTime()
{

}

// 主聊天页面程序
void mainMenu()
{

}