import launch
import launch_ros


def generate_launch_description():
    steer_ros_topics = ["steer_1_pos", "steer_2_pos", "steer_3_pos"]
    wheel_ros_topics = ["wheel_1_vel", "wheel_2_vel", "wheel_3_vel"]
    gz_steer_topics = [
        "/model/three_wheel_swerve/joint/steer_joint_1/cmd_pos",
        "/model/three_wheel_swerve/joint/steer_joint_2/cmd_pos",
        "/model/three_wheel_swerve/joint/steer_joint_3/cmd_pos",
    ]
    gz_wheel_topics = [
        "/model/three_wheel_swerve/joint/wheel_joint_1/cmd_vel",
        "/model/three_wheel_swerve/joint/wheel_joint_2/cmd_vel",
        "/model/three_wheel_swerve/joint/wheel_joint_3/cmd_vel",
    ]

    bridge_args = []
    for r, g in zip(steer_ros_topics, gz_steer_topics):
        bridge_args.append(f"{r}@{g}@std_msgs/msg/Float64@gz.msgs.Double")
    for r, g in zip(wheel_ros_topics, gz_wheel_topics):
        bridge_args.append(f"{r}@{g}@std_msgs/msg/Float64@gz.msgs.Double")

    bridge_node = launch_ros.actions.Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=bridge_args,
        output='screen',
    )

    cmd_vel_to_swerve_node = launch_ros.actions.Node(
        package='swerve_controller',
        executable='cmd_vel_to_swerve',
        output='screen',
    )

    return launch.LaunchDescription([
        bridge_node,
        cmd_vel_to_swerve_node,
    ])
