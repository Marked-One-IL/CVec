#ifndef VEC_H
#define VEC_H
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdbool.h>

#define DEFINE_VEC(type, name) \
struct Vec_##name \
{ \
	type* buffer; \
	size_t length; \
	size_t capacity; \
}; \
typedef struct Vec_##name Vec_##name; \
\
static Vec_##name  Vec_##name##_init      (void); \
static Vec_##name  Vec_##name##_initArr   (const type* arr, size_t length); \
static Vec_##name  Vec_##name##_initVec   (const Vec_##name* other); \
static void        Vec_##name##_free      (Vec_##name* self); \
static void        Vec_##name##_reserve   (Vec_##name* self, size_t capacity); \
static void        Vec_##name##_push      (Vec_##name* self, type v); \
static void        Vec_##name##_pushArr   (Vec_##name* self, const type* arr, size_t length); \
static void        Vec_##name##_pushVec   (Vec_##name* self, const Vec_##name* other); \
static void        Vec_##name##_pop       (Vec_##name* self); \
static void        Vec_##name##_popN      (Vec_##name* self, size_t n); \
static void        Vec_##name##_insert    (Vec_##name* self, size_t index, type v); \
static void        Vec_##name##_insertArr (Vec_##name* self, size_t index, const type* arr, size_t length); \
static void        Vec_##name##_insertVec (Vec_##name* self, size_t index, const Vec_##name* other); \
static void        Vec_##name##_erase     (Vec_##name* self, size_t index); \
static void        Vec_##name##_eraseN    (Vec_##name* self, size_t index, size_t n); \
\
static bool  Vec_##name##_Internal_overlapPtr    (const Vec_##name* self, const type* p); \
static type* Vec_##name##_Internal_duplicateArr  (type* arr, size_t length); \
\
static Vec_##name Vec_##name##_init(void) \
{ \
	Vec_##name obj = { NULL, 0, 0 }; \
	return obj; \
} \
static Vec_##name Vec_##name##_initArr(const type *arr, size_t length) \
{ assert(arr); \
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
static Vec_##name Vec_##name##_initVec(const Vec_##name *other) \
{ assert(other); \
\
	if (!other->buffer || !other->length) { \
		return Vec_##name##_init(); \
	} \
	return Vec_##name##_initArr(other->buffer, other->length); \
} \
static void Vec_##name##_free(Vec_##name *self) \
{ assert(self); \
\
	free(self->buffer); \
	self->buffer = NULL; \
	self->length = 0; \
	self->capacity = 0; \
} \
static void Vec_##name##_reserve(Vec_##name *self, size_t capacity) \
{ assert(self); \
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
static void Vec_##name##_push(Vec_##name *self, type v) \
{ assert(self); \
\
	Vec_##name##_reserve(self, self->length + 1); \
	self->buffer[self->length++] = v; \
} \
static void Vec_##name##_pushArr(Vec_##name *self, const type *arr, size_t length) \
{ assert(self && arr); \
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
static void Vec_##name##_pushVec(Vec_##name* self, const Vec_##name *other) \
{ assert(self && other); \
\
	if (!other->buffer || !other->length) { \
		return; \
	} \
	Vec_##name##_pushArr(self, other->buffer, other->length); \
} \
static void Vec_##name##_pop(Vec_##name *self) \
{ assert(self && self->length); \
\
	self->length--; \
} \
static void Vec_##name##_popN(Vec_##name *self, size_t n) \
{ assert(self && self->length >= n); \
\
	self->length -= n; \
} \
static void Vec_##name##_insert(Vec_##name *self, size_t index, type v) \
{ assert(self && (index <= self->length)); \
\
	Vec_##name##_reserve(self, self->length + 1); \
	type *dst = self->buffer + index + 1; \
	type *src = self->buffer + index; \
	size_t size = self->length - index; \
	memmove(dst, src, size * sizeof(type)); \
	self->buffer[index] = v; \
	self->length++; \
} \
static void Vec_##name##_insertArr(Vec_##name *self, size_t index, const type *arr, size_t length) \
{ assert(self && arr && (index <= self->length)); \
\
	if (Vec_##name##_Internal_overlapPtr(self, arr)) \
	{ \
		type* arrayClone = Vec_##name##_Internal_duplicateArr(arr, length); \
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
static void Vec_##name##_insertVec(Vec_##name *self, size_t index, const Vec_##name *other) \
{ assert(self && other && (index <= self->length)); \
\
	if (!other->buffer || !other->length) { \
		return; \
	} \
	Vec_##name##_insertArr(self, index, other->buffer, other->length); \
} \
static void Vec_##name##_erase(Vec_##name *self, size_t index) \
{ assert(self && (index < self->length)); \
\
	type *dst = self->buffer + index; \
	type *src = self->buffer + index + 1; \
	size_t size = self->length - index - 1; \
	memmove(dst, src, size* sizeof(type)); \
	self->length--; \
} \
static void Vec_##name##_eraseN(Vec_##name *self, size_t index, size_t n) \
{ assert(self && (index <= self->length) && (n <= self->length) && (n <= (self->length - index))); \
\
	type *dst = self->buffer + index; \
	type *src = self->buffer + index + n; \
	size_t size = self->length - index - n; \
	memmove(dst, src, size * sizeof(type)); \
	self->length -= n; \
} \
\
static bool Vec_##name##_Internal_overlapPtr(const Vec_##name *self, const type *p) \
{ assert(self && p); \
\
	return (p >= self->buffer) && (p < (self->buffer + self->length)); \
} \
static type* Vec_##name##_Internal_duplicateArr(const type *arr, size_t length) \
{ assert(arr); \
\
	type* buffer = (type*)malloc(length * sizeof(type)); assert(buffer); \
	memcpy(buffer, arr, length * sizeof(type)); \
	return buffer; \
}

#endif // VEC_H