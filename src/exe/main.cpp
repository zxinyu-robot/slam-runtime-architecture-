// system init
// 主循环
// 多线程调度
// main -> 核心业务模块 -> common
#include "utility/pose.h"
#include "tools/lvo.h"

int main() {
    slam::common::Pose pose;
    slam::frontend::VisualOdometry lvo;
    lvo.estimate(pose);
    // 系统运行逻辑
    return 0;
}
