#include "DLogProblemInstance.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"
#include "../Math/Number.h"
#include "../ThreadsPool/ThreadsPool.h"

void g2(mpz_t rop, mpz_t num, mpz_t p) {
    mpz_pow_ui(rop, num, 2);
    mpz_add_ui(rop, rop, 1);
    mpz_mod(rop, rop, p);
}

int pollardRho2(mpz_t rop, mpz_t num, int ttl) {

    mpz_t x, y, sub;

    mpz_init(x);
    mpz_init(y);
    mpz_init(sub);

    mpz_set_ui(x, 2);
    mpz_set_ui(y, 2);
    mpz_set_ui(rop, 1);

    int counter = ttl;

    while (mpz_cmp_ui(rop, 1) == 0) {

        if (counter == 0) {
            mpz_set_ui(rop, 0);
            return 0;
        }

        g2(x, x, num);
        g2(y, y, num);
        g2(y, y, num);
        mpz_sub(sub, x, y);
        mpz_abs(sub, sub);
        mpz_gcd(rop, sub, num);

        if (mpz_cmp(rop, num) == 0) {
            mpz_set_ui(rop, 0);
            return 0;
        }
        counter--;
    }
    return 1;
}


int isBSmooth2(mpz_t arg, double smoothness, int ttl, OrderedFactorList *array) {

    mpz_t x;
    mpz_init(x);
    mpz_set(x, arg);
    if (mpz_cmp_ui(x, 0) < 0) {
        mpz_t temp;
        mpz_init(temp);
        mpz_mul_si(temp, x, -2);
        mpz_add(x, temp, x);
        mpz_clear(temp);
    }

    if (mpz_cmp_ui(x, 0) == 0) {
        printf("x = 0\n");
        return 0;
    }

    if (mpz_cmp_ui(x, 1) == 0) {
        return 0;
    }


    //while the number is not prime
    mpz_t value;
    mpz_init(value);

    int v;

    while (mpz_probab_prime_p(x, 15) == 0) {
        v = pollardRho2(value, x, ttl);
        if (v == 0) {
            return 0;
        }
        while (mpz_probab_prime_p(value, 15) == 0) {
            v = pollardRho2(value, value, ttl);
            if (v == 0) {
                return 0;
            }
        }

        if (mpz_cmp_d(value, smoothness) > 0) {
            mpz_clear(value);
            return 0;
        }
        mpz_cdiv_q(x, x, value);
        insertFactor(array, allocateAndSetNumberFromNumber(value), allocateAndSetNumberFromULL(1));
    }
    if (mpz_cmp_d(x, smoothness) > 0) {
        mpz_clear(value);
        return 0;
    }
    insertFactor(array, allocateAndSetNumberFromNumber(x), allocateAndSetNumberFromULL(1));

    mpz_clear(value);
    mpz_clear(x);
    return 1;
}


