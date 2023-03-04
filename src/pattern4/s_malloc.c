/**
 * Synopsis:
 *
 *      char *buffer = NULL;
 *      char *other = NULL;
 *
 *      // We want the call to `s_malloc()` which ID=100 to fail after 10 allocations.
 *      s_malloc_init(100, 10);
 *      ...
 *      for (int i=0; i<=10; i++) {
 *          // This is the call to `s_malloc()` with the ID 100. Allocate 200 bytes.
 *          // `s_malloc()` will return NULL where i=10.
 *          buffer = s_malloc(100, 200);
 *          if (NULL == buffer) { ... }
 *          ...
 *          // This is the call to `s_malloc()` with the ID 110. Allocate 2000 bytes.
 *          // `s_malloc()` will never return NULL (unless, of course, if the process really runs out of memory).
 *          other = s_malloc(110, 2000);
 *          ...
 *          // Free the memory pointed by `buffer`.*
 *          s_free(&buffer); // now, the value of `buffer` is *guaranteed* to be NULL.
 *          s_free(&other); // now, the value of `other` is *guaranteed* to be NULL.
 *          ...
 *          if (...) {
 *              // The 2 lines below will not cause any trouble.
 *              s_free(&buffer); // it might seem useless or risky (it's debatable), but it does not harm
 *              s_free(&other); // it might seem useless or risky (it's debatable), but it does not harm
 *              return;
 *          }
 *      }
 */

#include <stdlib.h>
#include <string.h>
#include "s_malloc.h"

static long MALLOC_ID=-1;
static long MALLOC_COUNT=0;
static long COUNT=0;

/**
 * @brief Initialize the secure "malloc library".
 * @param in_id_failure The ID of the call to the function `s_malloc()` that must fail after `in_count`
 * calls. If the given value is negative, then the call will never fail (unless, of course, if
 * your process really runs out of memory!).
 * @param in_count_success For the specified ID `in_id`, the number of times a call to the function
 * `s_malloc()` succeeds until it fails. If the given value is negative or zero, that the first
 * call to the function will fail (for the given ID `in_id`).
 */

void
s_malloc_init(
        const long in_id_failure,
        const long in_count_success) {
    MALLOC_ID = in_id_failure;
    MALLOC_COUNT = in_count_success;
}

/**
 * @brief The `s_malloc()` function allocates `in_size` bytes and returns a pointer to the allocated memory.
 * @param in_id Unique ID of the call to `s_malloc()`.
 * @param in_size Number of bytes to allocate.
 * @param in_initialize Flag that tells whether the allocated memory must be initialized with all zeros or not.
 * - true: the allocated memory is initialized.
 * - false: the allocated memory is not initialized.
 * @return Upon successful completion, the function returns a non-NULL value that represents the address
 * of the allocated memory. Otherwise, it returns the value NULL.
 */

void *
s_malloc(
        const long in_id,
        const size_t in_size,
        Bool in_initialize) {
    void *p;

    if ((MALLOC_ID >= 0) && (in_id >= 0) && (MALLOC_ID == in_id)) {
        if (COUNT >= MALLOC_COUNT) {
            return NULL;
        }
        COUNT += 1;
    }
    p = malloc(in_size);
    if (in_initialize) {
        memset(p, 0, in_size);
    }
    return p;
}

/**
 * @brief Free the memory located at the address
 *
 * Synopsis:
 *
 *      char *c = (char*) malloc(100);
 *      ...
 *      s_free(&c);
 *      s_free(&c); // it might seem useless or risky (it's debatable), but it does not harm
 *
 * @param in_ptr Pointer to a pointer whose value is the address of the allocated memory to free.
 */

void s_free(void **in_ptr) {
    if (NULL == *in_ptr) return;
    free(*in_ptr);
    *in_ptr = NULL;
}
