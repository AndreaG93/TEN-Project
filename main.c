#include "IndexCalculusAlgorithm/DLogProblemInstance.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/ThirdStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/SecondStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/FirstStep.h"
#include "IndexCalculusAlgorithm/UserInput.h"
#include "Test/Tests.h"

#define MAX_RANDOM_INTEGER 2
#define NUMBER_BUFFER_LENGTH 25
#define POOL_SIZE 1

RawUserInput rawUserInput;

void functionxgcd(mpz_t old_r, mpz_t r, mpz_t quotient){
    mpz_t prov;
    mpz_init(prov);
    mpz_set(prov, r);

    mpz_t temp;
    mpz_init(temp);
    mpz_mul(temp, quotient, prov);

    mpz_sub(r, old_r, temp);
    mpz_set(old_r, prov);

    mpz_clear(prov);
    mpz_clear(temp);
}

int xgcd(mpz_t a, mpz_t b,mpz_t compare){


    mpz_t s;
    mpz_init(s);
    mpz_set_ui(s, 0);
    mpz_t t;
    mpz_init(t);
    mpz_set_ui(t, 1);
    mpz_t r;
    mpz_init(r);
    mpz_set(r, b);
    mpz_t old_s;
    mpz_init(old_s);
    mpz_set_ui(old_s, 1);
    mpz_t old_t;
    mpz_init(old_t);
    mpz_set_ui(old_t, 0);
    mpz_t old_r;
    mpz_init(old_r);
    mpz_set(old_r, a);
    mpz_t temp1;
    mpz_init(temp1);
    mpz_t temp2;
    mpz_init(temp2);
    mpz_t quotient;
    mpz_init(quotient);
    int times = 0;

    while(true){
        //while(time < 3){

        if(mpz_cmp_ui(r , 0) == 0){
            mpz_clear(s);
            mpz_clear(t);
            mpz_clear(r);
            mpz_clear(old_s);
            mpz_clear(old_t);
            mpz_clear(old_r);
            mpz_clear(temp1);
            mpz_clear(temp2);
            mpz_clear(quotient);
            //printf("return 0 in %d times\n", times);
            return 0;
        }
        mpz_fdiv_q(quotient, old_r, r);
        //printmpz("quotient = ", quotient);
        functionxgcd(old_r, r, quotient);
        functionxgcd(old_s, s, quotient);
        functionxgcd(old_t, t, quotient);

        times ++;

        gmp_fprintf(stderr, "(%Zd) * 46617753060 + (%Zd) * 97011687217 = (%Zd)\n", old_s, old_t, old_r);
    }

    gmp_fprintf(stderr, "old_r %Zd\n", old_r);
    gmp_fprintf(stderr, "old_s %Zd\n", old_s);
    gmp_fprintf(stderr, "old_t %Zd\n", old_t);



    //stampe
    /*
    pthread_mutex_lock(&lock);
    printf("old_s * a + old_t * b = old_r\n");
    mpz_t tempmul;
    mpz_init(tempmul);
    mpz_mul(tempmul, result[1], a);
    mpz_t tempmul2;
    printmpz("old_s = ", result[1]);
    printmpz("a = ", a);
    printmpz("old_t = ", result[2]);
    printmpz("b = ", b);
    printmpz("old_s * a = ", tempmul);
    mpz_init(tempmul2);
    mpz_mul(tempmul2, result[2], b);
    printmpz("old_t * b = ", tempmul2);
    mpz_add(tempmul, tempmul, tempmul2);
    printmpz("result[0] = ", result[0]);
    printmpz("risultato vero = ", tempmul);
    sleep(1);
    pthread_mutex_unlock(&lock);
*/

    mpz_clear(s);
    mpz_clear(t);
    mpz_clear(r);
    mpz_clear(old_s);
    mpz_clear(old_t);
    mpz_clear(old_r);
    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(quotient);

//	printf("return 1 in %d times\n", times);
    return 1;

}





int main(int argc, char **argv) {

    /*
    __mpz_struct* rwrew = allocateAndSetNumberFromString("46617753060");
    __mpz_struct* modulo = allocateAndSetNumberFromString("97011687217");
    __mpz_struct* mddd = allocateNumber();
    mpz_sqrt(mddd, modulo);
    xgcd(rwrew, modulo, mddd);

    return 0;
*/


#ifdef DEBUG
    computeOptimalSmoothnessBound();
    pollardRhoTest();
    factorizationTest();
    factorizationCheckingBSmoothnessTest();
    relationGenerationTest();
    numberAllocationDeAllocation();
    foundGenerator();
    return 0;
#endif

#ifdef RELEASE
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s [dLogBase] [dLogArgument] [multiplicativeGroup] [ (OPTIONAL) smoothnessBound]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    rawUserInput.dLogBase = argv[1];
    rawUserInput.dLogArgument = argv[2];
    rawUserInput.multiplicativeGroup = argv[3];

    if (argc == 5)
        rawUserInput.smoothnessBound = argv[4];
    else
        rawUserInput.smoothnessBound = NULL;

#endif
#ifndef RELEASE

    rawUserInput.dLogBase = "10";
    rawUserInput.dLogArgument = "150";
    rawUserInput.multiplicativeGroup = "97011687217";
    rawUserInput.smoothnessBound = NULL;

#endif


    DLogProblemInstanceInput *input = sanitizeRawUserInput(&rawUserInput, MAX_RANDOM_INTEGER, NUMBER_BUFFER_LENGTH);
    DLogProblemInstance *dLogProblemInstance = allocateDLogProblemInstance(input, POOL_SIZE);
    free(input);

    gmp_fprintf(stderr, "[INFO] Start computing log_(%Zd) (%Zd) in Z*_(%Zd)\n", dLogProblemInstance->discreteLogarithm->base, dLogProblemInstance->discreteLogarithm->argument, dLogProblemInstance->discreteLogarithm->multiplicativeGroup);
    gmp_fprintf(stderr, "[INFO] Smoothness Bound set to %Zd\n", dLogProblemInstance->smoothnessBound);


    fprintf(stderr, "Start 1° algorithm step...\n");
    startFirstStep(dLogProblemInstance);
    fprintf(stderr, "Start 2° algorithm step...\n");
    startSecondStep(dLogProblemInstance);
    fprintf(stderr, "Start 3° algorithm step...\n");
    startThirdStep(dLogProblemInstance);

    DiscreteLogarithm *output = dLogProblemInstance->discreteLogarithm;

    if (isCorrect(output)) {
        fprintf(stdout, "== SUCCESS ==\n");
        gmp_printf("--> In Z_(%Zd)\n", output->multiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) = %Zd\n", output->base, output->argument, output->value);
    } else {
        fprintf(stdout, "== FAILURE ==\n");
        gmp_printf("--> In Z_(%Zd)\n", output->multiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) != %Zd\n", output->base, output->argument, output->value);
    }

    freeDLogProblemInstance(dLogProblemInstance);

    return 0;
}

// SOME PRIME USED FOR TESTS
// ================================================
// Prime                    | Smallest Generator
// ================================================
// 97011687217              | 10
// 48112959837082048697     | 3