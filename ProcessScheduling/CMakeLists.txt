cmake_minimum_required(VERSION 3.6)
project(ProcessScheduling)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Core.cpp
        Core.h
        Disk.cpp
        Disk.h
        Process.cpp
        Process.h
        ProcessLocation.h
        ProcessScheduling.cpp
        Task.cpp
        Task.h
        TaskType.h)

add_executable(ProcessScheduling ${SOURCE_FILES})