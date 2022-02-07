#include "ros/ros.h"
#include "math.h"
#include <sensor_msgs/LaserScan.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include <stdio.h>
#include <vector>

#define deg2rad(x) x * M_PI/180
#define rad2deg(x) x * 180/M_PI

#define DistThreshold 2.0
#define RADIUS 200

using namespace std;
class reactive_follow_gap{

	struct FOV //Field Of View
	{
		unsigned int minIdx = 0;
		unsigned int maxIdx = 0;
	}

private:
	std::string m_lidarscan_topic;
	std::string m_drive_topic;

	ros::Subscriber m_LidarSub;
	ros::Publisher  m_drivePub;

	ros::NodeHandle n_;
	
	double m_angle;
	std::vector<double> ranges;

	double m_angle_increment;
	double m_angle_min;

public:
	reactive_follow_gap()
	{
		m_lidarscan_topic = "\scan";
		m_drive_topic = "/nav";
		m_LidarSub = n_.subscribe("/scan", 1000, &reactive_follow_gap::lidar_callback, this);
		m_drivePub = n_.advertise<ackermann_msgs::AckermannDriveStamped>("/nav", 1000);
	}


void lidar_callback(const sensor_msgs::LaserScan& data){
	/***************************************************
	 *
	 * Process each LIDAR scan as per the Follow Gap algorithm & publish an AckermannDriveStamped Massage
	 *
	 ***************************************************/


	// range setting -85 ~ 85 (degree)
	m_angle_increment = data.angle_increment;
	m_angle_min = data.angle_min;

	FOV fov;
	double minAngle = deg2rad((-90));
	fov.minIdx = abs((m_angle_min - minAngle) / m_angle_increment);
	double maxAngle = deg2rad((90));
	fov.maxIdx = abs((m_angle_max - maxAngle) / m_angle_increment);


	std::vector<float> ranges = data.ranges;
	std::vector<float> proc_ranges = preprocess_lidar(ranges, data.range_min, data.range_max);

	// Find closest point to LiDAR
	unsigned int closeIdx = 0;
	double closeValue = data.range_max;
	for(int i = fov.minIdx ; i <= fov.maxIdx ; i++){
		if(closeValue > proc_ranges[i]){
			closeValue = proc_ranges[i];
			closeIdx = i;
		}
	}


	// Eliminate all points inside 'bublle' (set them to zero)
	int radius = RADIUS;
	for(int i = closeIdx - radius; i < closeIdx + radius ; i++ ) // 가장 가까운 점의 거리와 반지름을 뺀다.
		proc_ranges[i] = 0.0;
	
	// Find max length gap
	unsigned int startIdx = 0.0;
	unsigned int endIdx   = 0.0;
	find_max_gap(proc_ranges, startIdx, endIdx, fov);

	// Find the best point in the gap
	double steering_angle = find_best_point(startIdx, endIdx, proc_ranges);

	// Publish Drive message
	ackermann_msgs::AckermannDriveStamped control_msg;
	control_msg.header.stamp	  = ros::Time::now();
	control_msg.header.frame_id	  = "laser";
	control_msg.drive.steering_angnle = steering_angle;
	control_msg.drive.speed		  = 1.5;

	m_drivePub.publish(control_msg);
	}

// 여기서부터 계속 이어나가는 걸로 ㅇㅅㅇ//
	

	

vector<float> preprocess_lidar(std::vector<float> ranges, double range_min, double range_max){
	/*************************************************************************************************		
	 *
	 * Preprocess the LiDAR scan array. Expert implementation includes:
	 * 1.Setting each value to the mean over some window
	 * 2.Rejecting the high values (eg. > 3m)
	 *
	 *
	 *************************************************************************************************/

	for(int i = 0 ; i < ranges.size() ; i++){
		
		if(std::isinf(ranges[i]) || std::isnan(ranges[i]))
			ranges[i] = 0.0;
		else if(ranges[i] < range_min)
			ranges[i] = range_min; //이미 넣은 range_min보다 더 작을 수는 없다.
		else if(ranges[i] > = range_max)
			ranges[i] = range_max; //이미 넣은 range_max보다 더 클 수는 없다.
	}

	return ranges;
}
		
		
		
		










































