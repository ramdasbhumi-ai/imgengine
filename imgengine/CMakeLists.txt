cmake_minimum_required(VERSION 3.10)
project(imgengine C)

# --- 1. GLOBAL STANDARDS ---
set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --- 2. PRINCIPAL-LEVEL COMPILER FLAGS ---
# We use Clang-specific hardening and SIMD optimizations
set(IMG_C_FLAGS "-O3 -march=native -fno-plt")
# set(IMG_C_FLAGS "-O3 -march=native -fno-plt -fvisibility=hidden -Wall -Wextra -Werror")

# Enable Sanitizers for Security (ASAN = Address, UBSAN = Undefined Behavior)
set(IMG_SAN_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${IMG_C_FLAGS} ${IMG_SAN_FLAGS}")

# --- 3. ARCHITECTURE DETECTION ---
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
    set(ARCH_DIR "arch/x86_64")
    add_definitions(-DARCH_X86_64)
    message(STATUS "[CORE] Target Architecture: x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(ARCH_DIR "arch/aarch64")
    add_definitions(-DARCH_AARCH64)
    message(STATUS "[CORE] Target Architecture: aarch64")
else()
    set(ARCH_DIR "arch/scalar")
    message(STATUS "[CORE] Target Architecture: Generic Scalar")
endif()

# --- 4. INCLUDE PATHS ---
include_directories(
    .
    api/v1
    src
    include
    memory
    runtime
    pipeline
    plugins
    io
    security
)

# --- 5. EXTERNAL DEPENDENCIES ---
find_package(Threads REQUIRED)
find_path(TURBOJPEG_INCLUDE_DIR NAMES turbojpeg.h)
find_library(TURBOJPEG_LIB NAMES turbojpeg)

# if(NOT TURBOJPEG_LIB)
#     message(FATAL_ERROR "libturbojpeg not found. Run: sudo apt-get install libturbojpeg0-dev")
# endif()

# --- 6. SOURCE COLLECTION (Filtered by Silicon) ---
file(GLOB_RECURSE ENGINE_SOURCES 
    "src/*.c" 
    "pipeline/*.c" 
    "plugins/*.c" 
    "io/*.c" 
    "runtime/*.c" 
    "memory/*.c"
    "security/*.c"
    "observability/*.c" # ADD THIS
    "${ARCH_DIR}/*.c"
)

# --- 7. TARGETS ---

# A. The Shared Library (Kernel-Grade Core)
add_library(imgengine SHARED ${ENGINE_SOURCES})
target_include_directories(imgengine PRIVATE ${TURBOJPEG_INCLUDE_DIR})
target_link_libraries(imgengine PRIVATE Threads::Threads ${TURBOJPEG_LIB} m "-fsanitize=address,undefined")

# B. The Production CLI Tool
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmd/imgengine/main.c")
    add_executable(imgengine_cli cmd/imgengine/main.c cmd/imgengine/args.c)
    target_link_libraries(imgengine_cli PRIVATE imgengine)
endif()

# C. The Latency Benchmarker
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmd/bench/lat_bench.c")
    add_executable(bench_lat cmd/bench/lat_bench.c)
    target_link_libraries(bench_lat PRIVATE imgengine)
endif()
