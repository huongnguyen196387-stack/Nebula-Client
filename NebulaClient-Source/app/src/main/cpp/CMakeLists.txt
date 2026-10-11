cmake_minimum_required(VERSION 3.22.1)
project("nebulaclient")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -frtti")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(
    nebulaclient
    SHARED
    jni_main.cpp
    modules/hook_module.cpp
    modules/renderer_module.cpp
    modules/feature_module.cpp
    modules/opengl_test_harness.cpp
)

find_library(log-lib log)
find_library(gles-lib GLESv2)
find_library(EGL-lib EGL)

target_link_libraries(
    nebulaclient
    ${log-lib}
    ${gles-lib}
    ${EGL-lib}
)
