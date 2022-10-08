#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"
#include "sensor_msgs/LaserScan.h"

ros::Publisher cmd_vel_publisher;

void radarCallback(const sensor_msgs::Range::ConstPtr& range_msg)
{
    // ROS_INFO("Range is: %f",range_msg->range);

    // geometry_msgs::Twist mi_mensaje_twist;

    // mi_mensaje_twist.linear.x = (1.0-range_msg->range)*2.0;
    // mi_mensaje_twist.angular.z = 0.0;

    // cmd_vel_publisher.publish(mi_mensaje_twist);

}

void lidarCallback(const sensor_msgs::LaserScan::ConstPtr& lidar_msg)
{
    int num_lasers = abs(lidar_msg->angle_max-lidar_msg->angle_min)/lidar_msg->angle_increment;
    ROS_INFO("Num of scans is: %d",num_lasers);

    float range = lidar_msg->ranges[(int)num_lasers/2-1];

    // ROS_INFO("Lidar scan is %d and range is: %f",((int)num_lasers/2-1),range);

    if(range == std::numeric_limits<sensor_msgs::Range::_range_type>::infinity())
    {
        range = lidar_msg->range_max;
    }

    geometry_msgs::Twist mi_mensaje_twist;

    ROS_INFO("Lidar range is: %f",range);

    mi_mensaje_twist.linear.x = (2.0-range)*-2.0;

    mi_mensaje_twist.angular.z = 0.0;

    cmd_vel_publisher.publish(mi_mensaje_twist);

}


int main(int argc, char **argv)
{
    ros::init(argc,argv,"nombre_mi_nodo");
    ros::NodeHandle nh;

    cmd_vel_publisher = nh.advertise<geometry_msgs::Twist>("/adc_car/cmd_vel",1000);
    ros::Subscriber sonar_subscriber = nh.subscribe("/adc_car/sensors/range/rear_center_sonar",1000,radarCallback);
    ros::Subscriber lidar_subscriber = nh.subscribe("/adc_car/sensors/scan",1000,lidarCallback);

    ros::Rate loop_rate(10); // Frequencia del bucle

    while(ros::ok())
    {

        // geometry_msgs::Twist mi_mensaje_twist;



        // mi_mensaje_twist.linear.x = 1.0;
        // mi_mensaje_twist.angular.z = 0.0;

        // cmd_vel_publisher.publish(mi_mensaje_twist);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}