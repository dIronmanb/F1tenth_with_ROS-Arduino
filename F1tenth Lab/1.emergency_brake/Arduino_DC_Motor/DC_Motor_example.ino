#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
 
#include <ros.h>
#include <std_msgs/Float32.h>
#include "CytronMotorDriver.h"

ros::NodeHandle  nh;
CytronMD motor(PWM_DIR, 13, 22);  //(PWM_DIR, PWM_PIN, PWM_DIR) PWM = Pin 3, DIR = Pin 4.


void dc_l298n( const std_msgs::Float32& cmd_msg){
  motor.setSpeed(cmd_msg.data);
    
}


ros::Subscriber<std_msgs::Float32> sub("dc", dc_l298n);

void setup(){
  nh.initNode();
  nh.subscribe(sub);
}

void loop(){
  nh.spinOnce();
  delay(1);
}
