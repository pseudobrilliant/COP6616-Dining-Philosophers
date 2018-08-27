cmake_minimum_required(VERSION 3.12)
project(COP6616_Assignment1)

set(CMAKE_CXX_STANDARD 14)

add_executable(COP6616_Assignment1 src/main.cpp headers/Solution.h src/SequentialSolution.cpp headers/SequentialSolution.h headers/Prime.h headers/Prime.h src/Solution.cpp src/TicketSolution.cpp headers/TicketSolution.h)

include_directories(COP6616_Assignment1 ./3rdParty)
target_link_libraries(COP6616_Assignment1 gmp)