#include <benchmark/benchmark.h>

#include <openstudio/model/Model.hpp>

#include <openstudio/model/Surface.hpp>
#include <openstudio/model/Surface_Impl.hpp>
#include <openstudio/model/ModelObject.hpp>
#include <openstudio/model/ModelObject_Impl.hpp>

#include <openstudio/utilities/idf/WorkspaceObject.hpp>
#include <openstudio/utilities/geometry/Point3d.hpp>
#include <openstudio/utilities/core/Assert.hpp>

#include <vector>
#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

model::Model makeModelWithNSurfaces(size_t nSurfaces) {
  Model m;
  double zOrigin = 0;
  Point3dVector pts{{0, 0, zOrigin}, {0, 1, zOrigin}, {1, 1, zOrigin}, {1, 0, zOrigin}};

  for (size_t i = 0; i < nSurfaces; ++i) {
    Surface(pts, m);
  }

  OS_ASSERT(m.getConcreteModelObjects<Surface>().size() == nSurfaces);
  return m;
}

static void Current(benchmark::State& state) {

  Model m = makeModelWithNSurfaces(state.range(0));

  std::vector<WorkspaceObject> objects = m.objects();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

    std::vector<ModelObject> result;

    for (auto& object : objects) {
      ModelObject castObject = object.cast<ModelObject>();
      result.push_back(castObject);
    }

    benchmark::DoNotOptimize(result);
  };

  state.SetComplexityN(state.range(0));
}

static void Proposed(benchmark::State& state) {

  Model m = makeModelWithNSurfaces(state.range(0));

  std::vector<WorkspaceObject> objects = m.objects();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

    std::vector<ModelObject> result;
    result.reserve(objects.size());

    for (auto& object : objects) {
      ModelObject castObject = object.cast<ModelObject>();
      result.push_back(castObject);
    }

    benchmark::DoNotOptimize(result);
  };

  state.SetComplexityN(state.range(0));
}

static void ProposedPushBackNoName(benchmark::State& state) {

  Model m = makeModelWithNSurfaces(state.range(0));

  std::vector<WorkspaceObject> objects = m.objects();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

    std::vector<ModelObject> result;
    result.reserve(objects.size());

    for (auto& object : objects) {
      result.push_back(object.cast<ModelObject>());
    }

    benchmark::DoNotOptimize(result);
  };

  state.SetComplexityN(state.range(0));
}

static void ProposedEmplace(benchmark::State& state) {

  Model m = makeModelWithNSurfaces(state.range(0));

  std::vector<WorkspaceObject> objects = m.objects();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

    std::vector<ModelObject> result;
    result.reserve(objects.size());

    for (auto& object : objects) {
      result.emplace_back(object.cast<ModelObject>());
    }

    benchmark::DoNotOptimize(result);
  };

  state.SetComplexityN(state.range(0));
}

// (Note: Under heaver external load, 1024 takes 99 s, 2048 takes 475s (8 min))
// Normal load: 1024 takes 33s
BENCHMARK(Current)
  // ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Range(4, 2048)
  ->Complexity();

BENCHMARK(Proposed)
  // ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Range(4, 2048)
  ->Complexity();

BENCHMARK(ProposedPushBackNoName)
  // ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Range(4, 2048)
  ->Complexity();

BENCHMARK(ProposedEmplace)
  // ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Range(4, 2048)
  ->Complexity();
