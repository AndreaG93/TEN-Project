//
// Created by Andrea on 15/11/2019.
//

#ifndef TEN_PROJECT_GMPBUFFER_H
#define TEN_PROJECT_GMPBUFFER_H

mpz_ptr* initGMPBuffer();
mpz_ptr getNumberFromGMPBuffer(mpz_ptr* buffer, int index);
void freeGMPBuffer(mpz_ptr *buffer);

#endif //TEN_PROJECT_GMPBUFFER_H
