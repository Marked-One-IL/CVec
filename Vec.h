#ifndef CVEC_VEC_H
#define CVEC_VEC_H
// https://github.com/Marked-One-IL/CVec.git
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdbool.h>

#define DEFINE_VEC_HEADER(type, name) \
struct Vec_##name \
{ \
    type *buffer; \
    size_t length; \
    size_t capacity; \
}; \
typedef struct Vec_##name Vec_##name; \
\
Vec_##name  Vec_##name##_init      (void); \
Vec_##name  Vec_##name##_initArr   (const type *arr, size_t length); \
Vec_##name  Vec_##name##_initVec   (const Vec_##name *other); \
Vec_##name  Vec_##name##_subVec    (const Vec_##name *other, size_t index, size_t n); \
void        Vec_##name##_free      (Vec_##name *self); \
void        Vec_##name##_clear     (Vec_##name *self); \
void        Vec_##name##_reserve   (Vec_##name *self, size_t capacity); \
void        Vec_##name##_resize    (Vec_##name *self, size_t length); \
void        Vec_##name##_resizeV   (Vec_##name *self, size_t length, type v); \
void        Vec_##name##_push      (Vec_##name *self, type v); \
void        Vec_##name##_pushArr   (Vec_##name *self, const type *arr, size_t length); \
void        Vec_##name##_pushVec   (Vec_##name *self, const Vec_##name *other); \
void        Vec_##name##_pop       (Vec_##name *self); \
void        Vec_##name##_popN      (Vec_##name *self, size_t n); \
void        Vec_##name##_insert    (Vec_##name *self, size_t index, type v); \
void        Vec_##name##_insertArr (Vec_##name *self, size_t index, const type *arr, size_t length); \
void        Vec_##name##_insertVec (Vec_##name *self, size_t index, const Vec_##name *other); \
void        Vec_##name##_erase     (Vec_##name *self, size_t index); \
void        Vec_##name##_eraseN    (Vec_##name *self, size_t index, size_t n);

