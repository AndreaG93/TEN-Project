cmake_minimum_required(VERSION 3.10)
project(TEN-Project)

LINK_LIBRARIES("gmp")
set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c Logger/Logger.c Logger/Logger.h Error/Error.c Error/Error.h Algorithms/PollardSRho.c Algorithms/PollardSRho.h DataStructures/UnorderedList.c DataStructures/UnorderedList.h routines.c routines.h routine_test.h)