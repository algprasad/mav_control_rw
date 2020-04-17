#include <iostream>
#include "ros/ros.h"
#include <trajectory_msgs/MultiDOFJointTrajectory.h>
#include <cmath>

void generateTrajectoryMessage(trajectory_msgs::MultiDOFJointTrajectory& trajectory) {
    unsigned int num_waypoints = 5;



    for(int i = 0; i<num_waypoints; i++){
        trajectory_msgs::MultiDOFJointTrajectoryPoint waypoint;
        geometry_msgs::Transform transform;
        geometry_msgs::Vector3 position;
        geometry_msgs::Quaternion orientation;

        //X, Y, Z coordinates of the positon of the first waypoint
        position.x = i;
        position.y = i;
        position.z = i;

        orientation.x = 0;
        orientation.y = 0;
        orientation.z = 0;
        orientation.w = 1;
        transform.rotation = orientation;
        transform.translation = position;

        waypoint.time_from_start.sec = i;

        //There is only one joint, but still its a vector so push_back had to be done
        waypoint.transforms.push_back(transform);

        //pushing the waypoint into the trajectory
        trajectory.points.push_back(waypoint);

    }



}


void generateSineTrajectory(trajectory_msgs::MultiDOFJointTrajectory& trajectory){
    //Sample time for the trajectory waypoints
    double Ts = 0.1;
    double omega = 1;

    unsigned int num_waypoints = 500;
    for(int i =0; i<num_waypoints; i++){
        trajectory_msgs::MultiDOFJointTrajectoryPoint waypoint;
        geometry_msgs::Transform transform;
        geometry_msgs::Vector3 position;
        geometry_msgs::Quaternion orientation;

        //X, Y, Z coordinates of the positon of the first waypoint
        //time of trajectory
        double t = Ts*i;
        position.x = omega*t;
        position.y = 0;
        //z = sine(x)
        position.z = 2 +  std::sin(position.x);

        orientation.x = 0;
        orientation.y = 0;
        orientation.z = 0;
        orientation.w = 1;
        transform.rotation = orientation;
        transform.translation = position;

        waypoint.time_from_start.sec = int(t);

        //calculate total time in nano seconds
        double nano_secs = (t-int(t))*1000000000;
        waypoint.time_from_start.nsec = int(nano_secs);

        //There is only one joint, but still its a vector so push_back had to be done
        waypoint.transforms.push_back(transform);

        //pushing the waypoint into the trajectory
        trajectory.points.push_back(waypoint);

    }

}

int main(int argc, char **argv){
	ros::init(argc, argv, "alg_trajectory");
	ros::NodeHandle nh;

	// Publisher and Subsriber stuff
	ros::Publisher traj_pub = nh.advertise<trajectory_msgs::MultiDOFJointTrajectory>("firefly/command/trajectory", 1000);
	
	// ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
    ros::Rate rate(20);


    //The trajectory message is only required to published once
    ///The trajectory was not publishing as it takes time to establish connection which was not being provided with a single pass. Hence waiting until the connections were established sovled the issue
	bool trajectory_published = false;
    while (!trajectory_published){
	    if(traj_pub.getNumSubscribers() > 0){
            trajectory_msgs::MultiDOFJointTrajectory traj_msg;

            //make the trajectory message
            //generateTrajectoryMessage(traj_msg);
            generateSineTrajectory(traj_msg);

            //publish the message
            traj_pub.publish(traj_msg);
            trajectory_published = true;

        }
	    else rate.sleep();

	}
	
	return 0;


}
