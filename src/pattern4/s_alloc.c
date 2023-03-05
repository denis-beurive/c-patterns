#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "s_alloc.h"

static long       MALLOC_ID    = -1;
static long       MALLOC_COUNT = 0;
static long       COUNT        = 0;
static const char *DUMP_PATH;
static Bool       EXIT_ON_DATA_RECORDING_ERROR;

static void
record_malloc(
        void **in_ptr,
        long in_id,
        size_t in_size,
        const char *in_file,
        unsigned long in_line,
        const char *in_function);

static void
record_realloc(
        void **in_ptr,
        void *in_old_address,
        long in_id,
        size_t in_size,
        const char *in_file,
        unsigned long in_line,
        const char *in_function);

static void
record_free(
        void **in_ptr,
        const char *in_file,
        unsigned long in_line,
        const char *in_function);

// -------------------------------------------------------------------------------------
// Public API
// -------------------------------------------------------------------------------------

/**
 * @brief Initialize the "s_alloc" library.
 *
 * Synopsis:
 *
 *      s_alloc_init(10, 2, "/tmp/dump-10.txt");
 *      // call (to `s_malloc` or `s_realloc`) which ID is 10 will fail after 2 iterations.
 *      ...
 *      s_alloc_init(11, 3, "/tmp/dump-11.txt");
 *      // call (to `s_malloc` or `s_realloc`) which ID is 11 will fail after 3 iterations.
 *      ...
 *
 * @param in_id_failure The ID of the call to the function `s_malloc()` that must fail after `in_count`
 * calls. If the given value is negative, then the call will never fail (unless, of course, if
 * your process really runs out of memory!).
 * @param in_count_success For the specified ID `in_id`, the number of times a call to the function
 * `s_malloc()` succeeds until it fails. If the given value is negative or zero, that the first
 * call to the function will fail (for the given ID `in_id`).
 * @param in_dump_path Path to a file used to record data relative to all allocations / deallocations.
 * If NULL, then no data is recorded.
 * @param in_exit_on_data_recording_error Flag that tells whether the library should terminate the process
 * execution if it is impossible to record allocation data.
 * - true: the library terminates the process execution.
 * - false: the library does not terminate the process execution (why would you do that?)
 * @note Please note that this function may (and probably will) be called multiple times.
 */

void
s_alloc_init(
        const long in_id_failure,
        const long in_count_success,
        const char *in_dump_path,
        const Bool in_exit_on_data_recording_error) {
    MALLOC_ID = in_id_failure;
    MALLOC_COUNT = in_count_success;
    DUMP_PATH                    = in_dump_path;
    EXIT_ON_DATA_RECORDING_ERROR = in_exit_on_data_recording_error;
}

/**
 * @brief The `s_malloc()` function allocates `in_size` bytes.
 *
 * Synopsis:
 *
 *      #define ID_FAIL 10
 *
 *      char *c;
 *
 *      s_alloc_init(ID_FAIL, 2, "/tmp/dump-10.txt");
 *      ...
 *      for (int i=0; i<3; i++) {
 *         if (failure == s_malloc(&c, ID_FAIL, 100, true, __FILE__, __LINE__, __func__)) {
 *              // i=2 => this bloc is executed
 *              ...
 *         }
 *         // i=0 => OK.
 *         // i=1 => OK.
 *         ...
 *         s_free(&c, __FILE__, __LINE__, __func__);
 *      }
 *
 * @param in_ptr The address of a pointer that will be assigned to the address of the allocated memory.
 * @param in_id Unique ID of the call to `s_malloc()`.
 * @param in_size Number of bytes to allocate.
 * @param in_initialize Flag that tells whether the allocated memory must be initialized with all zeros or not.
 * - true: the allocated memory is initialized with all zeros.
 * - false: the allocated memory is not initialized with all zeros.
 * @param in_file Path to the file from which this function is called (typically, you set the value `__FILE__`).
 * Optional: you can assign the value NULL to this parameter.
 * @param in_line The line, within the file `in_file`, where this function is called (typically, you set the value
 * `__LINE__`).
 * Optional: you can assign the value 0 to this parameter.
 * @param in_function Name of the function from which this function is called (typically, you set the value `__func__`).
 * Optional: you can assign the value NULL to this parameter.
 * @return Upon successful completion: `success`. Otherwise: `failure`.
 */

