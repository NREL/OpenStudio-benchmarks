#include <benchmark/benchmark.h>

#include <openstudio/energyplus/ForwardTranslator.hpp>

#include <openstudio/model/Model.hpp>

#include <openstudio/utilities/core/Logger.hpp>
#include <openstudio/utilities/core/FileLogSink.hpp>
#include <openstudio/utilities/idf/Workspace.hpp>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::energyplus;

static void BM_FT_ExampleModel_sameFT(benchmark::State& state) {

  FileLogSink logFile(toPath("./ForwardTranslator_Benchmark.log"));
  logFile.setLogLevel(Error);
  openstudio::Logger::instance().standardOutLogger().disable();

  Model model = exampleModel();

  ForwardTranslator forwardTranslator;
  forwardTranslator.setExcludeSpaceTranslation(state.range(1) == 0 ? false : true);

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (auto i = 0; i <= state.range(0); ++i) {
      Workspace workspace = forwardTranslator.translateModel(model);
    }
  }

  state.SetComplexityN(state.range(0));
}

static void BM_FT_ExampleModel_newFT(benchmark::State& state) {

  FileLogSink logFile(toPath("./ForwardTranslator_Benchmark.log"));
  logFile.setLogLevel(Error);
  openstudio::Logger::instance().standardOutLogger().disable();

  Model model = exampleModel();

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (auto i = 0; i <= state.range(0); ++i) {
      ForwardTranslator forwardTranslator;
      forwardTranslator.setExcludeSpaceTranslation(state.range(1) == 0 ? false : true);
      Workspace workspace = forwardTranslator.translateModel(model);
    }
  }

  state.SetComplexityN(state.range(0));
}

// Regular run, with n=512
/*
BENCHMARK(BM_WorkspaceSetNameWithChecks)->Unit(benchmark::kMillisecond)->Arg(512);
BENCHMARK(BM_WorkspaceSetNameWithoutAnyChecks)->Unit(benchmark::kMillisecond)->Arg(512);
*/

// Simple Runs
//BENCHMARK(BM_ExampleModel);

// With Complexity
BENCHMARK(BM_FT_ExampleModel_newFT)->Unit(benchmark::kMillisecond)->Ranges({{1, 512}, {0, 1}})->Complexity();

BENCHMARK(BM_FT_ExampleModel_sameFT)->Unit(benchmark::kMillisecond)->Ranges({{1, 512}, {0, 1}})->Complexity();
