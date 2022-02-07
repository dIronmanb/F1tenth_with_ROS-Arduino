#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "/home/heeyang/catkin_ws/devel/include/lab1/scan_range.h" //Before Coding, Make messages.

ros::Publisher lidar_pub;

void msgCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    float max = msg->range_min; //In sensor_msgs/LaserScan.h, There is range_min
    float min = msg->range_max;

    //Make Algorithm
    for(int i = 0; i < msg->ranges.size(); i++){
    
	if(!std::isnan(msg->ranges[i])){
	
		if(!std::isinf(msg->ranges[i])){

			if(max < msg->ranges[i]) max = msg->ranges[i];
			if(min > msg->ranges[i]) min = msg->ranges[i];
		}
	}
    }

   ROS_INFO("min: %f", min);
   ROS_INFO("max: %f", max);


  lab1::scan_range p_msg; //Declare p_msg in format: scan_range
  p_msg.farthest_point = max;
  p_msg.cloest_point = min;
  lidar_pub.publish(p_msg); 
}


int main(int argc, char** argv)
{
    ros::init(argc, argv,"lab1");

    ros::NodeHandle nh;

    ros::Subscriber ros_tutorial_sub = nh.subscribe("scan", 100, msgCallback);

    lidar_pub = nh.advertise<lab1::scan_range>("scan_range", 100);

    ros::spin();
    
    

    return 0;

}
