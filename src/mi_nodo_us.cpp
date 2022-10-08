#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include <sstream>
#include <math.h>


ros::Publisher cmd_vel_pub;

void radarCallback(const sensor_msgs::Range::ConstPtr& range_msg)
{
  double u = (0.5-range_msg->range)*2;
  ROS_INFO("Range is: [%f]", range_msg->range);
  geometry_msgs::Twist cmd_vel_msg;
  cmd_vel_msg.linear.x = u;
  cmd_vel_msg.angular.z = 0;
  cmd_vel_pub.publish(cmd_vel_msg);
}

void lidarCallback(const sensor_msgs::LaserScan::ConstPtr& lidar_msg)
{
  int num_angles = abs(lidar_msg->angle_max-lidar_msg->angle_min)/lidar_msg->angle_increment;
  float lidar_range = lidar_msg->ranges[(int)num_angles/2-1];

  ROS_INFO("Lidar resolution: [%d]",num_angles);

  if(lidar_range == std::numeric_limits<sensor_msgs::Range::_range_type>::infinity()){
    lidar_range = lidar_msg->range_max;
  }

  double u = -(0.5-lidar_range)*2;
  geometry_msgs::Twist cmd_vel_msg;
  cmd_vel_msg.linear.x = u;
  cmd_vel_msg.angular.z = 0;
  cmd_vel_pub.publish(cmd_vel_msg);

  ROS_INFO("Lidar range is: [%f] and u is: [%f]",lidar_range,u);
}



int main(int argc, char **argv)
{

  ros::init(argc, argv, "mi_nodo_us");
  ros::NodeHandle n;

	// Publicadores y suscriptores
  cmd_vel_pub = n.advertise<geometry_msgs::Twist>("/adc_car/cmd_vel", 1000);
  ros::Subscriber radar_sub = n.subscribe("/adc_car/sensors/range/rear_center_sonar", 1000, radarCallback);
  ros::Subscriber lidar_sub = n.subscribe("/adc_car/sensors/scan", 1000, lidarCallback);

	
	double f = 0.5;

  ros::Rate loop_rate(10); // 10 Hz


  while (ros::ok())
  {

    geometry_msgs::Twist cmd_vel_msg;

    double t = ros::Time::now().toSec();

    cmd_vel_msg.linear.x = 1.0;
    cmd_vel_msg.angular.z = 0.5*sin(t*2*M_PI*f);

    cmd_vel_pub.publish(cmd_vel_msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}