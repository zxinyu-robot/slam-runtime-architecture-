// 使用前向声明和命名空间
// 命名空间的作用是跨文件建立逻辑组织
// 前向声明 ， 避免库调用，减少依赖
namespace slam {
namespace common {
    struct Pose;
}

namespace frontend {
    class VisualOdometry {
    public:
    // 仅使用引用或指针时，不需要包含头文件
    void estimate(const common::Pose& last_pose);