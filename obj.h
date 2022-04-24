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

#ifndef OBJ_H
#define OBJ_H

/**
 * @file obj.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief OOP support library for C.
 * @version 0.1
 * @date 2022-04-24
 * @copyright Copyright (c) 2022 Vasilis Mylonas
 */

#include <stddef.h>
#include <stdint.h>

#define OBJ_METHODS_MAX 62
#define OBJ_HEADER      const struct __obj_vtable* __vptr;

#define OBJ_VTABLE_INIT(T, ...)                                                                    \
    static const struct __obj_vtable T##_vtable = {                                                \
        .size = sizeof(T),                                                                         \
        .name = #T,                                                                                \
        .methods = {__VA_ARGS__},                                                                  \
    };                                                                                             \
    self->__vptr = &T##_vtable;

#define OBJ_METHOD(function)                                                                       \
    {                                                                                              \
        .name = #function, .impl = (void (*)(void))function##_impl,                                \
    }

#define OBJ_CALL(ret_type, name, ...)                                                              \
    ((ret_type(*)())obj_get_method(__OBJ_FIRST(__VA_ARGS__), name))(__VA_ARGS__, name)

typedef void obj_t;

/**
 * Retrieves the specified method from an object's vtable.
 *
 * If no such method exists, abort() is called.
 *
 * @param self The object.
 * @param name The method name.
 * @return The requested method.
 */
void (*obj_get_method(const obj_t* self, const char* name))(void);

/**
 * Returns a string describing an object's type.
 *
 * @param self The object.
 * @return The type name.
 */
const char* obj_typeof(const obj_t* self);

/**
 * Returns the size of an object.
 *
 * @param self The object.
 * @return The object's size.
 */
size_t obj_sizeof(const obj_t* self);

/**
 * Returns an identifier representing an object's type.
 *
 * @param self The object.
 * @return The type id.
 */
uintptr_t obj_typeid(const obj_t* self);

/**
 * Calls the destructor for an object.
 *
 * @param object The object.
 */
void obj_destroy(obj_t* object);

/**
 * Returns a string representation of an object.
 *
 * @param self The object.
 * @return The object's string representation.
 */
char* obj_to_string(const obj_t* self);

/**
 * Prints information about an object's vtable to stderr.
 *
 * @param self The object.
 */
void obj_print_vtable(const obj_t* self);

#define __OBJ_FIRST(a, ...) a

struct __obj_method
{
    const char* name;
    void (*impl)(void);
};

struct __obj_vtable
{
    size_t size;
    const char* name;
    struct __obj_method methods[OBJ_METHODS_MAX];
};

#endif // OBJ_H
