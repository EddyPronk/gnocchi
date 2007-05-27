#ifndef GNOCCHI_CONFIG_H
#define GNOCCHI_CONFIG_H

#define ATTRIBUTE_NORETURN
#define ATTRIBUTE_UNUSED
#define ATTRIBUTE_PRINTF_1
#define ATTRIBUTE_PRINTF_2

#define xrealloc realloc

#include "auto-host.h"
#ifdef IN_GCC
//# include "ansidecl.h"
#endif
#endif /* GNOCCHI_CONFIG_H */
