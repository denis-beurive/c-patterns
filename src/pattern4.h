#ifndef C_PATTERNS_PATTERN4_H
#define C_PATTERNS_PATTERN4_H

#ifndef RESOURCES_TYPES
#error "You must declare a set of ressources by defined the macro RESOURCES_TYPES."
#endif

#define BASE_RESOURCES_TYPES \
DECLARE(DMem, "mem")  \
DECLARE(DFile, "file")

// Please note:
// The line `#define DECLARE(a, b) a` defines a simple macro (called "DECLARE") that replaces "(V1, V2)" into "V1,".
#define DECLARE(a, b) a,
enum Resource {
    // The macro "BASE_RESOURCES_TYPES" uses the macro "DECLARE". Thus, the macro "DECLARE" is evaluated:
    //    `DECLARE(DMem,   "mem")`   => DMem,
    //    `DECLARE(DFile,  "file")`  => DFile,
    // The same goes for the macro "RESOURCES_TYPES".
    // And thus: `enum Resource { BASE_RESOURCES_TYPES RESOURCES_TYPES }` => "enum Resource { DMem, DFile, DMysql, EOR }"
    BASE_RESOURCES_TYPES
    RESOURCES_TYPES
    EOR /* End Of Resources */
};
enum Resource resources[] = {
        BASE_RESOURCES_TYPES
        RESOURCES_TYPES
        EOR
};
#undef DECLARE

// Please note:
// The line `#define DECLARE(a, b) b` defines a simple macro (called "DECLARE") that replaces "(V1, V2)" into "V2,".
#define DECLARE(a, b) b,
char *resource_name[] = {
        // The macro "BASE_RESOURCES_TYPES" uses the macro "DECLARE". Thus, the macro "DECLARE" is evaluated:
        //    `DECLARE(DMem,   "mem")`   => "mem",
        //    `DECLARE(DFile,  "file")`  => "file",
        // The same goes for the macro "RESOURCES_TYPES".
        // And thus: `char *resource_name[] = { BASE_RESOURCES_TYPES RESOURCES_TYPES }` => "char *resource_name[] = { "mem", "file", DMysql", }"
        BASE_RESOURCES_TYPES
        RESOURCES_TYPES
        NULL
};
#undef DECLARE



// =>
// enum Resource { DMem, DFile, DMysql, EOR };
// char *resource_name[] = { "mem", "file", DMysql", NULL }

#endif //C_PATTERNS_PATTERN4_H
