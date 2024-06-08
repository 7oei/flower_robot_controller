#include "ros/ros.h"
#include "geometry_msgs/Vector3.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Bool.h"
#include <cmath>

void commandCallback(const geometry_msgs::Vector3::ConstPtr& msg, ros::Publisher* pub_diff_yaw, ros::Publisher* pub_robot_end) {
    if (msg->x == 0) {  // 直進の場合
        ros::Duration(msg->y).sleep();  // 直進距離y秒だけ待機
        std_msgs::Bool end_msg;
        end_msg.data = true;
        pub_robot_end->publish(end_msg);
    } else if (msg->x == 1) {  // 旋回の場合
        double total_time = 2 * M_PI * msg->y;  // 旋回時間
        ros::Time start_time = ros::Time::now();
        while (ros::Time::now() - start_time < ros::Duration(total_time)) {
            ros::spinOnce();
            double current_time = (ros::Time::now() - start_time).toSec();
            std_msgs::Float32 yaw_msg;
            yaw_msg.data = current_time;
            pub_diff_yaw->publish(yaw_msg);
            ros::Duration(0.01).sleep();  // 100Hzの間隔でpublish
        }
        std_msgs::Bool end_msg;
        end_msg.data = true;
        pub_robot_end->publish(end_msg);
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "flower_robot_controller");
    ros::NodeHandle n;

    ros::Publisher pub_diff_yaw = n.advertise<std_msgs::Float32>("diff_yaw", 1);
    ros::Publisher pub_robot_end = n.advertise<std_msgs::Bool>("robot_end", 1);

    ros::Subscriber sub = n.subscribe<geometry_msgs::Vector3>("robot_command", 1, boost::bind(commandCallback, _1, &pub_diff_yaw, &pub_robot_end));

    ros::spin();

    return 0;
}
