#ifndef C_PATTERNS_RESOURCE_MANAGER_H
#define C_PATTERNS_RESOURCE_MANAGER_H

#include <stdarg.h>

enum RM_EnumStatus { RM_failure, RM_success };
enum RM_EnumBool { RM_true=1, RM_false=0 };
typedef enum RM_EnumStatus RM_Status;
typedef enum RM_EnumBool RM_Bool;

void
RM_init(
        long in_id_failure,
        long in_count_success,
        char *in_report_path);


struct RM_StructResourceHandler {
    /**
     * @brief Borrow a resource.
     * @param in_ptr The address of a pointer that will be assigned to the address of the borrowed resource handler.
     * @param in_uid Unique ID of the call to `borrow()`.
     * This value is used when performing unit tests. It allows to make this call fail, programmatically.
     * @param in_file Path to the file from which this function is called (typically, you set the value `__FILE__`).
     * @param in_line The line, within the file `in_file`, where this function is called (typically, you set the value
     * `__LINE__`).
     * @param in_function in_function Name of the function from which this function is called (typically, you set the
     * value `__func__`).
     * Optional: you can assign the value NULL to this parameter.
     * @param in_init Flag that tells whether the borrowed resource must be initialized or not.
     * - RM_true: the borrowed resource is initialized.
     * - RM_false: the borrowed resource is not initialized.
     * @param ... Other parameters that depend on the resource to borrow.
     * @return On success: RM_success. Otherwise: RM_failure.
     */
    RM_Status *(*borrow)(
            void **in_ptr,
            long in_uid,
            char *in_file,
            unsigned long in_line,
            char *in_function,
            RM_Bool in_init,
            ...);
    /**
     * @brief Give back a previously borrowed resource.
     * @param in_ptr The address of a pointer that is assigned to the address of the borrowed resource handler.
     * @param in_uid Unique ID of the call to `borrow()`.
     * This value is used when performing unit tests. It allows to make this call fail, programmatically.
     * @param in_file Path to the file from which this function is called (typically, you set the value `__FILE__`).
     * @param in_line The line, within the file `in_file`, where this function is called (typically, you set the value
     * `__LINE__`).
     * @param in_function in_function Name of the function from which this function is called (typically, you set the
     * value `__func__`).
     * Optional: you can assign the value NULL to this parameter.
     * @param ... Other parameters that depend on the resource to borrow.
     * @return On success: RM_success. Otherwise: RM_failure.
     */
    RM_Status *(*give_back)(
            void **in_ptr,
            long in_uid,
            char *in_file,
            unsigned long in_line,
            char *in_function,
            ...);
};

typedef struct RM_StructResourceHandler RM_ResourceHandler;




void
record_borrow(
        void **in_ptr,
        char *in_type,
        long in_id,
        const char *in_file,
        unsigned long in_line,
        const char *in_function);

void
record_give_back(
        void **in_ptr,
        char *in_type,
        long in_id,
        const char *in_file,
        unsigned long in_line,
        const char *in_function);

#endif //C_PATTERNS_RESOURCE_MANAGER_H