#define DEFINE_VEC_SOURCE(type, name) \
static bool Vec_##name##_Internal_overlapPtr(const Vec_##name *self, const type *p) \
{ assert(NULL != self); assert(NULL != p); \
\
    return (p >= self->buffer) && (p < (self->buffer + self->length)); \
} \
static type *Vec_##name##_Internal_duplicateArr(const type *arr, size_t length) \
{ assert(NULL != arr); \
\
    type *buffer = (type*)malloc(length * sizeof(type)); assert(buffer); \
    memcpy(buffer, arr, length * sizeof(type)); \
    return buffer; \
} \
Vec_##name Vec_##name##_init(void) \
{ \
    Vec_##name obj = { NULL, 0, 0 }; \
    return obj; \
} \
Vec_##name Vec_##name##_initArr(const type *arr, size_t length) \
{ assert(NULL != arr); \
\
    Vec_##name obj = Vec_##name##_init(); \
    if (length) { \
        obj.buffer = Vec_##name##_Internal_duplicateArr(arr, length); \
    } else { \
        obj.buffer = NULL; \
    } \
    obj.length = length; \
    obj.capacity = length; \
    return obj; \
} \
Vec_##name Vec_##name##_initVec(const Vec_##name *other) \
{ assert(NULL != other); \
\
    if (!other->buffer || !other->length) { \
        return Vec_##name##_init(); \
    } \
    return Vec_##name##_initArr(other->buffer, other->length); \
} \
Vec_##name Vec_##name##_subVec(const Vec_##name *other, size_t index, size_t n) \
{ assert(NULL != other); assert(index <= other->length); assert(n <= (other->length - index));\
    return Vec_##name##_initArr(other->buffer + index, n); \
} \
void Vec_##name##_free(Vec_##name *self) \
{ assert(NULL != self); \
\
    free(self->buffer); \
    self->buffer = NULL; \
    self->length = 0; \
    self->capacity = 0; \
} \
void Vec_##name##_clear(Vec_##name *self) \
{ assert(NULL != self); \
    self->length = 0; \
} \
void Vec_##name##_reserve(Vec_##name *self, size_t capacity) \
{ assert(NULL != self); \
\
    if (capacity > self->capacity) \
    { \
        size_t doubleCapacity = self->capacity * 2; \
        size_t newCapacity = (doubleCapacity > capacity) ? doubleCapacity : capacity; \
        type *buffer = (type*)realloc(self->buffer, newCapacity * sizeof(type)); assert(buffer); \
        self->buffer = buffer; \
        self->capacity = newCapacity; \
    } \
} \
void Vec_##name##_resize(Vec_##name *self, size_t length) \
{ assert(NULL != self); \
    if (length > self->length) { \
        Vec_##name##_reserve(self, length); \
        memset(self->buffer + self->length, 0, (length - self->length) * sizeof(type)); \
    } \
    self->length = length; \
} \
void Vec_##name##_resizeV(Vec_##name *self, size_t length, type v) \
{ assert(NULL != self); \
    if (length > self->length) { \
        Vec_##name##_reserve(self, length); \
        size_t i = self->length; \
        for (; i < length; i++) { \
            self->buffer[i] = v; \
        } \
    } \
    self->length = length; \
} \
void Vec_##name##_push(Vec_##name *self, type v) \
{ assert(NULL != self); \
\
    Vec_##name##_reserve(self, self->length + 1); \
    self->buffer[self->length++] = v; \
} \
void Vec_##name##_pushArr(Vec_##name *self, const type *arr, size_t length) \
{ assert(NULL != self); assert(NULL != arr); \
\
    if (Vec_##name##_Internal_overlapPtr(self, arr)) \
    { \
        type *arrayClone = Vec_##name##_Internal_duplicateArr(arr, length); \
        Vec_##name##_reserve(self, self->length + length); \
        memcpy(self->buffer + self->length, arrayClone, length * sizeof(type)); \
        free(arrayClone); \
    } \
    else \
    { \
        Vec_##name##_reserve(self, self->length + length); \
        memcpy(self->buffer + self->length, arr, length * sizeof(type)); \
    } \
\
    self->length += length; \
} \
void Vec_##name##_pushVec(Vec_##name *self, const Vec_##name *other) \
{ assert(NULL != self); assert(NULL != other); \
\
    if (!other->buffer || !other->length) { \
        return; \
    } \
    Vec_##name##_pushArr(self, other->buffer, other->length); \
} \
void Vec_##name##_pop(Vec_##name *self) \
{ assert(NULL != self); assert(0 != self->length); \
\
    self->length--; \
} \
void Vec_##name##_popN(Vec_##name *self, size_t n) \
{ assert(NULL != self); assert(self->length >= n); \
\
    self->length -= n; \
} \
void Vec_##name##_insert(Vec_##name *self, size_t index, type v) \
{ assert(NULL != self); assert(index <= self->length); \
\
    Vec_##name##_reserve(self, self->length + 1); \
    type *dst = self->buffer + index + 1; \
    type *src = self->buffer + index; \
    size_t size = self->length - index; \
    memmove(dst, src, size * sizeof(type)); \
    self->buffer[index] = v; \
    self->length++; \
} \
void Vec_##name##_insertArr(Vec_##name *self, size_t index, const type *arr, size_t length) \
{ assert(NULL != self); assert(NULL != arr); assert(index <= self->length); \
\
    if (Vec_##name##_Internal_overlapPtr(self, arr)) \
    { \
        type *arrayClone = Vec_##name##_Internal_duplicateArr(arr, length); \
        Vec_##name##_reserve(self, self->length + length); \
        type *dst = self->buffer + index + length; \
        type *src = self->buffer + index; \
        size_t size = self->length - index; \
        memmove(dst, src, size * sizeof(type)); \
        memcpy(self->buffer + index, arrayClone, length * sizeof(type)); \
        free(arrayClone); \
    } \
    else \
    { \
        Vec_##name##_reserve(self, self->length + length); \
        type *dst = self->buffer + index + length; \
        type *src = self->buffer + index; \
        size_t size = self->length - index; \
        memmove(dst, src, size * sizeof(type)); \
        memcpy(self->buffer + index, arr, length * sizeof(type)); \
    } \
    self->length += length; \
} \
void Vec_##name##_insertVec(Vec_##name *self, size_t index, const Vec_##name *other) \
{ assert(NULL != self); assert(NULL != other); assert(index <= self->length); \
\
    if (!other->buffer || !other->length) { \
        return; \
    } \
    Vec_##name##_insertArr(self, index, other->buffer, other->length); \
} \
void Vec_##name##_erase(Vec_##name *self, size_t index) \
{ assert(NULL != self); assert(index < self->length); \
\
    type *dst = self->buffer + index; \
    type *src = self->buffer + index + 1; \
    size_t size = self->length - index - 1; \
    memmove(dst, src, size * sizeof(type)); \
    self->length--; \
} \
void Vec_##name##_eraseN(Vec_##name *self, size_t index, size_t n) \
{ assert(NULL != self); assert(index <= self->length); assert(n <= self->length); assert(n <= (self->length - index)); \
\
    type *dst = self->buffer + index; \
    type *src = self->buffer + index + n; \
    size_t size = self->length - index - n; \
    memmove(dst, src, size * sizeof(type)); \
    self->length -= n; \
}

#endif // CVEC_VEC_H