#include <benchmark/benchmark.h>

#include <openstudio/osversion/VersionTranslator.hpp>
#include <openstudio/model/Model.hpp>
#include <openstudio/utilities/core/Filesystem.hpp>

#include <openstudio/OpenStudio.hxx>

#include <resources.hxx>

using namespace openstudio;

static void BM_LoadIdfFile(benchmark::State& state, const std::string& testCase) {

  path modelPath = resourcesPath() / toPath(testCase);

  for (auto _ : state) {
    osversion::VersionTranslator translator;
    model::OptionalModel result = translator.loadModel(modelPath);
  }
}

BENCHMARK_CAPTURE(BM_LoadIdfFile, example_1_13_4, std::string("osversion/1_13_4/example.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, example_1_14_0, std::string("osversion/1_14_0/example.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, Windows_Complete, std::string("model/7-7_Windows_Complete.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, Model12, std::string("model/15023_Model12.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, floorplan_school, std::string("model/floorplan_school.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, two_stories_pre_intersect, std::string("model/two_stories_pre_intersect.osm"))
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, seb, std::string("model/seb.osm"))->Unit(benchmark::kMillisecond);
