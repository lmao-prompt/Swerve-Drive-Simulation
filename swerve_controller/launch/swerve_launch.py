import launch
import launch_ros


def generate_launch_description():
    steer_gz_topics = [
        "/model/three_wheel_swerve/joint/steer_joint_1/cmd_pos",
        "/model/three_wheel_swerve/joint/steer_joint_2/cmd_pos",
        "/model/three_wheel_swerve/joint/steer_joint_3/cmd_pos",
    ]
    wheel_gz_topics = [
        "/model/three_wheel_swerve/joint/wheel_joint_1/cmd_vel",
        "/model/three_wheel_swerve/joint/wheel_joint_2/cmd_vel",
        "/model/three_wheel_swerve/joint/wheel_joint_3/cmd_vel",
    ]

    bridge_args = []
    for g in steer_gz_topics:
        bridge_args.append(f"{g}@{g}@std_msgs/msg/Float64@gz.msgs.Double")
    for g in wheel_gz_topics:
        bridge_args.append(f"{g}@{g}@std_msgs/msg/Float64@gz.msgs.Double")

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
