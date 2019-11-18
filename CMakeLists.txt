cmake_minimum_required(VERSION 3.10)
project(TEN-Project)

LINK_LIBRARIES("gmp")
set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c ThirdPhase.h DiscreteLogarithm.h DiscreteLogarithm_test.h GMPCommon.h MathCommon.h MathCommon_test.h Logger/Logger.c Logger/Logger.h Error/Error.c Error/Error.h DataStructures/UnorderedList.c DataStructures/UnorderedList.h FactorBase.h NumberBuffer.h NumberBuffer_test.h ControlBlock.h ThirdPhase.h)