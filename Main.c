#include "String.h"

#include <stdio.h>

int main(void)
{
    /**
     This phase represents the equivalent of a "construction" of a string
     object in any object oriented programming language.
     Java uses the keyword new, while C++ uses implicit constructor calling
     in order to simplify the syntax.
     
     C does not have the luxury of constructors, so you have to make a
     "factory function" which is going to have to allocate the necessary memory and THEN
     apply operations to the function pointers representing the string object methods.
     
     This may seem counter-intuitive at first, but this kind of explicit declaration may
     in fact help to understand how modern object-oriented programming languages work.
     */
	String* str = string_create();
    
    // The following operations are the same as
    // using dot notation access in languages like Java
    // and C++. Instead, since we operate on pointers,
    // we have to use the arrow notation.
    
    // C does not have operator overloading, so it needs a
    // function which can be used instead of the = sign.
	str->assign(str, "Hello, test");
	printf("%s\n", str->get(str));
    
    // Adds a character to the end of the string, much like
    // the analogous function in C++.
	str->cpush_back(str, 'h');
	printf("%s\n", str->get(str));
    
    // Removes the last character of the string.
	str->pop_back(str);
    
	printf("%s\n", str->get(str));
    
    // This will append a string to the end of the current object.
	str->spush_back(str, "Test Test String Cat");
	printf("%s\n", str->get(str));

    str->spush_back(str, "Prova prova prova ciao come stai");
    printf("%s\n", str->get(str));
    
    // Destroys the string object.
	string_delete(str);
	return 0;
}
