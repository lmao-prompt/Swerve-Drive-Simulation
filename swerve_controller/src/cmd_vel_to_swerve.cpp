#include <cmath>
#include <vector>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/float64.hpp"

struct WheelModule {
  double x;
  double y;
};

class CmdVelToSwerve : public rclcpp::Node
{
public:
  CmdVelToSwerve() : Node("cmd_vel_to_swerve")
  {
    modules_ = {
      { 0.3,   0.0    },  // Roda 1 (Depan)
      {-0.15,  0.2598 },  // Roda 2 (Belakang Kiri)
      {-0.15, -0.2598 }   // Roda 3 (Belakang Kanan)
    };

    for (int i = 0; i < 3; ++i) {
      std::string id = std::to_string(i + 1);
      steer_pubs_.push_back(this->create_publisher<std_msgs::msg::Float64>(
        "/model/three_wheel_swerve/joint/steer_joint_" + id + "/cmd_pos", 10));
      wheel_pubs_.push_back(this->create_publisher<std_msgs::msg::Float64>(
        "/model/three_wheel_swerve/joint/wheel_joint_" + id + "/cmd_vel", 10));
    }

    cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel", 10,
      std::bind(&CmdVelToSwerve::cmdVelCallback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "Node Swerve 3 Roda Siap!");
  }

private:
  void cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
  {
    double vx = msg->linear.x;
    double vy = msg->linear.y;
    double omega = msg->angular.z;

    for (size_t i = 0; i < modules_.size(); ++i) {
      double mx = modules_[i].x;
      double my = modules_[i].y;

      double vx_m = vx - omega * my;
      double vy_m = vy + omega * mx;

      double speed = std::sqrt(vx_m * vx_m + vy_m * vy_m) / 0.05;

      double angle;
      if (speed < 0.01) {
        angle = last_angle_[i];
        speed = 0.0;
      } else {
        angle = std::atan2(vy_m, vx_m);
        while (angle > M_PI)  angle -= 2.0 * M_PI;
        while (angle < -M_PI) angle += 2.0 * M_PI;
      }

      double steer_change = std::abs(angle - last_angle_[i]);
      if (steer_change > M_PI) steer_change = 2.0 * M_PI - steer_change;

      auto steer_msg = std_msgs::msg::Float64();
      steer_msg.data = angle;
      steer_pubs_[i]->publish(steer_msg);

      auto wheel_msg = std_msgs::msg::Float64();
      if (steer_change > 0.087) {
        wheel_msg.data = 0.0;  // tunggu steer dulu
      } else {
        wheel_msg.data = speed;
      }
      wheel_pubs_[i]->publish(wheel_msg);

      if (speed > 0.01) {
        last_angle_[i] = angle;
      }
    }
  }

  std::vector<WheelModule> modules_;
  std::vector<rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr> steer_pubs_;
  std::vector<rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr> wheel_pubs_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
  double last_angle_[3] = {0.0, 0.0, 0.0};
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CmdVelToSwerve>());
  rclcpp::shutdown();
  return 0;
}