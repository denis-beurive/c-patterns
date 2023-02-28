/**
 * Use a function to allocate an array of structures. The allocation is performed by a function to
 * which we pass the address of a pointer used to store the address of the allocated memory location.
 *
 * Synopsis:
 *
 *      struct my_struct **array = NULL;
 *      malloc_array_of_struct(&array,...);
 *      if (NULL == array) { ... } // or test the returned value
 *      // ...
 *      free_array_of_struct(&array,...);
 */

#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 10
enum Status { failure = 1, success = 0 };

struct my_struct {
    int a;
    int b;
};

/**
 * @brief Free the resources allocated for a given array.
 * @note Please note that you can call this function multiple times on the same pointer.
 *       This property is very handy.
 *       - Always explicitly set the value of a freed pointer to NULL.
 *       - Always test the value of a pointer before you free it.
 * @param in_out_prt Pointer to the array.
 * @param in_capacity The array capacity.
 */

void
free_array_of_struct(
        struct my_struct ***in_out_prt,
        const size_t in_capacity) {
    // Always test for NULL.
    if (NULL == *in_out_prt) return;

    for (int i=0; i<in_capacity; i++) {
        // Be careful: `*in_out_prt[i]` is equal to `*(in_out_prt[i])`.
        if (NULL == (*in_out_prt)[i]) continue;
        free((*in_out_prt)[i]);
        (*in_out_prt)[i] = NULL;
    }

    free(*in_out_prt);
    *in_out_prt = NULL;
}

/**
 * @brief Allocate resources for an array.
 * @param in_out_ptr Address of a pointer used to store that address of the memory location allocated for the array.
 * @param in_capacity The required capacity.
 * @return On success `success`. Otherwise `failure`.
 */

enum Status
malloc_array_of_struct(
        struct my_struct ***in_out_ptr,
        const size_t in_capacity) {
    *in_out_ptr = (struct my_struct**) malloc(sizeof(struct my_struct*) * in_capacity);
    if (NULL == *in_out_ptr) return failure;

    for (int i=0; i<in_capacity; i++) {
        // Be careful: `*in_out_prt[i]` is equal to `*(in_out_prt[i])`.
        (*in_out_ptr)[i] = (struct my_struct*) malloc(sizeof(struct my_struct));
        if (NULL == (*in_out_ptr)[i]) {
            // Free all previously allocated memory.
            free_array_of_struct(in_out_ptr,
                                 i);
            return failure;
        }
    }

    return success;
}

enum Status test() {
    // Always initialize the value of a pointer to NULL.
    // Note that you can call `free_array_of_struct` on this pointer.
    struct my_struct **array_of_struct = NULL; // pointer to an array of `struct my_struct*`.

    // Just to prove the point: call `free_array_of_struct` now. There is nothing to free, but it does not hurt.
    free_array_of_struct(&array_of_struct,
                         CAPACITY);

    // Allocate the array.
    if (failure == malloc_array_of_struct(&array_of_struct,
                                          CAPACITY)) {
        return failure;
    }

    // Initialize each element.
    for (int i=0; i<CAPACITY; i++) {
        array_of_struct[i]->a = i;
        array_of_struct[i]->b = i*10;
    }

    // Print the array.
    for (int i=0; i<CAPACITY; i++) {
        printf("(%d, %d)\n", array_of_struct[i]->a, array_of_struct[i]->b);
    }

    // Free all allocated resources.
    free_array_of_struct(&array_of_struct,
                         CAPACITY);

    // Just to prove the point: call `free_array_of_struct` now. There is nothing to free, but it does not hurt.
    free_array_of_struct(&array_of_struct,
                         CAPACITY);

    return success;
}

int main() {
    enum Status status = test();
    printf("%s\n", success == status ? "success" : "failure");
    return status;
}


