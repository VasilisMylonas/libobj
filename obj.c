#include "obj.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

static noreturn void obj_trap_method(const obj_t* self, const char* name)
{
    fprintf(stderr,
            "Requested method '%s::%s()' does not exist on object <%s %p>\n",
            obj_typeof(self),
            name,
            obj_typeof(self),
            self);

    abort();
}

void (*obj_get_method(const obj_t* self, const char* name))(void)
{
    const struct __obj_vtable* vtable = *(const struct __obj_vtable**)self;

    for (size_t i = 0; i < OBJ_METHODS_MAX; i++)
    {
        if (vtable->methods[i].name == NULL)
        {
            obj_trap_method(self, name);
        }

        if (strcmp(vtable->methods[i].name, name) == 0)
        {
            return vtable->methods[i].impl;
        }
    }

    obj_trap_method(self, name);
}

const char* obj_typeof(const obj_t* self)
{
    const struct __obj_vtable* vtable = *(const struct __obj_vtable**)self;
    return vtable->name;
}

size_t obj_sizeof(const obj_t* self)
{
    const struct __obj_vtable* vtable = *(const struct __obj_vtable**)self;
    return vtable->size;
}

uintptr_t obj_typeid(const void* self)
{
    const struct __obj_vtable* vtable = *(const struct __obj_vtable**)self;
    return (uintptr_t)vtable;
}

void obj_destroy(obj_t* self)
{
    OBJ_CALL(void, "obj_destroy", self);
}

char* obj_to_string(const obj_t* self)
{
    return OBJ_CALL(char*, "obj_to_string", self);
}

void obj_print_vtable(const obj_t* self)
{
    const struct __obj_vtable* vtable = *(const struct __obj_vtable**)self;
    fprintf(stderr, "VTable for type %s:\n", vtable->name);

    for (size_t i = 0; i < OBJ_METHODS_MAX; i++)
    {
        if (vtable->methods[i].name != NULL)
        {
            fprintf(stderr,
                    "  %s::%s() - %p\n",
                    vtable->name,
                    vtable->methods[i].name,
                    vtable->methods[i].impl);
        }
    }
}