Status
s_malloc(
        void **in_ptr,
        const long in_id,
        const size_t in_size,
        Bool in_initialize,
        const char *in_file,
        unsigned long in_line,
        const char *in_function) {
    // Shall we simulate a shortage of resources?
    if ((MALLOC_ID >= 0) && (in_id >= 0) && (MALLOC_ID == in_id)) {
        if (COUNT >= MALLOC_COUNT) {
            return failure;
        }
        COUNT += 1;
    }
    *in_ptr = (void*)malloc(in_size);
    if (in_initialize) memset(*in_ptr, 0, in_size);
    // Dump data into the dump file.
    record_malloc(in_ptr, in_id, in_size, in_file, in_line, in_function);
    return success;
}

/**
 * @brief Reallocate memory.
 *
 *      #define ID_OK 9
 *      #define ID_FAIL 10
 *
 *      char *c;
 *
 *      s_alloc_init(ID_FAIL, 2, "/tmp/dump-10.txt");
 *      ...
 *      if (failure == s_malloc(&c, ID_OK, 100, true, __FILE__, __LINE__, __func__)) {
 *              // treat the error
 *      }
 *
 *      for (int i=0; i<3; i++) {
 *          it (failure == s_realloc(&c, ID_FAIL, 1000, __FILE__, __LINE__, __func__)) {
 *              // i=2 => this bloc is executed
 *              ...
 *          }
 *         // i=0 => OK.
 *         // i=1 => OK.
 *         ...
 *      }
 *
 *      s_free(&c, __FILE__, __LINE__, __func__);
 *
 * @param in_ptr The address of a pointer that:
 * (1) is assigned to the address of the memory to reallocate
 * and
 * (2) will be assigned to the address of the reallocated memory.
 * @param in_id Unique ID of the call to `s_realloc()`.
 * @param in_new_size The new required size of the memory.
 * @param in_file Path to the file from which this function is called (typically, you set the value `__FILE__`).
 * Optional: you can assign the value NULL to this parameter.
 * @param in_line The line, within the file `in_file`, where this function is called (typically, you set the value
 * `__LINE__`).
 * Optional: you can assign the value 0 to this parameter.
 * @param in_function Name of the function from which this function is called (typically, you set the value `__func__`).
 * Optional: you can assign the value NULL to this parameter.
 * @return Upon successful completion: `success`. Otherwise: `failure`.
 */

Status
s_realloc(
        void **in_ptr,
        const long in_id,
        size_t in_new_size,
        const char *in_file,
        unsigned long in_line,
        const char *in_function) {
    void *p;
    void *old_ptr = *in_ptr;

    // Shall we simulate a shortage of resources?
    if ((MALLOC_ID >= 0) && (in_id >= 0) && (MALLOC_ID == in_id)) {
        if (COUNT >= MALLOC_COUNT) return failure;
        COUNT += 1;
    }
    p = realloc(*in_ptr, in_new_size);
    if (NULL == p) return failure;
    *in_ptr = p;
    // Dump data into the dump file.
    record_realloc(in_ptr, old_ptr, in_id, in_new_size, in_file, in_line, in_function);
    return success;
}


/**
 * @brief Free the memory located at the address
 *
 * Synopsis:
 *
 *      #define ID_FAIL 10
 *
 *      char *c;
 *
 *      s_alloc_init(ID_FAIL, 2, "/tmp/dump-10.txt");
 *
 *      if (failure == s_malloc(&c, ID_FAIL, 100, true, __FILE__, __LINE__, __func__)) {
 *          // Treat the error
 *      }
 *      ...
 *      s_free(&c, __FILE__, __LINE__, __func__);
 *      s_free(&c, __FILE__, __LINE__, __func__); // it might seem useless or risky (it's debatable), but it does not harm
 *
 * @param in_ptr The address of a pointer that is assigned to the address of the memory to free.
 * @param in_file Path to the file from which this function is called (typically, you set the value `__FILE__`).
 * Optional: you can assign the value NULL to this parameter.
 * @param in_line The line, within the file `in_file`, where this function is called (typically, you set the value
 * `__LINE__`).
 * Optional: you can assign the value 0 to this parameter.
 * @param in_function Name of the function from which this function is called (typically, you set the value `__func__`).
 * Optional: you can assign the value NULL to this parameter.
 */

