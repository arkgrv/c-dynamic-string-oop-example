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

		uint32_t (*size)(const struct dstring_t* THIS);
		uint32_t (*capacity)(const struct dstring_t* THIS);
		int (*empty)(const struct dstring_t* THIS);

		void (*assign)(struct dstring_t* THIS, const char* str);
		const char* (*get)(const struct dstring_t* THIS);

		void (*pop_back)(struct dstring_t* THIS);
		void (*cpush_back)(struct dstring_t* THIS, char c);
		void (*spush_back)(struct dstring_t* THIS, const char* str);
        
        char (*at)(struct dstring_t* THIS, int pos);

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
