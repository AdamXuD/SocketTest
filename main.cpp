#include <iostream>
#include <cstring>
#include "include/json/json.h"

using namespace std;

struct Msg //信息类
{
    int type;           //消息类型
    string toUser;    //目标账号
    string fromUser;  //用户来源
    string content; //内容

    Msg(int type = 0, string toUser = "", string fromUser = "", string content = "")
    {
        this->type = type;
        this->toUser = toUser;
        this->fromUser = fromUser;
        this->content = content;
    }

    Msg & operator=(const Msg &msg)
    {
        this->type = msg.type;
        this->fromUser = msg.fromUser;
        this->toUser = msg.toUser;
        this->content = msg.content;
        return *this;
    }

    string serializer() //利用jsoncpp库来实现序列化与反序列化 以及实现不定长字符串的发送
    {
        Json::Value root;
        root["type"] = Json::Value(this->type);
        root["toUser"] = Json::Value(this->toUser);
        root["fromUser"] = Json::Value(this->fromUser);
        root["content"] = Json::Value(this->content);
        return root.toStyledString();
    }

    void parser(string jsonString)
    {
        Json::CharReaderBuilder rbuilder;
        istringstream iss(jsonString);
        string err;
        Json::Value root;
        Json::parseFromStream(rbuilder, iss, &root, &err);
        this->type = root["type"].asInt();
        this->toUser = root["toUser"].asString();
        this->fromUser = root["fromUser"].asString();
        this->content = root["content"].asString();
    }
};

void packUp()
{
    string packet;
    
}


int main()
{
    Msg test1(2, "儿子", "爸爸", "我是你爸爸！");
    Msg test2;
    test2.parser(test1.serializer());
    Msg test3 = test2;
    cout << test3.content << endl;
    return 0;
}