cmake_minimum_required(VERSION 3.10)
project (CDynamicString C)

set(CMAKE_C_STANDARD 99)

if (MSVC)
	set(CMAKE_C_FLAGS_DEBUG "/MTd /TC /Zi /Od")
	set(CMAKE_C_FLAGS_RELEASE "/TC /Ot")
else()
	set(CMAKE_C_FLAGS_DEBUG "-Wall -g -O0 -fPIC -fsanitize=address")
	set(CMAKE_C_FLAGS_RELEASE "-Os -fPIC")
endif()

set(LIBSOURCES String.c String.h)
set(EXECSOURCES Main.c)

set(LIBNAME string)
set(EXECNAME Example)

add_library(${LIBNAME} STATIC ${LIBSOURCES})
add_executable(${EXECNAME} ${EXECSOURCES})

target_link_libraries(${EXECNAME} PUBLIC ${LIBNAME})
