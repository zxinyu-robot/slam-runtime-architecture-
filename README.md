# Fork Spatial Runtime

[![CI](https://github.com/zxinyu-robot/fork-spatial-runtime/actions/workflows/ci.yml/badge.svg)](https://github.com/zxinyu-robot/fork-spatial-runtime/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C.svg)](https://isocpp.org/)

面向机器人状态估计管线的、与优化器后端解耦的增量空间状态运行时。

Fork Spatial Runtime 位于传感器前端和图优化后端之间，统一管理状态变量生命周期、因子批次校验、原子图事务和固定延迟窗口边缘化。项目当前是一个可构建、可测试的 C++20 原型，不包含完整 SLAM 前端，也不宣称替代 GTSAM、Ceres 或 ROS 2。

> A backend-agnostic C++20 runtime for incremental spatial state and factor-graph transactions.

## 为什么需要它

机器人系统通常需要同时接入 LiDAR、IMU、视觉和外部定位源。若各传感器模块直接修改优化图，状态创建、迟到观测、重复因子和边缘化行为会分散在不同模块中，难以测试和回滚。

本项目将这些职责收敛到一个窄接口：

```text
Sensor adapters
      │ MeasurementEnvelope
      ▼
Factor providers
      │ FactorBatch
      ▼
SpatialRuntime ── validation / lifecycle / transaction / window
      │ GraphTransaction
      ▼
Optimizer backend (GTSAM / Ceres / test double)
```

## 当前已实现

- 状态键：机器人、会话、状态类型、时间戳和实例编号。
- 状态生命周期：Requested → Initialized → Active / Fixed → Marginalized → Archived。
- 因子批次校验：重复 ID、缺失状态、非法协方差和迟到观测。
- 原子提交语义：后端拒绝事务时，不提交候选状态和图版本。
- 固定延迟窗口策略，以及由后端生成边缘化先验的接口。
- 后端无关的 `IOptimizerBackend`、`IFactorProvider` 和 `ISensorAdapter` 接口。
- Linux/macOS CI 和无需第三方依赖的单元测试。

## 尚未实现

- GTSAM、Ceres 或 g2o 的生产后端。
- ROS 2/DDS 适配器、共享内存传输和持久化。
- 多机器人子图融合、回环检测和可视化界面。
- 实时性能 benchmark 与真机数据集验证。

这些内容属于后续路线，不是当前版本的完成能力。

## 快速开始

要求：CMake 3.20+、Make 和支持 C++20 的编译器。

```bash
cmake --preset default
cmake --build --preset default
ctest --preset default --output-on-failure
./build/default/examples/cpp/runtime_demo
```

预期演示输出：

```text
accepted graph version: 1
active states: 1
backend operations: 3
```

## 最小用法

```cpp
using namespace fork_spatial::runtime;

SpatialRuntime runtime(
    std::make_unique<MyOptimizerBackend>(),
    std::make_unique<FixedLagWindowPolicy>(2'000'000'000));

FactorBatch batch;
batch.state_requests.push_back({pose_key, initial_pose});

const SubmitResult result = runtime.submit(batch);
if (result) {
    const EstimateSnapshot estimate = runtime.optimize();
}
```

优化后端必须保证 `apply()` 的原子性。边缘化分为“计算先验”和“提交事务”两步，避免后端部分修改后运行时状态无法回滚。

## 项目结构

```text
src/runtime/
  model.h             # 状态、因子和事务数据模型
  interfaces.h        # 传感器、因子提供器、窗口和后端接口
  state_store.*       # 状态生命周期与索引
  spatial_runtime.*   # 批次校验、提交、边缘化和优化入口
examples/cpp/
  runtime_demo.cpp
  test_spatial_runtime.cpp
docs/
  architecture.md
```

## 路线图

1. 增加 GTSAM iSAM2 适配器和 SE(3) 状态类型约束。
2. 增加乱序观测缓冲、并发读快照和可重复 benchmark。
3. 增加 ROS 2 消息适配器与 rosbag 回放示例。
4. 使用公开数据集报告吞吐、延迟和内存占用。

详细设计边界见 [架构说明](docs/architecture.md)。

## License

[MIT](LICENSE)
