/**
 * @file: host.h 
 * Assertion related routines of Utils library for compiler prototype project
 * @defgroup Asserts Assertions
 * @brief Assertion routines/macros
 * @ingroup Utils
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once
#ifndef UTILS_HOST_H
#define UTILS_HOST_H

/* System headers */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <list>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <numeric>
#include <iomanip>

using namespace std;

/* Path-related routines */
namespace Host
{
    /**
     * Concatenate path strings
     */
    inline std::string concatPaths( const std::string &basedir,
                                    const std::string &subpath)
    {
        char sep = '/'; //Linux separator
        string tmp = basedir;

#ifdef OS_WIN
        sep = '\\'; // Windows separator
#endif

        if ( basedir[ basedir.length()] != sep) { // Check if basedir string ends with separator
            tmp += sep;                //add separator
            return (tmp + subpath);
        } else
        {
            return (basedir + subpath);
        }
    }
}

#endif /* UTILS_HOST_H */
