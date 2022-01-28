# OpenStudio-benchmarks

A collection of microbenchmarks for the [OpenStudio SDK](https://github.com/NREL/OpenStudio).

```
mkdir build && cd build
openstudio_DIR=/usr/local/openstudio-3.3.0/lib/cmake/openstudio/ cmake -G Ninja ..
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
