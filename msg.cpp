#include "msg.h"

BaseMsg::BaseMsg(int type, std::string fromUser, std::string toUser, std::string content)
{
    this->type = type;
    this->toUser = toUser;
    this->fromUser = fromUser;
    this->content = content;
}

const BaseMsg &BaseMsg::operator=(const BaseMsg &msg)
{
    this->type = msg.type;
    this->fromUser = msg.fromUser;
    this->toUser = msg.toUser;
    this->content = msg.content;
    return *this;
}

History::History(int type, std::string fromUser, std::string toUser, std::string content, std::string timeStamp) : BaseMsg(type, fromUser, toUser, content)
{
    this->timeStamp = timeStamp;
}

Msg::Msg(int type, std::string fromUser, std::string toUser, std::string content) : BaseMsg(type, fromUser, toUser, content) {}

std::string Msg::serializer() //利用jsoncpp库来实现序列化与反序列化 以及实现不定长字符串的发送
{
    Json::Value root;
    root["type"] = Json::Value(this->type);
    root["toUser"] = Json::Value(this->toUser);
    root["fromUser"] = Json::Value(this->fromUser);
    root["content"] = Json::Value(this->content);
    return root.toStyledString();
}

void Msg::parser(std::string jsonString)
{
    Json::CharReaderBuilder rbuilder;
    std::istringstream iss(jsonString);
    std::string err;
    Json::Value root;
    Json::parseFromStream(rbuilder, iss, &root, &err);
    this->type = root["type"].asInt();
    this->toUser = root["toUser"].asString();
    this->fromUser = root["fromUser"].asString();
    this->content = root["content"].asString();
}

void Msg::mapToContent(std::string column[2], std::map<std::string, std::string> &pMap)
{
    Json::Value root;
    std::map<std::string, std::string>::iterator it = pMap.begin();
    for (int i = 0; i < pMap.size(); i++, it++)
    {
        root[i][column[0]] = Json::Value(it->first);
        root[i][column[1]] = Json::Value(it->second);
    }
    this->content = root.toStyledString();
}

void Msg::contentToMap(std::string column[2], std::map<std::string, std::string> &pMap)
{
    std::istringstream iss(this->content);
    Json::CharReaderBuilder rbuilder;
    std::string err;
    Json::Value root;
    Json::parseFromStream(rbuilder, iss, &root, &err);
    for (int i = 0; i < root.size(); i++)
    {
        pMap[root[i][column[0]].asString()] = root[i][column[1]].asString();
    }
}

void Msg::historyToContent(std::list<History *> &pList)
{
    Json::Value root;
    std::list<History *>::iterator it = pList.begin();
    for (int i = 0; i < pList.size(); i++, it++)
    {
        root[i]["fromUser"] = Json::Value((*it)->fromUser);
        root[i]["target"] = Json::Value((*it)->toUser);
        root[i]["content"] = Json::Value((*it)->content);
        root[i]["timeStamp"] = Json::Value((*it)->timeStamp);
    }
    this->content = root.toStyledString();
}

void Msg::contentToHistory(std::list<History *> &pList)
{
    std::istringstream iss(this->content);
    Json::CharReaderBuilder rbuilder;
    std::string err;
    Json::Value root;
    Json::parseFromStream(rbuilder, iss, &root, &err);
    for (int i = 0; i < root.size(); i++)
    {
        History *tmp = new History;
        tmp->fromUser = root[i]["fromUser"].asString();
        tmp->toUser = root[i]["target"].asString();
        tmp->content = root[i]["content"].asString();
        tmp->timeStamp = root[i]["timeStamp"].asString();
        pList.push_back(tmp);
    }
}

std::string packUp(std::string jsonString)
{
    char Csz[9];
    sprintf(Csz, "%08d", (int)jsonString.size());
    return PACKET_HEADER + std::string(Csz) + jsonString + PACKET_END;
}

std::string unPack(std::string packet)
{
    if (packet.find(PACKET_HEADER) != std::string::npos)
    {
        if (packet.find(PACKET_END) != std::string::npos)
        {
            return packet.substr(PACKET_HEADER_SIZE + SIZE_LENGTH - 1, atoi(packet.substr(PACKET_HEADER_SIZE - 1, SIZE_LENGTH).c_str()));
        }
    }
    return Msg().serializer();
}
