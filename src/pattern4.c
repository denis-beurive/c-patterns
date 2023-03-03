#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_CAPACITY 128

void safe_free(void **in_prt) {
    free(*in_prt);
    *in_prt = NULL;
}

enum EnumStatus { failure = 1, success = 0 };

typedef enum EnumStatus Status;

// =====================================================================
// Implement a list
// =====================================================================

struct StructList {
    void **elements;
    unsigned int capacity;
    unsigned int size;
    void (*element_dispose)(void **);
};

typedef struct StructList* List;

void list_init(List *in_list) {
    *in_list = NULL;
}

Status list_create(List *in_list,
                   unsigned int in_capacity,
                   void (*in_element_dispose)(void **)) {
    *in_list = (List) malloc(sizeof(struct StructList));
    if (NULL == *in_list) {
        return failure;
    }
    (*in_list)->elements = (void **) malloc(sizeof(void *) * in_capacity);
    if (NULL == (*in_list)->elements) {
        safe_free((void**)in_list);
        return failure;
    }
    (*in_list)->capacity = in_capacity;
    (*in_list)->size = 0;
    (*in_list)->element_dispose = in_element_dispose;
    return success;
}

void list_dispose(List *in_list) {
    if (NULL == *in_list) {
        return;
    }
    for (unsigned int i=0; i<(*in_list)->size; i++) {
        void *element = (*in_list)->elements[i];
        (*in_list)->element_dispose(&element);
    }
    safe_free((void **) &(*in_list)->elements);
    safe_free((void **) in_list);
}

Status list_push(List *in_list, void *in_element) {
    if ((*in_list)->size + 1 >= (*in_list)->capacity) {
        unsigned int new_capacity = 2 * (*in_list)->capacity;
        (*in_list)->elements = (void **) realloc((void *) ((*in_list)->elements),
                                                 sizeof(void *) * new_capacity);
        if (NULL == (*in_list)->elements) {
            list_dispose(in_list);
            return failure;
        }
        (*in_list)->capacity = new_capacity;
    }
    ((*in_list)->elements)[(*in_list)->size] = in_element;
    (*in_list)->size += 1;
    return success;
}

// =====================================================================
// Implement an element for the list
// =====================================================================

typedef char* Element;

void element_init(Element *in_element) {
    *in_element = NULL;
}

Status element_create(Element *out_element, const char *in_src) {
    size_t src_len = strlen(in_src);
    *out_element = (Element) malloc(sizeof(char) * (src_len + 1));
    if (NULL == *out_element) {
        return failure;
    }
    strcpy(*out_element, in_src);
    return success;
}

void element_dispose(Element **in_element) {
    if (NULL == *in_element) return;
    safe_free((void**)in_element);
}



int main() {

    List list;
    char buffer[BUFFER_CAPACITY];

    for (unsigned int count=0; count<100; count++) {

        list_init(&list);
        list_create(&list,
                    10,
                    (void (*)(void **)) element_dispose);
        for (unsigned int i = 0; i < 40; i++) {
            Element element;

            element_init(&element);
            snprintf(buffer,
                     BUFFER_CAPACITY,
                     "element-%d",
                     i);
            element_create(&element,
                           buffer);
            list_push(&list,
                      (void *) element);
        }
        printf("size    : %u\n",
               list->size);
        printf("capacity: %u\n",
               list->capacity);
        for (unsigned int i = 0; i < list->size; i++) {
            printf("=> [%s]\n",
                   (char *) list->elements[i]);
        }
        list_dispose(&list);
    }

    return 0;
}

