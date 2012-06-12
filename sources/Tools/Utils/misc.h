/**
 * @file: misc.h 
 * @defgroup Misc Miscellaneous
 * @brief Various auxiliary routines
 * @ingroup Utils
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef MISC_H
#define MISC_H

/**
 * Check if pointer is not null
 * @ingroup Misc
 */
inline bool isNotNullP( const void *pointer)
{
    return pointer != NULL;
}

/**
 * Check if pointer is null
 * @ingroup Misc
 */
inline bool isNullP( const void *pointer)
{
    return pointer == NULL;
}

/**
 * Check if pointers are equal
 * @ingroup Misc
 */
inline bool areEqP( const void *p1, const void *p2)
{
    return p1 == p2;
}

/**
 * Check if pointers are not equal
 * @ingroup Misc
 */
inline bool areNotEqP( const void *p1, const void *p2)
{
    return p1 != p2;
}

/**
 * @brief Get absolute value
 * @ingroup Misc
 * @param Value value type
 * @param val signed value used to get absolute value
 *
 * @par 
 * Returns absolute value for a given signed operand. Note that operators
 * "<" and "-" must be defined for parameter class
 */
template<class Value> inline Value abs( Value val)
{
    if( val < 0)
        return -val;

    return val;
}



/**
 * Conversion of string to signed 32-bit integer with success check
 */
inline Int32 convStr2Int32( std::string str, bool *ok = NULL)
{
    bool zero_value = (str == std::string("0"));
    Int32 val = atoi( str.c_str()); // On error returns 0, so we should check if input string is actually "0"
    if ( isNotNullP( ok))
    {
        if ( val == 0 && !zero_value)
        {
            *ok = false;
        } else
        {
            *ok = true;
        }
    }
    return val;
}

/**
 * Conversion of string to signed 64-bit integer with success check
 */
inline Int64 convStr2Int64( std::string str, bool *ok = NULL)
{
    bool zero_value = (str == std::string("0"));
    Int64 val = atol( str.c_str());// On error returns 0, so we should check if input string is actually "0"
    if ( isNotNullP( ok))
    {
        if ( val == 0 && !zero_value)
        {
            *ok = false;
        } else
        {
            *ok = true;
        }
    }
    return val;
}

/**
 * Conversion of string to double with success check
 */
inline Double convStr2Double( std::string str, bool *ok = NULL)
{
    bool zero_value = ( str == std::string("0")
                        || str == std::string("0.0") );
    Double val = atof( str.c_str());// On error returns 0, so we should check if input string is actually "0"
    if ( isNotNullP( ok))
    {
        if ( val == 0 && !zero_value)
        {
            *ok = false;
        } else
        {
            *ok = true;
        }
    }
    return val;
}


#endif