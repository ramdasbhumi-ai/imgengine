### imgengine/CMakeLists.txt

## 1. Final check on dependencies

    sudo apt-get update && sudo apt-get upgrade
    sudo apt-get install -y libturbojpeg0-dev libnuma-dev clang

## 2. Build the engine

    This will take ~10-20 seconds as LTO (Link Time Optimization) crunches the kernels

    make -j$(nproc)

# 3. Set Library Path (Crucial for Linux to find your .so)

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

# 4. FIRST TEST RUN (The "Golden Moment")

# Replace with a real path to a jpeg in your workspace

./imgengine -i tests/samples/input.jpg -o out.jpg -w 800 -h 600 -v

### how to run in github-codespaaces

    <!-- rm -rf * -->
    cd /workspaces/imgengine/imgengine
    mkdir build && cd build
    rm -rf *
    cmake ..
    make -j$(nproc)



bash
cd /workspaces/imgengine/imgengine/build
cmake ..
make -j$(nproc)

# Execute the benchmark
./bench_lat