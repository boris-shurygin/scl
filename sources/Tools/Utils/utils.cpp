/**
 * @file: mem_utest.cpp 
 * Implementation of various utils
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"

/**
 * Die with message
 */
void fatal(const char *msg, ...)
{
    abort();
}

/*
  The ASSERT macro calls this this function when the test fails.
*/
void fatalAssert(const char *assertion, const char *file, int line)
{
    fatal("ASSERT: \"%s\" in file %s, line %d", assertion, file, line);
}

/*
  The ASSERT_X macro calls this this function when the test fails.
*/
void fatalAssertWithMess(const char *where, const char *what, const char *file, int line)
{
    fatal("ASSERT failure in %s: \"%s\", file %s, line %d", where, what, file, line);
}