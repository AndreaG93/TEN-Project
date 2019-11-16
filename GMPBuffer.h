//
// Created by Andrea on 15/11/2019.
//

#ifndef TEN_PROJECT_GMPBUFFER_H
#define TEN_PROJECT_GMPBUFFER_H

__mpz_struct **initGMPBuffer();

__mpz_struct *getNumberFromGMPBuffer(__mpz_struct **buffer, int index);

void freeGMPBuffer(__mpz_struct **buffer);

#endif //TEN_PROJECT_GMPBUFFER_H
