#include "String.h"

#include <stdio.h>

int main(void)
{
	String* str = string_create();
	str->assign(str, "Hello, test");
	printf("%s\n", str->get(str));
	str->cpush_back(str, 'h');
	printf("%s\n", str->get(str));
	str->pop_back(str);
	printf("%s\n", str->get(str));
	string_delete(str);
	return 0;
}