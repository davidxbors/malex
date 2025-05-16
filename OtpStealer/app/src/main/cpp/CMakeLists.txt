cmake_minimum_required(VERSION 3.4.1)

add_library(
        native-lib
        SHARED
        native-lib.cpp
)

# Find required libraries
find_library(log-lib log)
find_library(android-lib android)

# Link libraries to your native library
target_link_libraries(
        native-lib
        ${log-lib}
        ${android-lib}
)
