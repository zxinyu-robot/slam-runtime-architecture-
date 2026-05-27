#ifndef SLAM_TOOLS_LVO_H
#define SLAM_TOOLS_LVO_H

namespace slam {
namespace common {
struct Pose;
}

namespace frontend {
class VisualOdometry {
public:
    void estimate(const common::Pose& last_pose);
};
}  // namespace frontend
}  // namespace slam

#endif
