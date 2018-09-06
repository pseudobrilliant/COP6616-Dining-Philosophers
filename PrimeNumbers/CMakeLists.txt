cmake_minimum_required(VERSION 3.5)
project(PrimeNumbers)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

set(CMAKE_CXX_FLAGS -pthread)

add_executable(PrimeNumbers src/main.cpp headers/Solution.h src/SequentialSolution.cpp headers/SequentialSolution.h headers/Prime.h headers/Prime.h src/Solution.cpp src/TicketSolution.cpp headers/TicketSolution.h src/SiloSolution.cpp headers/SiloSolution.h)
