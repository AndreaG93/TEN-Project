#pragma once

#define COMPARE_AND_SWAP(dataPointer, expectedValue, desiredValue) __sync_bool_compare_and_swap(dataPointer, expectedValue, desiredValue)
