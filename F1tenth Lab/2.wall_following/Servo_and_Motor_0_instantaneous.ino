/*
  Servo Sub
  Servo Pub
  DC    Pub

  사용법

  1.roscore&
  2.rosrun rosserial_python serial_node.py _port:=[Arduino ROS 시리얼 포트]
  3.rostopic pub /servo std_msgs/UInt16 "data: 넣으려는 값"
  3.rostopic pub /dc std_msgs/Float32 "넣으려는 값"
  4. rostopic echo /servo
 */

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include "CytronMotorDriver.h"


ros::NodeHandle  nh;

// DC, Servo, 그리고 Servo의 각도를 publish할 theta
CytronMD motor(PWM_DIR, 13, 22);  //(PWM_DIR, PWM_PIN, PWM_DIR) PWM = Pin 3, DIR = Pin 4.
Servo servo;
int flag = 0;


// 서보랑 dc모터
void callback_servo_and_dc(const std_msgs::Float32MultiArray& cmd_msg){
  
  // dc모터 처리
  if(flag == 0){
    if(cmd_msg.data[0] < -30) motor.setSpeed(-30);
    else if(cmd_msg.data[0] > 30) motor.setSpeed(30);
    else  motor.setSpeed(cmd_msg.data[0]);
  }

  //angle 처리
  if(cmd_msg.data[1] < 85) servo.write(85);
  else if(cmd_msg.data[1] > 145) servo.write(145);
  else  servo.write(cmd_msg.data[1]);
 
  
}

void callback_dc(const std_msgs::Float32 &msg)
{

  if(0 == msg.data){
    flag = 1;
    motor.setSpeed(msg.data);
  }
  else{
    flag = 0;
  }
}

// declaration
ros::Subscriber<std_msgs::Float32MultiArray> sub_servo_and_dc("/servo_and_dc", callback_servo_and_dc);
ros::Subscriber<std_msgs::Float32> sub_dc("/dc", callback_dc); // safety에서 발행한 dc를 구독해야 한다. -> emergency braking을 진행하기 위해

//setup
void setup(){
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(sub_dc);  
  nh.subscribe(sub_servo_and_dc);
  
  
  servo.attach(52); //attach it to pin 9
}




//loop
void loop(){ 
  
  nh.spinOnce();
  
  delay(1);
}
