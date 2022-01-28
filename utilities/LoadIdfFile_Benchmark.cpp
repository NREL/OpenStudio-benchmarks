#include <benchmark/benchmark.h>

#include <openstudio/utilities/idf/IdfFile.hpp>

#include <openstudio/utilities/core/Filesystem.hpp>
#include <openstudio/utilities/core/Assert.hpp>

#include <openstudio/resources.hxx>
#include <openstudio/OpenStudio.hxx>

using namespace openstudio;

static void BM_LoadIdfFile(benchmark::State& state, const std::string& testCase) {

  path idfPath = resourcesPath() / toPath(testCase);

  for (auto _ : state) {
    OptionalIdfFile oIdfFile = IdfFile::load(idfPath);
  }
}

BENCHMARK_CAPTURE(BM_LoadIdfFile, 5ZoneAirCooled, std::string("energyplus/5ZoneAirCooled/in.idf"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, Daylighting_School, std::string("energyplus/Daylighting_School/in.idf"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, HospitalBaseline, std::string("energyplus/HospitalBaseline/in.idf"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, Office_With_Many_HVAC_Types, std::string("energyplus/Office_With_Many_HVAC_Types/in.idf"))
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, RefBldgLargeOffice, std::string("energyplus/RefLargeOffice/RefBldgLargeOfficeNew2004_Chicago.idf"))
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_LoadIdfFile, SmallOffice, std::string("energyplus/SmallOffice/SmallOffice.idf"))->Unit(benchmark::kMillisecond);
