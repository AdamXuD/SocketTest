#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include "include/json/json.h"

#define PACKET_HEADER "PACKET_HEADER"
#define PACKET_END "PACKET_END"
#define PACKET_HEADER_SIZE sizeof(PACKET_HEADER)
#define PACKET_END_SIZE sizeof(PACKET_END)
#define SIZE_LENGTH 8

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

    const Msg & operator=(const Msg &msg)
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

    void mapToContent(string column[2], map<string, string> &pMap)
    {
        Json::Value root;
        map<string, string>::iterator it = pMap.begin();
        for(int i = 0; i < pMap.size(); i++, it++)
        {
            root[i][column[0]] = Json::Value(it->first);
            root[i][column[1]] = Json::Value(it->second);
        }
        this->content = root.toStyledString();
    }

    void contentToMap(string column[2], map<string,  string> &pMap)
    {
        istringstream iss(this->content);
        Json::CharReaderBuilder rbuilder;
        string err;
        Json::Value root;
        Json::parseFromStream(rbuilder, iss, &root, &err);
        for(int i = 0; i < root.size(); i++)
        {
            cout << root[i][column[0]].asString() << root[i][column[1]].asString() << endl;
            pMap[root[i][column[0]].asString()] = root[i][column[1]].asString();
        }
    }

    
};

string packUp(string jsonString)
{
    char Csz[9];
    sprintf(Csz, "%08d", (int)jsonString.size());
    return PACKET_HEADER + string(Csz) + jsonString + PACKET_END; 
}

string unPack(string packet)
{
    if(packet.find(PACKET_HEADER) != string::npos)
    {
        if(packet.find(PACKET_END) != string::npos)
        {
            return packet.substr(PACKET_HEADER_SIZE + SIZE_LENGTH - 1, atoi(packet.substr(PACKET_HEADER_SIZE - 1, SIZE_LENGTH).c_str()));
        }
    }
    return Msg().serializer();
}


int main()
{
    // Msg test1(2, "儿子", "爸爸", "我是你爸爸！");
    // Msg test2;
    // test2.parser(unPack(packUp(test1.serializer())));

    // cout << test2.type << endl;
    // cout << test2.fromUser << endl;
    // cout << test2.toUser << endl;
    // cout << test2.content << endl;

    // Json::Value root;
    // Json::Value element;
    // element["aaa"] = Json::Value("bbb");
    // element["ccc"] = Json::Value("ddd");
    // root["test"][0] = Json::Value(element);
    // root["test"][1] = Json::Value("aaa");
    // root["test"][2] = Json::Value("bbb");
    // cout << root.toStyledString() << endl;

    map<string, string> test;
    test["aaa"] = "bbb";
    test["ccc"] = "ddd";

    Msg msg;
    string column[2] = {"111", "222"};
    msg.mapToContent(column, test);
    cout << msg.content << endl;

    Msg msg2;
    msg2.parser(unPack(packUp(msg.serializer())));

    map<string, string> test2;
    msg2.contentToMap(column, test2);
    map<string, string>::iterator it = test2.begin();
    for(int i = 0; i < test2.size(); i++, it++)
    {
        cout << it->first << " " << it->second << endl;
    }
    return 0;
}