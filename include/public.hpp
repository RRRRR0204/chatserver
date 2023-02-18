#ifndef PUBLIC_H
#define PUBLIC_H

/**
 * 公共文件
 */

enum EnMsgType
{
    LOGIN_MSG = 1,    //< 登录消息 1
    LOGIN_MSG_ACK,    //< 登录响应消息 2
    REG_MSG,          //< 注册消息 3
    REG_MSG_ACK,      //< 注册相应消息 4
    ONE_CHAT_MSG,     //< 点对点聊天消息 5
    ADD_FRIEND_MSG,   //< 添加好友消息 6
    CREATE_GROUP_MSG, //< 创建群组消息 7
    ADD_GROUP_MSG,    //< 加入群组消息 8
    GROUP_CHAT_MSG    //< 群组聊天消息 9
};

#endif