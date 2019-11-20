//
// Created by andrea on 20/11/19.
//

#include "ThirdPhase.h"
#include "DLogProblemInstance.h"
#include "../Math/Number.h"


void startThirdPhase(DLogProblemInstance dLogProblemInstance) {

    __mpz_struct* randomProduct = getAuxiliaryNumber(dLogProblemInstance.applicationBuffer, 0);
    __mpz_struct** listOfRandomExponent = allocateNumbersArray(dLogProblemInstance.factorBaseLength);




}