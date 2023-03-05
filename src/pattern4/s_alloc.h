//
// Created by Denis BEURIVE on 04/03/2023.
//

#ifndef C_PATTERNS_S_ALLOC_H
#define C_PATTERNS_S_ALLOC_H

#include "common.h"


void
s_alloc_init(
        long in_id_failure,
        long in_count_success,
        const char *in_dump_path,
        Bool in_exit_on_data_recording_error);

Status
s_malloc(
        void **in_ptr,
        long in_id,
        size_t in_size,
        Bool in_initialize,
        const char *in_file,
        unsigned long in_line,
        const char *in_function);

void
s_free(
        void **in_ptr,
        const char *in_file,
        unsigned long in_line,
        const char *in_function);

Status
s_realloc(
        void **in_ptr,
        long in_id,
        size_t in_new_size,
        const char *in_file,
        unsigned long in_line,
        const char *in_function);

#endif //C_PATTERNS_S_ALLOC_H
