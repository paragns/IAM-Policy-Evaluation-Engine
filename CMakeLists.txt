cmake_minimum_required(VERSION 3.14)
project(IAMPolicyEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(third_party)

add_executable(iam_engine
    src/main.cpp
    src/PolicyLoader.cpp
    src/PolicyEngine.cpp
)