void functionxgcd(mpz_t old_r, mpz_t r, mpz_t quotient) {
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

int xgcd(mpz_t *result, mpz_t a, mpz_t b, mpz_t compare) {


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

    while (mpz_cmp(old_r, compare) > 0) {
        //while(time < 3){

        if (mpz_cmp_ui(r, 0) == 0) {
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

        times++;
    }

    mpz_set(result[0], old_r);
    mpz_set(result[1], old_s);
    mpz_set(result[2], old_t);


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


OrderedFactorList *
own_gcd(/*ExpArray *expArray, */mpz_t p, mpz_t y, mpz_t mpzaccettable, mpz_t mpzaccettablehalf, mpz_t sqrtB, /*int mul*/
                                mpz_t smoothness, /* Array *a,*/ int mode, OrderedFactorList* original) {

    //printmpz("mpzaccettable = ", mpzaccettable);

    __mpz_struct *input = allocateAndSetNumberFromNumber(y);

    __mpz_struct *input0 = allocateAndSetNumberFromNumber(y);
    __mpz_struct *input1 = allocateAndSetNumberFromNumber(y);
    __mpz_struct *input2 = allocateAndSetNumberFromNumber(y);

    mpz_t *result = malloc(3 * sizeof(mpz_t));
    mpz_init(result[0]);
    mpz_init(result[1]);
    mpz_init(result[2]);

    //ExpArray *expArray1;
    //ExpArray *expArray2;

    OrderedFactorList *numerator = allocateOrderedFactorList();
    OrderedFactorList *denominator = allocateOrderedFactorList();

    if (mode == 0) {

        if (xgcd(result, p, y, mpzaccettable) == 1) {

            mpz_set(input0, result[0]);
            mpz_set(input1, result[1]);
            mpz_set(input2, result[2]);

            if (isBSmooth2(result[2], mpz_get_d(smoothness), mpz_get_d(sqrtB), denominator)) {

                //freeArray(a);
                //a = malloc(sizeof(Array));
                //initArray(a, mul);

                if (isBSmooth2(result[0], mpz_get_d(smoothness), mpz_get_d(sqrtB), numerator)) {

                    printOrderedFactorList(original);
                    printOrderedFactorList(numerator);
                    printOrderedFactorList(denominator);

                    gmp_fprintf(stderr, "[INFO] %Zd\n", input);

                    OrderedFactorList *ou = mergeOrderedFactorListUsingOptimization(numerator, denominator);
                    freeOrderedFactorList(numerator);
                    freeOrderedFactorList(denominator);
                    //printOrderedFactorList(outputPointer);
                    return ou;
                    /*
                    //subExpArray(expArray, expArray1, expArray2);
                    if (mpz_cmp_d(result[0], 0) < 0 && mpz_cmp_d(result[2], 0) >= 0) {
                        //freed(a, expArray1, expArray2, mul, result);
                        return 1;
                    } else if (mpz_cmp_d(result[0], 0) >= 0 && mpz_cmp_d(result[2], 0) < 0) {
                        //freed(a, expArray1, expArray2, mul, result);
                        return -1;
                    } else {
                        //freed(a, expArray1, expArray2, mul, result);
                        return 0;
                    }*/
                } else {
                    freeOrderedFactorList(numerator);
                    freeOrderedFactorList(denominator);
                    return NULL;
                }
            } else {
                freeOrderedFactorList(numerator);
                freeOrderedFactorList(denominator);
                return NULL;
            }
        }


        freeOrderedFactorList(numerator);
        freeOrderedFactorList(denominator);

        //freed(a, expArray1, expArray2, mul, result);
        return NULL;
    } else {
        /*
        if (xgcd(result, p, y, mpzaccettable) == 1) {

            if (own_gcd(expArray1, p, result[0], mpzaccettablehalf, mpzaccettablehalf, sqrtB, mul, smoothness, a, 0) !=
                -10) {
                if (own_gcd(expArray2, p, result[2], mpzaccettablehalf, mpzaccettablehalf, sqrtB, mul, smoothness, a,
                            0) != -10) {

                    subExpArray(expArray, expArray1, expArray2);
                    if (mpz_cmp_d(result[0], 0) < 0 && mpz_cmp_d(result[2], 0) >= 0) {
                        freed(a, expArray1, expArray2, mul, result);
                        return 1;
                    } else if (mpz_cmp_d(result[0], 0) >= 0 && mpz_cmp_d(result[2], 0) < 0) {
                        freed(a, expArray1, expArray2, mul, result);
                        return -1;
                    } else {
                        freed(a, expArray1, expArray2, mul, result);
                        return 0;
                    }

                } else {
                    freed(a, expArray1, expArray2, mul, result);
                    return -10;
                }
            } else {
                freed(a, expArray1, expArray2, mul, result);
                return -10;
            }
        }
         */
    }
    //freed(a, expArray1, expArray2, mul, result);
    return NULL;
}


typedef struct {

    OrderedFactorList *relationLeftSide;
    OrderedFactorList *relationRightSide;

} Relation;

Relation *allocateRelation() {

    Relation *output = malloc(sizeof(Relation));

    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {
        output->relationLeftSide = NULL;
        output->relationRightSide = NULL;
    }

    return output;
}

void deallocateRelation(Relation *input) {

    freeOrderedFactorList(input->relationLeftSide);
    freeOrderedFactorList(input->relationRightSide);
    free(input);
}

Relation *
getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator,
            __mpz_struct *logarithmArgument) {

    Relation *output = allocateRelation();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 2);

    __mpz_struct *randomNumber = buffer[0];
    __mpz_struct *power = buffer[1];

    __mpz_struct *sqrtB = allocateNumber();
    mpz_sqrt(sqrtB, instance->smoothnessBound);

    mpz_t ten;
    mpz_init(ten);
    mpz_set_ui(ten, 10);

    mpz_t mpzaccettable;
    mpz_init(mpzaccettable);
    mpz_pow_ui(mpzaccettable, ten, mpz_sizeinbase(instance->discreteLogarithm->multiplicativeGroup, 10) / 2);

    mpz_t mpzaccettabledouble;
    mpz_init(mpzaccettabledouble);
    mpz_pow_ui(mpzaccettabledouble, ten, mpz_sizeinbase(mpzaccettable, 10) / 2);


    do {

        if (output->relationLeftSide != NULL)
            freeOrderedFactorList(output->relationLeftSide);

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_set(randomNumber, logarithmArgument);
        } else {
            mpz_set_ui(randomNumber, 1);
        }

        output->relationLeftSide = allocateOrderedFactorList();

        //__mpz_struct *currentPrimeNumber = allocateAndSetNumberFromNumber(instance->secondPhaseOutput->base);
        __mpz_struct *currentPrimeNumberExponent = selectUniformlyDistributedRandomInteger(randomIntegerGenerator);

        appendFactor(output->relationLeftSide, allocateAndSetNumberFromNumber(instance->discreteLogarithm->base), currentPrimeNumberExponent);

        mpz_powm(power, instance->discreteLogarithm->base, currentPrimeNumberExponent, instance->discreteLogarithm->multiplicativeGroup);
        //mpz_mul(randomNumber, randomNumber, power);
        //mpz_mod(randomNumber, randomNumber, instance->discreteLogarithm->multiplicativeGroup);

        if (instance->currentIndexCalculusAlgorithmStep == 2)
            output->relationRightSide = own_gcd(instance->discreteLogarithm->multiplicativeGroup, power,
                                                mpzaccettable, mpzaccettabledouble, sqrtB, instance->smoothnessBound,
                                                0, output->relationLeftSide);
        else {
            output->relationRightSide = allocateOrderedFactorList();
            if (isBSmooth2(randomNumber, mpz_get_d(instance->smoothnessBound), mpz_get_d(sqrtB),
                           output->relationRightSide)) {
                //printOrderedFactorList(output->relationRightSide);
            } else {
                freeOrderedFactorList(output->relationRightSide);
                output->relationRightSide = NULL;
            }

        }

    } while (output->relationRightSide == NULL);

    releaseNumbers(numbersBuffer, 2);

    return output;
}

