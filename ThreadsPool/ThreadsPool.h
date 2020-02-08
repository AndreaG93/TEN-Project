#pragma once

pthread_t *startThreadPool(unsigned long long poolSize, void *(*thread_routine)(void *), void *thread_argument);