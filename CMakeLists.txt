cmake_minimum_required(VERSION 3.10)
project(TEN-Project)

LINK_LIBRARIES("gmp")
set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c GMPCommon/GMPNumberOrderedList.c GMPCommon/GMPNumberOrderedList.h ThirdPhase.h DiscreteLogarithm.h DiscreteLogarithm_test.h ApplicationBuffer/ApplicationBuffer.h MathCommon.h MathCommon_test.h Logger/Logger.c Logger/Logger.h Error/Error.c Error/Error.h DataStructures/UnorderedList.c DataStructures/UnorderedList.h FactorBase.h Math/NumberBuffer.h Math/NumberBuffer_test.h ControlBlock.h ThirdPhase.h GMPCommon/GMPNumberOrderedList.c GMPCommon/GMPNumberOrderedList.h Math/Factor.c Math/Factor.h Math/OrderedFactorList.c Math/OrderedFactorList.h Math/NumberBuffer.h Math/Number.c Math/Number.h Math/Factorization.c Math/Factorization.h ApplicationBuffer/ApplicationBuffer.c)