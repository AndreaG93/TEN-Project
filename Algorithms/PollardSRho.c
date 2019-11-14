//
// Created by Andrea on 14/11/2019.
//

#include "PollardSRho.h"

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

/*
typedef struct list {
    void *data;
    struct list *next;
} List;

List *insert(void *data, List *next)
{
    List *new;

    new = malloc(sizeof(List));
    new->data = data;
    new->next = next;
    return new;
}

List *insert_in_order(void *x, List *xs)
{
    if (xs == NULL || mpz_cmp(x, xs->data) < 0)
    {
        return insert(x, xs);
    }
    else
    {
        List *head = xs;
        while (xs->next != NULL && mpz_cmp(x, xs->next->data) < 0)
        {
            xs = xs->next;
        }
        xs->next = insert(x, xs->next);
        return head;
    }
}

void rho_factor(mpz_t f, mpz_t n, long long unsigned c)
{
    mpz_t t, h, d, r;

    mpz_init_set_ui(t, 2);
    mpz_init_set_ui(h, 2);
    mpz_init_set_ui(d, 1);
    mpz_init_set_ui(r, 0);

    while (mpz_cmp_si(d, 1) == 0)
    {
        mpz_mul(t, t, t);
        mpz_add_ui(t, t, c);
        mpz_mod(t, t, n);

        mpz_mul(h, h, h);
        mpz_add_ui(h, h, c);
        mpz_mod(h, h, n);

        mpz_mul(h, h, h);
        mpz_add_ui(h, h, c);
        mpz_mod(h, h, n);

        mpz_sub(r, t, h);
        mpz_gcd(d, r, n);
    }

    if (mpz_cmp(d, n) == 0)
    {
        rho_factor(f, n, c+1);
    }
    else if (mpz_probab_prime_p(d, 25))
    {
        mpz_set(f, d);
    }
    else
    {
        rho_factor(f, d, c+1);
    }

    mpz_clears(t, h, d, r, NULL);
}

void rho_factors(List **fs, mpz_t n)
{
    while (! (mpz_probab_prime_p(n, 25)))
    {
        mpz_t *f = malloc(sizeof(*f));
        mpz_init_set_ui(*f, 0);

        rho_factor(*f, n, 1);
        *fs = insert_in_order(*f, *fs);
        mpz_divexact(n, n, *f);
    }

    *fs = insert_in_order(n, *fs);
}

int mainfdsfds(int argc, char *argv[])
{
    mpz_t f, n;
    mpz_init_set_ui(f, 0);
    List *fs = NULL;

    if (argc<2)
    {
        printf("usage: rho n\n");
        return 1;
    }

    mpz_init_set_str(n, argv[1], 10);

    if (mpz_probab_prime_p(n, 25))
    {
        printf("%s is prime\n", argv[1]);
        return 0;
    }

    printf("Factors of %s:", argv[1]);
    rho_factors(&fs, n);
    while (fs != NULL) {
        printf(" %s", mpz_get_str(NULL, 10, fs->data));
        fs = fs->next;
    }
    printf("\n");

    return 0;
}
 */