#include <termios.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

int ReturnInputKey()
{
  struct termios org_term;

  char input_key = 0;

  tcgetattr(STDIN_FILENO, &org_term);

  struct termios new_term = org_term;

  new_term.c_lflag &= ~(ECHO | ICANON);

  new_term.c_cc[VMIN] = 0;
  new_term.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

  read(STDIN_FILENO, &input_key, 1);

  tcsetattr(STDIN_FILENO, TCSANOW, &org_term);

  return input_key;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "robot_controller_node");
  ros::NodeHandle n;

  ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);

  int return_key = 0;

  ros::Rate loop_rate(60);
 
  geometry_msgs::Twist vel_msg;
  vel_msg.linear.x = 0;
  vel_msg.linear.y = 0;
  vel_msg.linear.z = 0;
  vel_msg.angular.x = 0;
  vel_msg.angular.y = 0;
  vel_msg.angular.z = 0;
 
  while (ros::ok())
  {
    return_key = ReturnInputKey();

    vel_msg.linear.x = 0;
    vel_msg.angular.z = 0;
    // W 입력시 전진
   
    if(return_key == 119 | return_key == 87)
    {
      ROS_INFO("Input W");
      vel_msg.linear.x = 1.0;
    }
    else if (return_key == 115 | return_key == 83)
    {
      ROS_INFO("Input S");
      vel_msg.linear.x = -1.0;
    }
    else if(return_key == 97 | return_key == 65)
    {
      ROS_INFO("Input A");
      vel_msg.angular.z = 1.0;
    }
    else if(return_key == 100 | return_key == 68)
    {
      ROS_INFO("Input D");
      vel_msg.angular.z -1.0;
    }
    else  if(return_key == 113 | return_key == 81)
    {
      ROS_INFO("Input X");
      break;
    }
    // ESC 입력시 종료
    vel_pub.publish(vel_msg);


    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;

}


