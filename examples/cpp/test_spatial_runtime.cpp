#include "runtime/spatial_runtime.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <span>
#include <string>

namespace {

class FakeBackend final : public slam::runtime::IOptimizerBackend {
public:
    bool apply(
        const slam::runtime::GraphTransaction& transaction,
        std::string& error) override {
        (void)error;
        last_sequence_ = transaction.sequence;
        return true;
    }

    slam::runtime::EstimateSnapshot optimize(
        std::uint64_t graph_version) override {
        return slam::runtime::EstimateSnapshot{graph_version, 0, {}};
    }

    bool marginalize(
        std::span<const slam::runtime::StateKey> states,
        slam::runtime::FactorProposal& prior,
        std::string& error) override {
        (void)error;
        prior = {};
        marginalized_count_ += states.size();
        return true;
    }

    std::uint64_t last_sequence_{};
    std::size_t marginalized_count_{};
};

slam::runtime::FactorProposal odometry_factor(
    slam::runtime::FactorId id,
    const slam::runtime::StateKey& first,
    const slam::runtime::StateKey& second) {
    return slam::runtime::FactorProposal{
        id,
        "test_odometry",
        "relative_pose",
        second.timestamp_ns,
        {first, second},
        {1.0, 0.0, 0.0},
        {{0.1, 0.1, 0.1}},
        {},
    };
}

}  // namespace

int main() {
    using namespace slam::runtime;

    auto backend = std::make_unique<FakeBackend>();
    SpatialRuntime runtime(
        std::move(backend),
        std::make_unique<FixedLagWindowPolicy>(100));

    const StateKey old_pose{1, 1, StateType::Pose, 0, 0};
    const StateKey current_pose{1, 1, StateType::Pose, 200, 0};
    FactorBatch initial_batch;
    initial_batch.state_requests = {
        {old_pose, {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}}},
        {current_pose, {{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}}},
    };
    initial_batch.factors.push_back(
        odometry_factor(1, old_pose, current_pose));

    const SubmitResult accepted = runtime.submit(initial_batch);
    assert(accepted);
    assert(runtime.graph_version() == 1);

    FactorBatch duplicate_batch;
    duplicate_batch.factors.push_back(
        odometry_factor(1, old_pose, current_pose));
    assert(runtime.submit(duplicate_batch).code == SubmitCode::DuplicateFactor);

    const SubmitResult marginalized = runtime.advance_window(150);
    assert(marginalized);
    assert(runtime.graph_version() == 2);

    const std::vector<StateRecord> states = runtime.states();
    const auto old_state = std::find_if(
        states.begin(), states.end(),
        [&](const StateRecord& state) { return state.key == old_pose; });
    assert(old_state != states.end());
    assert(old_state->lifecycle == StateLifecycle::Marginalized);

    FactorBatch late_batch;
    late_batch.factors.push_back(
        odometry_factor(2, old_pose, current_pose));
    assert(runtime.submit(late_batch).code == SubmitCode::LateObservation);

    const EstimateSnapshot snapshot = runtime.optimize();
    assert(snapshot.graph_version == 2);
    return 0;
}
