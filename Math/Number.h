#pragma once

__mpz_struct *allocateNumber();

__mpz_struct *allocateAndSetULLNumber(unsigned long long number);

__mpz_struct *allocateAndSetNumber(__mpz_struct *number);