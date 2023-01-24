# OpenStudio-benchmarks

A collection of microbenchmarks for the [OpenStudio SDK](https://github.com/NREL/OpenStudio).

```
mkdir build && cd build
# For example:
cmake -Dopenstudio_DIR=/usr/local/openstudio-3.3.0/lib/cmake/openstudio/ -G Ninja -DCMAKE_BUILD_TYPE:STRING=Release .
# or from a build directory after building the package
cmake -Dopenstudio_DIR=$os_build_rel2/_CPack_Packages/Darwin/TGZ/OpenStudio-3.5.1-alpha+3291a14888-Darwin-arm64/lib/cmake/openstudio/ -G Ninja -DCMAKE_BUILD_TYPE:STRING=Release .
ninja
```

You can override which libopenstudiolib.so is used after it's built

```
LD_LIBRARY_PATH="/usr/local/openstudio-3.2.1/lib/" ./Products/model/Model_Benchmark
```

Run all:

```
cd build
python run_benchmark.py --quiet --prefix ori --all
```
