#ifndef SLAM_COMMON_POSE_H
#define SLAM_COMMON_POSE_H

// 基础数据结构
namespace slam {
namespace common {
struct Pose {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};
}  // namespace common
}  // namespace slam

#endif
