#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Point.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

float move_x;
float move_y; 

void getGoalCallback(const geometry_msgs::Point::ConstPtr& app_goal)
{
  move_x = app_goal->x;
  move_y = app_goal->y;
  move_base_msgs::MoveBaseGoal goal;
  
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
    //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = move_x;
  goal.target_pose.pose.position.y = move_y;
  goal.target_pose.pose.orientation.w = 1.0;
  
  ROS_INFO("Sending goal");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("Hooray, the base moved 1 meter forward");
  else
      ROS_INFO("The base failed to move forward 1 meter for some reason");  
}



int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle n;
  ros::Subscriber get_time = n.subscribe("/app_point", 1000, getGoalCallback);
  //tell the action client that we want to spin a thread by default
//move_base is the action name.  It also tells the action client to start a thread to call ros::spin() so that ROS callbacks will be processed by passing "true" as the second argument of the MoveBaseClient constructor. 


  ros::spin();

  return 0;

}

//rostopic pub /app_point geometry_msgs/Point ': 0, y: 0, z: 0}'