void
s_free(void **in_ptr,
       const char *in_file,
       unsigned long in_line,
       const char *in_function) {
    // Dump data into the dump file, if required.
    record_free(in_ptr, in_file, in_line, in_function);
    if (NULL == *in_ptr) return;
    free(*in_ptr);
    *in_ptr = NULL;
}

// -------------------------------------------------------------------------------------
// Private API
// -------------------------------------------------------------------------------------

static void
record_malloc(
        void **in_ptr,
        const long in_id,
        const size_t in_size,
        const char *in_file,
        unsigned long in_line,
        const char *in_function) {
    FILE *fd;

    if (NULL == DUMP_PATH) return;

    fd = fopen(DUMP_PATH, "a");
    if (NULL == fd) {
        fprintf(stderr,
                "WARNING: cannot open dump file \"%s\"!\n",
                DUMP_PATH);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_ERROR);
    }
    if (fprintf(fd,
                "A %s[%s] %s[%s]:%lud %p %p %lud (%ld)\n",
                (NULL != in_function) ? "+" : "-",
                (NULL != in_function) ? in_function : "",
                (NULL != in_file) ? "+" : "-",
                (NULL != in_file) ? in_file : "",
                in_line,
                in_ptr,  // the address of the pointer used to store the address of the allocated memory
                *in_ptr, // the address of the allocated memory
                in_size, // the size of the allocated memory
                in_id) < 0) {
        fprintf(stderr,
                "WARNING: error while while writing into file \"%s\"!\n",
                DUMP_PATH);
        fclose(fd);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_ERROR);
    }
    if (0 != fclose(fd)) {
        fprintf(stderr,
                "WARNING: error while closing dump file \"%s\"!\n",
                DUMP_PATH);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_ERROR);
    }
}

static void
record_realloc(
        void **in_ptr,
        void *in_old_address,
        const long in_id,
        const size_t in_size,
        const char *in_file,
        unsigned long in_line,
        const char *in_function) {
    FILE *fd;

    if (NULL == DUMP_PATH) return;

    fd = fopen(DUMP_PATH,
               "a");
    if (NULL == fd) {
        fprintf(stderr,
                "WARNING: cannot open dump file \"%s\"!\n",
                DUMP_PATH);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_ERROR);
    }
    if (fprintf(fd,
                "R %s[%s] %s[%s]:%lud %p %p %p %lud (%ld)\n",
                (NULL != in_function) ? "+" : "-",
                (NULL != in_function) ? in_function : "",
                (NULL != in_file) ? "+" : "-",
                (NULL != in_file) ? in_file : "",
                in_line,
                in_ptr,     // the address of the pointer used to store the address of the allocated memory
                in_old_address, // the previous address of the allocated memory
                *in_ptr,    // the new address of the allocated memory
                in_size,    // the new size of the allocated memory
                in_id) < 0) {
        fprintf(stderr,
                "WARNING: error while while writing into file \"%s\"!\n",
                DUMP_PATH);
        fclose(fd);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_ERROR);
    }
    if (0 != fclose(fd)) {
        fprintf(stderr,
                "WARNING: error while closing dump file \"%s\"!\n",
                DUMP_PATH);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_ERROR);
    }
}

static void
record_free(
        void **in_ptr,
        const char *in_file,
        unsigned long in_line,
        const char *in_function) {

    FILE *fd;

    if (NULL == DUMP_PATH) return;

    fd = fopen(DUMP_PATH, "a");
    if (NULL == fd) {
        fprintf(stderr,
                "WARNING: cannot open dump file \"%s\"!\n",
                DUMP_PATH);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_FAILURE);
    }
    if (fprintf(fd,
                "F %s[%s] %s[%s]:%lud %p %p\n",
                (NULL != in_function) ? "+" : "-",
                (NULL != in_function) ? in_function : "",
                (NULL != in_file) ? "+" : "-",
                (NULL != in_file) ? in_file : "",
                in_line,
                in_ptr,         // the address of the pointer used to store the address of the memory to free
                *in_ptr) < 0) { // the address of the memory that to free
        fprintf(stderr,
                "WARNING: error while while writing into file \"%s\"!\n",
                DUMP_PATH);
        fclose(fd);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_FAILURE);
    }
    if (0 != fclose(fd)) {
        fprintf(stderr,
                "WARNING: error while closing dump file \"%s\"!\n", DUMP_PATH);
        if (EXIT_ON_DATA_RECORDING_ERROR) exit(EXIT_FAILURE);
    }
}

