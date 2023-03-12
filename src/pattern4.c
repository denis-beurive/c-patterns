/**
 * This file shows how to "extends a list of enums".
 *
 * The file "pattern4.h" defines a base list of enums: enum Resource { DMem, DFile, };
 *
 * We want the user of this header file (which may be part of a library) to be able
 * to extend this list of enums (without modifying the header file, of course).
 *
 * To see the source after the macro expansions: gcc -E pattern4.c
 */
#include <stdio.h>

// Please note:
// The macro "RESOURCES_TYPES" used the macro "DECLARE" that is not declared yet.
// The macro "DECLARE" is declared in the file "pattern4.h".
#define RESOURCES_TYPES  \
DECLARE(DMysql, "mysql")

// This header file "pattern4.h" must be included *AFTER* the definition of the macro "RESOURCES_TYPES".
#include "pattern4.h"

// At this point, the header file "pattern4.h" defines the following entities:
// enum Resource { DMem, DFile, DMysql, EOR }; // EOR: End Of Resources (so you can iterate on the enums)
// char *resource_name[] = { "mem", "file", DMysql", NULL }

int main() {
    int e = 0;
    while (e != EOR) {
        printf("%s\n", resource_name[e]);
        e += 1;
    }
    return 0;
}



