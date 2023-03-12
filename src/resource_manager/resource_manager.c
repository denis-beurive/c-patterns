#include <stddef.h>
#include <stdio.h>
#include "resource_manager.h"

static char *REPORT_FILE       = NULL;
static long CALL_FAILURE_ID    = -1;
static long CALL_COUNT_SUCCESS = 0;
static long CALL_COUNT         = 0;

/**
 * @brief Initialize the library.
 *
 * Synopsis:
 *
 *      RM_init(10, 2, "/tmp/dump-10.txt");
 *      // call which ID is 10 will fail after 2 iterations.
 *      ...
 *      RM_init(11, 3, "/tmp/dump-11.txt");
 *      // call which ID is 11 will fail after 3 iterations.
 *      ...
 *
 * @param in_id_failure The ID of the call to the function that must fail programmatically after
 * `in_count_success` calls. If the given value is negative, then the call will never fail programmatically.
 * @param in_count_success For the specified ID `in_id_failure`, the number of times a call to the function
 * succeeds until it fails programmatically. If the given value is negative or zero, then the first
 * call to the function will fail programmatically (for the given ID `in_id_failure`).
 * @param in_report_path Path to a file used to record data relative to all "borrow" / "give back".
 * If NULL, then no data is recorded.
 * @note Please note that this function may (and probably will) be called multiple times.
 */

void
RM_init(
        const long in_id_failure,
        const long in_count_success,
        char *in_report_path) {
    CALL_FAILURE_ID    = in_id_failure;
    CALL_COUNT_SUCCESS = in_count_success;
    REPORT_FILE        = in_report_path;
}

RM_Status
RM_borrow() {

    return RM_success;
}

RM_Status
RM_give_back() {

    return RM_success;
}

void
record_borrow(
        void **in_ptr,
        char *in_type,
        long in_id,
        const char *in_file,
        unsigned long in_line,
        const char *in_function) {
    FILE *fd;

    if (NULL == REPORT_FILE) return;

    fd = fopen(REPORT_FILE,
               "a");
    if (NULL == fd) {
        fprintf(stderr, "WARNING: cannot open report file \"%s\"!\n", REPORT_FILE);
        return;
    }

    if (fprintf(fd,
                "B %s %s[%s] [%s]:%lud %p %p (%ld)\n",
                in_type,
                (NULL != in_function) ? "+" : "-",
                (NULL != in_function) ? in_function : "",
                in_file,
                in_line,
                in_ptr,  // the address of the pointer used to store the address of the borrowed resource handler
                *in_ptr, // the address of the borrowed resource handler
                in_id) < 0) {
        fprintf(stderr, "WARNING: error while while writing into report file \"%s\"!\n", REPORT_FILE);
    }

    if (0 != fclose(fd)) {
        fprintf(stderr, "WARNING: error while closing report file \"%s\"!\n", REPORT_FILE);
    }
}

void
record_give_back(
        void **in_ptr,
        char *in_type,
        long in_id,
        const char *in_file,
        unsigned long in_line,
        const char *in_function) {

    FILE *fd;

    if (NULL == REPORT_FILE) return;

    fd = fopen(REPORT_FILE, "a");
    if (NULL == fd) {
        fprintf(stderr, "WARNING: cannot open report file \"%s\"!\n", REPORT_FILE);
        return;
    }
    if (fprintf(fd,
                "G %s %s[%s] [%s]:%lud %p %p (%ld)\n",
                in_type,
                (NULL != in_function) ? "+" : "-",
                (NULL != in_function) ? in_function : "",
                in_file,
                in_line,
                in_ptr,  // the address of the pointer used to store the address of the handler to the resource to give back
                *in_ptr, // the address of the handler to the resource to give back
                in_id) < 0) {
        fprintf(stderr, "WARNING: error while while writing into report file \"%s\"!\n", REPORT_FILE);
    }
    if (0 != fclose(fd)) {
        fprintf(stderr, "WARNING: error while closing report file \"%s\"!\n", REPORT_FILE);
    }
}
