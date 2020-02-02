cmake_minimum_required(VERSION 3.10)
project(TEN-Project)

LINK_LIBRARIES("gmp")
LINK_LIBRARIES("pthread")
set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c DLogIndexMethod/DiscreteLogarithm.h Buffers/NumbersBuffer.h Math/Factor.c Math/Factor.h Math/OrderedFactorList.c Math/OrderedFactorList.h Math/Number.c Math/Number.h Math/Factorization.c Math/Factorization.h Buffers/NumbersBuffer.c DLogIndexMethod/ThirdPhase.c DLogIndexMethod/ThirdPhase.h DLogIndexMethod/DLogProblemInstance.c DLogIndexMethod/DLogProblemInstance.h DLogIndexMethod/DiscreteLogarithm.c Math/RandomNumber.c Math/RandomNumber.h DLogIndexMethod/SecondPhase.c DLogIndexMethod/SecondPhase.h Math/Common.c Math/Common.h DLogIndexMethod/FactorBase.c DLogIndexMethod/FactorBase.h Buffers/CircularBuffer.c Buffers/CircularBuffer.h DLogIndexMethod/FirstPhase.c DLogIndexMethod/FirstPhase.h Math/Matrix.c Math/Matrix.h ThreadsPool/ThreadsPool.c ThreadsPool/ThreadsPool.h DLogIndexMethod/RelationsRetrieval.c DLogIndexMethod/RelationsRetrieval.h)