__mpz_struct **getLogarithmRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer,
                                    RandomIntegerGenerator *randomIntegerGenerator, __mpz_struct *logarithmArgument) {

    Relation *relation = getRelation(instance, numbersBuffer, randomIntegerGenerator, logarithmArgument);

    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length, true);

    OrderedFactorListNode *currentLeftSideRelationNodeList = relation->relationLeftSide->head;
    OrderedFactorListNode *currentRightSideRelationNodeList = relation->relationRightSide->head;

    //printOrderedFactorList(relation->relationLeftSide);
    //printOrderedFactorList(relation->relationRightSide);

    int index = 0;

    for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;
         currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node, index++) {

        if (instance->currentIndexCalculusAlgorithmStep == 2) {

            if (currentLeftSideRelationNodeList != NULL && currentRightSideRelationNodeList != NULL) {

                if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0 &&
                    mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_sub(*(output + index), currentLeftSideRelationNodeList->factor->exponent,
                            currentRightSideRelationNodeList->factor->exponent);

                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

                } else if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) ==
                           0) {

                    mpz_set(*(output + index), currentLeftSideRelationNodeList->factor->exponent);
                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;

                } else if (
                        mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) ==
                        0) {

                    mpz_mul_si(*(output + index), currentRightSideRelationNodeList->factor->exponent, -1);
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

                } else
                    mpz_set_ui(*(output + index), 0);

            } else if (currentLeftSideRelationNodeList != NULL) {

                if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_set(*(output + index), currentLeftSideRelationNodeList->factor->exponent);
                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                } else
                    mpz_set_ui(*(output + index), 0);

            } else if (currentRightSideRelationNodeList != NULL) {

                if (mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_mul_si(*(output + index), currentRightSideRelationNodeList->factor->exponent, -1);
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;
                } else
                    mpz_set_ui(*(output + index), 0);

            } else
                mpz_set_ui(*(output + index), 0);
        }

        if (instance->currentIndexCalculusAlgorithmStep == 3) {

            if (currentLeftSideRelationNodeList != NULL && currentRightSideRelationNodeList != NULL) {

                if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0 &&
                    mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_sub(*(output + index), currentRightSideRelationNodeList->factor->exponent,
                            currentLeftSideRelationNodeList->factor->exponent);

                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

                } else if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) ==
                           0) {

                    mpz_mul_si(*(output + index), currentLeftSideRelationNodeList->factor->exponent, -1);
                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;

                } else if (
                        mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) ==
                        0) {

                    mpz_set(*(output + index), currentRightSideRelationNodeList->factor->exponent);
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

                } else
                    mpz_set_ui(*(output + index), 0);

            } else if (currentLeftSideRelationNodeList != NULL) {

                if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_mul_si(*(output + index), currentLeftSideRelationNodeList->factor->exponent, -1);
                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                } else
                    mpz_set_ui(*(output + index), 0);

            } else if (currentRightSideRelationNodeList != NULL) {

                if (mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_set(*(output + index), currentRightSideRelationNodeList->factor->exponent);
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;
                } else
                    mpz_set_ui(*(output + index), 0);

            } else
                mpz_set_ui(*(output + index), 0);
        }
    }

    deallocateRelation(relation);
    return output;
}

void *threadRoutineForRelationRetrieval(void *input) {

    ThreadArgument *threadArgument = (ThreadArgument *) input;
    unsigned int threadID = threadArgument->threadID;
    ThreadsPoolData *threadsPoolData = (ThreadsPoolData *) threadArgument->threadArgument;
    DLogProblemInstance *instance = (DLogProblemInstance *) threadsPoolData->dLogProblemInstance;

    NumbersBuffer *numbersBuffer = allocateNumbersBuffer(instance->numbersBuffer->size);
    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(
            instance->discreteLogarithm->multiplicativeGroup);
    CircularBuffer *circularBuffer = threadsPoolData->arrayOfCircularBuffer[threadID];

    while (threadsPoolData->stoppingCondition != true) {

        __mpz_struct **relation = getLogarithmRelation(instance, numbersBuffer, randomIntegerGenerator, NULL);
        pushIntoCircularBuffer(circularBuffer, relation);
    }

    free(threadArgument);
    freeCircularBuffer(circularBuffer);
    freeNumbersBuffer(numbersBuffer);
    freeRandomIntegerGenerator(randomIntegerGenerator);

    return NULL;
}