/**
 * Report an error with all required data for fast identification:
 *
 * - the error is identified by an (hopefully unique) integer.
 * - the error message includes the precise location (in the source code) where the error was thrown.
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1
#define LAST_ERROR_MESSAGE_BUFFER_CAPACITY 128 // should be bigger in real life
#define LAST_ERROR_FILE_BUFFER_CAPACITY 2048

enum EnumStatus { failure, success };
typedef enum EnumStatus Status;

static int  LAST_ERROR_ID;
static char LAST_ERROR_FILE[LAST_ERROR_FILE_BUFFER_CAPACITY];
static int  LAST_ERROR_LINE;
static char LAST_ERROR_MESSAGE[LAST_ERROR_MESSAGE_BUFFER_CAPACITY];

/**
 * @brief Initialize the last error.
 */

void
last_error_init() {
    LAST_ERROR_ID = -1;
    LAST_ERROR_MESSAGE[0] = 0;
}

/**
 * @brief Return the ID of the last error.
 * @return The ID of the last error.
 */

int
last_error_get_id() {
    return LAST_ERROR_ID;
}

/**
 * @brief Return the message associated with the last error.
 * @return The message associated with the last error.
 */

char *
last_error_get_message() {
    return LAST_ERROR_MESSAGE;
}

/**
 * @brief Return the path to the file where the error occurred.
 * @return The path to the file where the error occurred.
 */

char *
last_error_file() {
    return LAST_ERROR_FILE;
}

int
last_error_line() {
    return LAST_ERROR_LINE;
}

/**
 * @brief Set the last error.
 * @param in_error_id An integer that (hopefully) uniquely identifies the error.
 * @param in_file The *ABSOLUTE* path to the (C) file that contains the code that raised the error.
 * @param in_line The number of the line, within `in_file`, where the error was raised.
 * @param in_function The name of the function that raised the error.
 * @param in_fmt The format descriptor.
 * @param ... The list of arguments.
 * @return Upon successful completion, the function returns the value `success`.
 * Otherwise, it returns the value `failure`. This means that the buffer used to store the error
 * message is not big enough. However, the content of the buffer used to store the error message is always
 * guaranteed to be zero terminated.
 * @note The return value is likely to be ignored by the calling code. Indeed, this function is intended
 * to be used to store information about an error. We assume that it will do its job just fine.
 */

Status
last_error_set(
        const int in_error_id,
        const char *in_file,
        int in_line,
        const char *in_function,
        const char *in_fmt,
        ...) {
    int    bytes_written      = 0;
    size_t remaining_capacity = LAST_ERROR_MESSAGE_BUFFER_CAPACITY;

    // Copy the error ID, the line number and the name of the source file.
    LAST_ERROR_ID = in_error_id;
    LAST_ERROR_LINE = in_line;
    strncpy(LAST_ERROR_FILE, in_file, LAST_ERROR_FILE_BUFFER_CAPACITY);
    LAST_ERROR_FILE[LAST_ERROR_FILE_BUFFER_CAPACITY-1] = 0;

    // See the comment for the function `vsnprintf()`. `snprintf` behaves the same way.
    // If the buffer is big enough, then `bytes_written` is the number of bytes actually written,
    // **excluding the final zero character**.
    bytes_written = snprintf(LAST_ERROR_MESSAGE,
                             remaining_capacity,
                             "#%010d [%s:%d %s()] ", in_error_id, in_file, in_line, in_function);
    if ((bytes_written < 0) || (bytes_written >= remaining_capacity)) {
        LAST_ERROR_MESSAGE[0] = 0;
        return failure;
    }
    remaining_capacity -= (size_t)bytes_written;

    // NOTE:
    //
    // The function `vsnprintf(char *str, size_t size, const char *format, va_list ap)`
    // writes at most `size` bytes (including the terminating null byte.
    //
    // The function `vsnprintf()` does not write more than `size` bytes
    // (including the terminating null byte ('\0')). If the output was truncated
    // due to this limit then the return value is the number of characters (excluding
    // the terminating null byte) which would have been written to the final string if
    // enough space had been available. Thus, a return value of `size` or more means that
    // the output was truncated.

    va_list arg_ptr;
    va_start(arg_ptr, in_fmt);
    bytes_written = vsnprintf(LAST_ERROR_MESSAGE + bytes_written, // start writing at the end of the string
                              remaining_capacity,
                              in_fmt,
                              arg_ptr);
    if ((bytes_written < 0) || (bytes_written >= remaining_capacity)) {
        LAST_ERROR_MESSAGE[0] = 0;
        return failure;
    }
    return success;
}

Status
function1() {
    return last_error_set(1,
                          __FILE__,
                          __LINE__,
                          __func__,
                          "This is the error #%d in %s",
                          1,
                          __func__);
}

Status
function2() {
    return last_error_set(2,
                          __FILE__,
                          __LINE__,
                          __func__,
                          "This is the error #%d in %s",
                          2,
                          __func__);
}

Status
function_fail() {
    // Create a string that will be too big...
    char data[LAST_ERROR_MESSAGE_BUFFER_CAPACITY];
    memset((void *) data,
           '.',
           LAST_ERROR_MESSAGE_BUFFER_CAPACITY);
    data[LAST_ERROR_MESSAGE_BUFFER_CAPACITY - 1] = 0;

    return last_error_set(3,
                          __FILE__,
                          __LINE__,
                          __func__,
                          "This is too long #%d in %s (data: %s)",
                          3,
                          __func__,
                          data);
}

int
main() {
    last_error_init();

    if (function1() == failure) { return EXIT_ERROR; };
    printf("message: [%s]\n",
           last_error_get_message());
    printf("id:      [%d]\n",
           last_error_get_id());
    printf("file:    [%s]\n",
           last_error_file());
    printf("line:    [%d]\n",
           last_error_line());


    if (function2() == failure) { return EXIT_ERROR; };
    printf("message: [%s]\n",
           last_error_get_message());
    printf("id:      [%d]\n",
           last_error_get_id());
    printf("file:    [%s]\n",
           last_error_file());
    printf("line:    [%d]\n",
           last_error_line());


    if (function_fail() == success) { return EXIT_ERROR; }
    printf("message: [%s]\n",
           last_error_get_message());
    printf("id:      [%d]\n",
           last_error_get_id());
    printf("file:    [%s]\n",
           last_error_file());
    printf("line:    [%d]\n",
           last_error_line());

    return EXIT_SUCCESS;
}
