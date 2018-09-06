cmake_minimum_required(VERSION 3.12)
project(DiningPhilosophers)

set(CMAKE_CXX_STANDARD 14)

add_executable(DiningPhilosophers_Version1 src/Solution1.cpp headers/Philosopher.h)

add_executable(DiningPhilosophers_Version2 src/Solution2.cpp headers/Philosopher.h)

add_executable(DiningPhilosophers_Version3 src/Solution3.cpp headers/Philosopher.h src/Host.cpp headers/Host.h)

add_executable(DiningPhilosophers_Version4 src/Solution4.cpp headers/Philosopher.h src/Host.cpp headers/Host.h)