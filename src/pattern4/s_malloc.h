//
// Created by Denis BEURIVE on 04/03/2023.
//

#ifndef C_PATTERNS_S_MALLOC_H
#define C_PATTERNS_S_MALLOC_H

#include "common.h"

void
s_malloc_init(
        long in_id_failure,
        long in_count_success);

void *
s_malloc(
        const long in_id,
        const size_t in_size,
        Bool in_initialize);

#endif //C_PATTERNS_S_MALLOC_H
