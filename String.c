#include "String.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define STRNPOS -1

/*
 * Definition of string internal storage.
 * The string internal storage object will contain the string
 * buffer and all the data necessary to correctly manage a dynamic string
 * such as size and capacity.
*/
struct storage_t {
	char* buffer;
	uint32_t size;
	uint32_t capacity;
};

/**
 * Auxiliary function to calculate next allocation size in bytes.
*/
inline static int nsize(int size)
{
	--size;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size |= size >> 8;
	size |= size >> 16;
	++size;
	return size;
}

void reallocate(struct storage_t* str, uint32_t newsize)
{
    newsize = nsize(newsize);
	char* tmp = realloc(str->buffer, sizeof(char) * newsize);
	if (NULL != tmp) str->buffer = tmp;
#ifdef DEBUG
	else {
		fprintf(stderr, "Error: reallocation of string failed\n");
		assert(0);
	}
#endif
}

/**
 * Static string management functions.
 * These will act as "member functions or methods" of the main string
 * object, being called via function pointers when needed. The user may not
 * see or use the underlying implementation.
*/

static int compare(const struct dstring_t* THIS, const struct dstring_t* other)
{
	if (NULL == THIS || NULL == other) return 0;
	return strcmp(THIS->data->buffer, other->data->buffer);
}

// Returns the size of the string (length)
static int size(const struct dstring_t* THIS)
{
	if (NULL == THIS) return 0;
	return THIS->data->size;
}

// Returns the effective size of the string in bytes. This represents
// the memory space taken by the string.
static int capacity(const struct dstring_t* THIS)
{
	if (NULL == THIS) return 0;
	return THIS->data->capacity;
}

// Returns true if the string is empty
static int empty(const struct dstring_t* THIS)
{
	if (NULL == THIS) return 1;
	uint32_t i;
	for (i = 0; i < THIS->data->size; ++i) {
		if ('\0' != THIS->data->buffer[i]) return 0;
	}
	return 1;
}
    
// Reserves memory for the string
static void reserve(struct dstring_t* THIS, int size)
{
    if (NULL == THIS || NULL == THIS->data) return;
    char* buf = realloc(THIS->data->buffer, sizeof(char) * nsize(size));
    if (NULL == buf) return;
    THIS->data->buffer = buf;
    THIS->data->capacity = size;
}
    
// Resizes the string, truncating to the end value if the size
// is less than the preallocated string, or filling in with
// zero values if the string is needed to grow
static void resize(struct dstring_t* THIS, int new_size)
{
    if (NULL == THIS || NULL == THIS->data) return;
    char* buf = realloc(THIS->data->buffer, size);
    if (NULL == buf) return;
    THIS->data->buffer = buf;
    THIS->data->size = new_size;
    THIS->data->capacity = new_size;
}

// Assigns a new value to the string, as a const char* parameter
static void assign(struct dstring_t* THIS, const char* str)
{
	// Checking pointers
	if (NULL == THIS) return;	// just do nothing
	else if (NULL == str) {
		// free storage and reset string
		free(THIS->data->buffer);
		THIS->data->size = 0;
		THIS->data->capacity = 0;
	}
	else {
		if (NULL != THIS->data->buffer)
			free(THIS->data->buffer);

		THIS->data->size = strlen(str);
		THIS->data->capacity = THIS->data->size + 1;
		THIS->data->buffer = malloc(sizeof(char) * THIS->data->capacity);

		// NULL check
		if (NULL == THIS->data->buffer) {
			// Check fail, reset string
			THIS->data->size = 0;
			THIS->data->capacity = 0;
		}
		else {
			// Copy string content
			strncpy(THIS->data->buffer, str, THIS->data->size);
			THIS->data->buffer[THIS->data->size] = '\0';
		}
	}
}

// Returns a pointer to the buffer
static const char* get(const struct dstring_t* THIS)
{
	if (NULL == THIS || NULL == THIS->data || NULL == THIS->data->buffer)
		return "\0";
	return THIS->data->buffer;
}

