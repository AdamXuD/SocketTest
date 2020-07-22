#include "msg.h"

using namespace std;

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

    // map<string, string> test;
    // test["aaa"] = "bbb";
    // test["ccc"] = "ddd";

    // Msg msg;
    // string column[2] = {"111", "222"};
    // msg.mapToContent(column, test);
    // cout << msg.content << endl;

    // Msg msg2;
    // msg2.parser(unPack(packUp(msg.serializer())));

    // map<string, string> test2;
    // msg2.contentToMap(column, test2);
    // map<string, string>::iterator it = test2.begin();
    // for(int i = 0; i < test2.size(); i++, it++)
    // {
    //     cout << it->first << " " << it->second << endl;
    // }
    list<History *> history;
    History *history1 = new History;
    history1->fromUser = "fromUser1";
    history1->toUser = "toUser1";
    history1->content = "content1";
    history1->timeStamp = "timeStamp1";
    History *history2 = new History;
    history2->fromUser = "fromUser2";
    history2->toUser = "toUser2";
    history2->content = "content2";
    history2->timeStamp = "timeStamp2";
    history.push_back(history1);
    history.push_back(history2);
    Msg test;
    test.historyToContent(history);
    cout << test.content << endl;
    list<History *> history_2;
    test.contentToHistory(history_2);
    list<History *>::iterator it = history_2.begin();
    for (int i = 0; i < history_2.size(); i++, it++)
    {
        cout << (*it)->fromUser << endl;
        cout << (*it)->toUser << endl;
        cout << (*it)->content << endl;
        cout << (*it)->timeStamp << endl;
    }

    return 0;
}