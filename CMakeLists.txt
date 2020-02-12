cmake_minimum_required(VERSION 3.10)
project(TEN-Project)

LINK_LIBRARIES("gmp")
LINK_LIBRARIES("pthread")
LINK_LIBRARIES("m")
set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c IndexCalculusAlgorithm/DiscreteLogarithm.h Buffers/NumbersBuffer.h Math/Factor.c Math/Factor.h Math/OrderedFactorList.c Math/OrderedFactorList.h Math/Number.c Math/Number.h Math/Factorization.c Math/Factorization.h Buffers/NumbersBuffer.c IndexCalculusAlgorithm/AlgorithmSteps/ThirdStep.c IndexCalculusAlgorithm/AlgorithmSteps/ThirdStep.h IndexCalculusAlgorithm/DLogProblemInstance.c IndexCalculusAlgorithm/DLogProblemInstance.h IndexCalculusAlgorithm/DiscreteLogarithm.c Math/RandomNumber.c Math/RandomNumber.h IndexCalculusAlgorithm/AlgorithmSteps/SecondStep.c IndexCalculusAlgorithm/AlgorithmSteps/SecondStep.h Math/Common.c Math/Common.h IndexCalculusAlgorithm/FactorBase.c IndexCalculusAlgorithm/FactorBase.h Buffers/CircularBuffer.c Buffers/CircularBuffer.h IndexCalculusAlgorithm/AlgorithmSteps/FirstStep.c IndexCalculusAlgorithm/AlgorithmSteps/FirstStep.h Math/Matrix.c Math/Matrix.h ThreadsPool/ThreadsPool.c ThreadsPool/ThreadsPool.h IndexCalculusAlgorithm/RelationsRetrieval.c IndexCalculusAlgorithm/RelationsRetrieval.h IndexCalculusAlgorithm/UserInput.c IndexCalculusAlgorithm/UserInput.h Error.c Error.h Test/Tests.h)