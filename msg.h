#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <list>
#include "include/json/json.h"

#define PACKET_HEADER "PACKET_HEADER"
#define PACKET_END "PACKET_END"
#define PACKET_HEADER_SIZE sizeof(PACKET_HEADER)
#define PACKET_END_SIZE sizeof(PACKET_END)
#define SIZE_LENGTH 8

struct BaseMsg //信息类
{
    int type;        //消息类型
    std::string toUser;   //目标账号
    std::string fromUser; //用户来源
    std::string content;  //内容

    BaseMsg(int type = 0, std::string toUser = "", std::string fromUser = "", std::string content = "");
    const BaseMsg &operator=(const BaseMsg &msg);
};

struct History : BaseMsg
{
    std::string timeStamp;
    History(int type = 0, std::string toUser = "", std::string fromUser = "", std::string content = "", std::string timeStamp = "");
};

struct Msg : BaseMsg
{
    Msg(int type = 0, std::string toUser = "", std::string fromUser = "", std::string content = "");


    std::string serializer(); //利用jsoncpp库来实现序列化与反序列化 以及实现不定长字符串的发送
    void parser(std::string jsonString);

    void mapToContent(std::string column[2], std::map<std::string, std::string> &pMap);
    void contentToMap(std::string column[2], std::map<std::string, std::string> &pMap);

    void historyToContent(std::list<History *> &pList);
    void contentToHistory(std::list<History *> &pList);
};

std::string packUp(std::string jsonString);
std::string unPack(std::string packet);