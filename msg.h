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
#define STRING_EMPTY ""

struct BaseMsg //信息类
{
    int type;             //消息类型
    std::string toUser;   //目标账号
    std::string fromUser; //用户来源
    std::string content;  //内容

    BaseMsg(int type = 0, std::string fromUser = STRING_EMPTY, std::string toUser = STRING_EMPTY, std::string content = STRING_EMPTY);
    const BaseMsg &operator=(const BaseMsg &msg);
};

struct History : BaseMsg
{
    std::string timeStamp;
    History(int type = 0, std::string fromUser = STRING_EMPTY, std::string toUser = STRING_EMPTY, std::string content = STRING_EMPTY, std::string timeStamp = STRING_EMPTY);
};

struct Query : BaseMsg
{
    int id;
    Query(int type = 0, int id = -1, std::string fromUser = STRING_EMPTY, std::string toUser = STRING_EMPTY, std::string content = STRING_EMPTY);

    std::string serializer(); //利用jsoncpp库来实现序列化与反序列化 以及实现不定长字符串的发送
    void parser(std::string jsonString);
};

struct Msg : BaseMsg
{
    Msg(int type = 0, std::string fromUser = STRING_EMPTY, std::string toUser = STRING_EMPTY, std::string content = STRING_EMPTY);

    std::string serializer(); //利用jsoncpp库来实现序列化与反序列化 以及实现不定长字符串的发送
    void parser(std::string jsonString);

    void mapToContent(std::string column[2], std::map<std::string, std::string> &pMap);
    void contentToMap(std::string column[2], std::map<std::string, std::string> &pMap);

    void historyToContent(std::list<History *> &pList);
    void contentToHistory(std::list<History *> &pList);

    void queryListToContent(std::list<Query *> &pList);
    void contentToQueryList(std::list<Query *> &pList);
};


std::string packUp(std::string jsonString);
std::string unPack(std::string packet);