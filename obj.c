/*
  This file is part of libobj (https://github.com/VasilisMylonas/libobj)

  The MIT License (MIT)

  Copyright (c) 2022 Vasilis Mylonas

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
 */

#include "obj.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void (*libobj_on_missing_method)(const obj_t* object, const char* name);

void (*__libobj_get_method(const obj_t* self, const char* name))(void)
{
    for (size_t i = 0; i < OBJ_METHODS_MAX; i++)
    {
        if ((*self)->_private.methods[i].name == NULL)
        {
            break;
        }

        if (strcmp((*self)->_private.methods[i].name, name) == 0)
        {
            return (*self)->_private.methods[i].impl;
        }
    }

    if (libobj_on_missing_method != NULL)
    {
        libobj_on_missing_method(self, name);
    }
    else
    {
        fprintf(stderr,
                "Requested method '%s::%s()' does not exist on object <%s %p>\n",
                obj_typeof(self),
                name,
                obj_typeof(self),
                self);
    }

    abort();
}

const char* obj_typeof(const obj_t* self)
{
    return (*self)->_private.name;
}

size_t obj_sizeof(const obj_t* self)
{
    return (*self)->_private.size;
}

uintptr_t obj_typeid(const obj_t* self)
{
    return (uintptr_t)*self;
}

void obj_destroy(obj_t* self)
{
    OBJ_CALL(void, obj_destroy, self);
}

char* obj_to_string(const obj_t* self)
{
    return OBJ_CALL(char*, obj_to_string, self);
}

void obj_print_vtable(const obj_t* self)
{
    fprintf(stderr, "VTable for type %s:\n", obj_typeof(self));

    for (size_t i = 0; i < OBJ_METHODS_MAX; i++)
    {
        if ((*self)->_private.methods[i].name != NULL)
        {
            fprintf(stderr,
                    "  %s::%s() - %p\n",
                    obj_typeof(self),
                    (*self)->_private.methods[i].name,
                    (*self)->_private.methods[i].impl);
        }
    }
}
