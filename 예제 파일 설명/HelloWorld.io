#include <ros.h>
#include <std_msgs/String.h>


//노드를 처리하는 NodeHandle 선언
ros::NodeHandle nh;


std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg); //chatter라는 topic에서 계속 msg 발행


char hello[13] = "hello world!";

void setup()
{
    nh.initNode();
    nh.advertise(chatter);
}

void loop()
{
    str_msg.data = hello; // str_msg.data에 문자열 대입
    chatter.publish(&str_msg); // &str_msg 발행
    nh.spinOnce();
    delay(1000);
}
