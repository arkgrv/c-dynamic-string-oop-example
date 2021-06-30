#ifndef STRING_C_H
#define STRING_C_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	/*
		String internal data storage
	*/
	typedef struct storage_t* storage_type;

	/* 
		Effective string representation
	*/
	typedef struct dstring_t {
		storage_type data;

		int (*compare)(const struct dstring_t* THIS, const struct dstring_t* other);

		int (*size)(const struct dstring_t* THIS);
		int (*capacity)(const struct dstring_t* THIS);
		int (*empty)(const struct dstring_t* THIS);
        
        void (*reserve)(struct dstring_t* THIS, int size);
        void (*resize)(struct dstring_t* THIS, int new_size);

		void (*assign)(struct dstring_t* THIS, const char* str);
		const char* (*get)(const struct dstring_t* THIS);

		void (*pop_back)(struct dstring_t* THIS);
		void (*cpush_back)(struct dstring_t* THIS, char c);
		void (*spush_back)(struct dstring_t* THIS, const char* str);
        
        char (*at)(struct dstring_t* THIS, int pos);
        char (*front)(struct dstring_t* THIS);
        char (*back)(struct dstring_t* THIS);
        
        char* (*begin)(struct dstring_t* THIS);
        char* (*end)(struct dstring_t* THIS);

		int (*cfind)(struct dstring_t* THIS, char c);
		int (*sfind)(struct dstring_t* THIS, const char* str);

		void (*clear)(struct dstring_t* THIS);
	} String;

	/*
		Basic creation and destruction functions
	*/
	String* string_create();
	//String* string_createl(int length);
	void string_delete(struct dstring_t* str);

#ifdef __cplusplus
}
#endif

#endif
