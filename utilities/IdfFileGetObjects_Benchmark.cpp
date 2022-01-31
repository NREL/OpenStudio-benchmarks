#include <benchmark/benchmark.h>

#include <openstudio/utilities/idf/IdfFile.hpp>

#include <openstudio/utilities/core/Filesystem.hpp>
#include <openstudio/utilities/core/Assert.hpp>

#include <openstudio/utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <algorithm>

using namespace openstudio;

static void BM_GetObjectsByType_Ori(benchmark::State& state) {

  path idfPath = resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.idf");
  IdfFile idfFile = IdfFile::load(idfPath).get();
  std::vector<IdfObject> objects = idfFile.objects();

  for (auto _ : state) {

    for (const IddObjectType objectType :
         {IddObjectType::Construction, IddObjectType::RunPeriod, IddObjectType::Material, IddObjectType::Schedule_Compact}) {
      IdfObjectVector result;
      for (const auto& idfObject : objects) {
        if (idfObject.iddObject().type() == objectType) {
          result.push_back(idfObject);
        }
      }
      benchmark::DoNotOptimize(result);
    }
  }
}

static void BM_GetObjectsByType_EraseRemoveIf(benchmark::State& state) {

  path idfPath = resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.idf");
  IdfFile idfFile = IdfFile::load(idfPath).get();
  std::vector<IdfObject> objects = idfFile.objects();

  for (auto _ : state) {

    for (const IddObjectType objectType :
         {IddObjectType::Construction, IddObjectType::RunPeriod, IddObjectType::Material, IddObjectType::Schedule_Compact}) {
      IdfObjectVector result = objects;
      result.erase(
        std::remove_if(result.begin(), result.end(), [&objectType](const auto& idfObject) { return idfObject.iddObject().type() != objectType; }),
        result.end());
      benchmark::DoNotOptimize(result);
    }
  }
}

static void BM_GetObjectsByType_Reserve(benchmark::State& state) {

  path idfPath = resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.idf");
  IdfFile idfFile = IdfFile::load(idfPath).get();
  std::vector<IdfObject> objects = idfFile.objects();

  for (auto _ : state) {
    for (const IddObjectType objectType :
         {IddObjectType::Construction, IddObjectType::RunPeriod, IddObjectType::Material, IddObjectType::Schedule_Compact}) {
      IdfObjectVector result;
      result.reserve(objects.size());
      for (const auto& idfObject : objects) {
        if (idfObject.iddObject().type() == objectType) {
          result.push_back(idfObject);
        }
      }
      benchmark::DoNotOptimize(result);
    }
  }
}

BENCHMARK(BM_GetObjectsByType_Ori)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_GetObjectsByType_EraseRemoveIf)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_GetObjectsByType_Reserve)->Unit(benchmark::kMillisecond);
