#include <ros/ros.h>
#include <tf/transform_listener.h>

// get roll, pitch and yaw expression of quaternion
// express by Pitch -> Roll -> Yaw order,
// instead of standard Yaw -> Pitch -> Roll order.
void getPRY(tf::Quaternion &q, double &roll, double &pitch, double &yaw) {
    // false q. swapping (x, y, z) -> (z, x, y)
    tf::Quaternion qf{q.z(), q.x(), q.y(), q.w()};
    // false fall of getRPY. swapping (Y, P, R) -> (P, R, Y)
    tf::Matrix3x3(qf).getRPY(yaw, roll, pitch);
}

int main(int argc, char** argv){
    ros::init(argc, argv, "test_lis_node");

    ros::NodeHandle node;

    tf::TransformListener listener;

    ros::Rate rate(5.0);

    while (node.ok()){
        tf::StampedTransform tr;

        try{
            listener.lookupTransform("map", "imu",
                    ros::Time(0), tr);
        }
        catch (tf::TransformException ex){
            ROS_ERROR("%s",ex.what());
            ros::Duration(1.0).sleep();
        }

        tf::Quaternion rot = tr.getRotation();

        double roll, pitch, yaw;
        tf::Matrix3x3(rot).getRPY(roll, pitch, yaw);
        ROS_INFO("YPR: %+3.1f %+3.1f %+3.1f",
                180.0 / M_PI * roll, 180.0 / M_PI * pitch, 180.0 / M_PI * yaw);

        getPRY(rot, roll, pitch, yaw);
        ROS_INFO("PRY: %+3.1f %+3.1f %+3.1f",
                180.0 / M_PI * roll, 180.0 / M_PI * pitch, 180.0 / M_PI * yaw);

        rate.sleep();
    }
    return 0;
};

