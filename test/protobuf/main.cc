#include "test.pb.h"
#include <iostream>
#include <string>

using namespace rpcpr;

void test1(){
    LoginRequest req;
    req.set_name("yyc");
    req.set_pwd("200728");

    std::string send_str;
    if(req.SerializeToString(&send_str)) // 序列化
    {
        std::cout << send_str << std::endl;
    }


    LoginRequest reqB;
    if(reqB.ParseFromString(send_str))
    {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    } // 反序列化
}
void test2(){
    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();  // result是GetFriendListsResponse的ResultCode 类型
    rc->set_errcode(0);

    User *user1 = rsp.add_friend_list();    // 数组类型用add加入
    user1->set_name("yyc");
    user1->set_age(18);
    user1->set_sex(User::MAN);
    User *user2 = rsp.add_friend_list();    // 数组类型用add加入
    user2->set_name("yyc");
    user2->set_age(19);
    user2->set_sex(User::MAN);
    User *user3 = rsp.add_friend_list();    // 数组类型用add加入
    user3->set_name("yyc");
    user3->set_age(20);
    user3->set_sex(User::WOMAN);

    std::cout << rsp.friend_list(2).name() << rsp.friend_list(2).age() << rsp.friend_list(2).sex() << std::endl;
    std::cout << rsp.friend_list_size() << std::endl;
}
int main()
{
    test1();
    test2();

    return 0;
}