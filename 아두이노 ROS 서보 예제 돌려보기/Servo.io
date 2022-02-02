#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

//서보 라이브러리
#include <Servo.h> 
//로스 라이브러리
#include <ros.h>
//메시지 해더
#include <std_msgs/UInt16.h>

//ROS 노드 클래스 변수
ros::NodeHandle  nh;

//서보 클래스 변수
Servo servo;

//서보 메시지 콜백함수 RX(recieve) 함수
void servo_cb( const std_msgs::UInt16& cmd_msg){
  //실제 메시지 전달시 이 루틴을 들어오게 된다.
  //전덜 데이터(서보 각도)를 기준으로 서보 제어
  servo.write(cmd_msg.data); //set servo angle, should be from 0-180  

  //동작중인지 LED 깜박임 
  digitalWrite(PC13, HIGH-digitalRead(PC13));  //toggle led  
}

//ROS 메시지 Rx(구독) 등록 
//servo라는 노드로 메시지 받을 수 있음.
ros::Subscriber<std_msgs::UInt16> sub("servo", servo_cb);

void setup(){
  //LED 핀 출력 설정
  pinMode(PC13, OUTPUT);

  //노드 초기화
  nh.initNode();
  //노드 설정 - 서보
  nh.subscribe(sub);
  
  //서보 핀 등록
  servo.attach(PB6); 
}


void loop(){
  //메시지 있으면 콜백 실행 
  nh.spinOnce();
  //1ms 대기
  delay(1);
}