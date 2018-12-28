#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class navGoals {

  public:
    navGoals():ac("move_base", true)
    
    {
      ros::NodeHandle n;
      get_time = n.subscribe("/app_point", 1000, &navGoals::getGoalCallback,this);
      pose_sub = n.subscribe("/amcl_pose", 1000, &navGoals::poseCallback,this);
      
    }

    void getGoalCallback(const geometry_msgs::Point::ConstPtr& app_goal)
    {
      
      move_x = app_goal->x;
      move_y = app_goal->y;
      move_base_msgs::MoveBaseGoal goal;
      
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
      
      /*
        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
          ROS_INFO("Hooray, succcesful");
        else
          ROS_INFO("The base failed to move forward 1 meter for some reason");  
      */
    }

    void poseCallback(const geometry_msgs::PoseWithCovarianceStampedConstPtr& current_pose)
    {
      
      current_pose_ = current_pose->pose.pose;
      ROS_INFO("Pose callback");
      double current_dist_to_target = std::sqrt(
          (current_pose_.position.x - move_x) * (current_pose_.position.x - move_x) +
          (current_pose_.position.y - move_y) * (current_pose_.position.y - move_y));
      ROS_INFO("%f",current_dist_to_target);
      if ((current_dist_to_target <= 0.7))  // skip when 0.4m near goal
      {
          ac.cancelAllGoals ();
          ROS_INFO("goal cancelled");

      }
    }

   protected:
    float move_x;
    float move_y; 
    float tolerance = 0.5;

    geometry_msgs::Pose current_pose_;
   
    ros::Subscriber get_time;
    ros::Subscriber pose_sub;
    MoveBaseClient ac;//("move_base", true);

};

int main(int argc, char** argv){
  
  ros::init(argc, argv, "simple_navigation_goals");
  navGoals navigation;

//tell the action client that we want to spin a thread by default
//move_base is the action name.  It also tells the action client to start a thread to call ros::spin() so that ROS callbacks will be processed by passing "true" as the second argument of the MoveBaseClient constructor. 

  ROS_INFO("main");
  ros::spin();

  return 0;

}

//rostopic pub /app_point geometry_msgs/Point ': 0, y: 0, z: 0}'