// Removes the last character from the string
static void pop_back(struct dstring_t* THIS)
{
	if (NULL == THIS) return;
	THIS->data->buffer[THIS->data->size] = '\0';
	THIS->data->size--;
}

// Adds a character to the end of the string reallocating if necessary
static void cpush_back(struct dstring_t* THIS, char c)
{
	if (NULL == THIS) return;
	if (THIS->data->size + 2 >= THIS->data->capacity)
		reallocate(THIS->data, THIS->data->size + 2);

	if (NULL == THIS->data->buffer) return;
	THIS->data->buffer[THIS->data->size] = c;
	THIS->data->buffer[THIS->data->size + 1] = '\0';
}

static void spush_back(struct dstring_t* THIS, const char* str)
{
	if (NULL == THIS) return;
	// Appendable string length
	size_t len = strlen(str);
	if (THIS->data->size + len + 2 >= THIS->data->capacity)
		reallocate(THIS->data, THIS->data->size + len + 2);

	if (NULL == THIS->data->buffer) return;

	// Updating size
	THIS->data->size += len;

	strcat(THIS->data->buffer, str);
	THIS->data->buffer[THIS->data->size + 1] = '\0';
}

static int cfind(struct dstring_t* THIS, char c)
{
	if (NULL == THIS || NULL == THIS->data->buffer) return STRNPOS;
	char* found = strchr(THIS->data->buffer, c);
	return NULL == found ? STRNPOS : found - THIS->data->buffer;
}

static int sfind(struct dstring_t* THIS, const char* str)
{
	if (NULL == THIS || NULL == THIS->data->buffer) return STRNPOS;
	char* found = strstr(THIS->data->buffer, str);
	return NULL == found ? STRNPOS : found - THIS->data->buffer;
}
    
static char at(struct dstring_t* THIS, int pos)
{
    if (NULL == THIS || pos < 0 || pos > size(THIS)) return '\0';
    return THIS->data->buffer[pos];
}
    
static char front(struct dstring_t* THIS)
{
    if (NULL == THIS) return '\0';
    return *(THIS->data->buffer);
}
    
static char back(struct dstring_t* THIS)
{
    if (NULL == THIS) return '\0';
    return *(THIS->data->buffer + (THIS->data->size - 1));
}
    
static char* begin(struct dstring_t* THIS)
{
    if (NULL == THIS) return NULL;
    return THIS->data->buffer;
}
    
static char* end(struct dstring_t* THIS)
{
    if (NULL == THIS) return NULL;
    return (THIS->data->buffer + (THIS->data->size - 1));
}

// Removes all characters from the string
static void clear(struct dstring_t* THIS)
{
	if (NULL == THIS) return;
	memset(THIS->data->buffer, '\0', THIS->data->capacity);
}

String* string_create()
{
	String* str = malloc(sizeof(struct dstring_t));
	if (NULL == str) {
		fprintf(stderr, "Error: unable to allocate string storage\n");
		exit(EXIT_FAILURE);
	}

	// Allocating internal data storage
	str->data = malloc(sizeof(struct storage_t));
	if (NULL == str->data) {
		fprintf(stderr, "Error: unable to allocate string storage\n");
		exit(EXIT_FAILURE);
	}

	str->data->buffer = NULL;
	str->data->size = 0;
	str->data->capacity = 0;

	// Assigning function pointers
	str->assign = &assign;
	str->get = &get;
	str->compare = &compare;
	str->size = &size;
	str->capacity = &capacity;
    str->reserve = &reserve;
    str->resize = &resize;
	str->empty = &empty;
	str->pop_back = &pop_back;
	str->cpush_back = &cpush_back;
	str->spush_back = &spush_back;
	str->cfind = &cfind;
	str->sfind = &sfind;
	str->clear = &clear;
    str->at = &at;
    str->front = &front;
    str->back = &back;
    str->begin = &begin;
    str->end = &end;

	return str;
}

void string_delete(struct dstring_t* str)
{
	if (NULL == str) return;
	free(str->data->buffer);
	free(str->data);
	free(str);
}

#ifdef __cplusplus
}
#endif
