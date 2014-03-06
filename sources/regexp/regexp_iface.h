/**
 * @file:regexp/regexp_iface.h
 * Interface of regular expressions package
 * @defgroup REGEXP RegExp
 */
#pragma once

#include "../Tools/tools_iface.h"

/**
 * Debug assert for regexp library
 * @ingroup REGEXP
 */
#if !defined(RE_ASSERTD)
#    define RE_ASSERTD(cond) ASSERT_XD(cond, "RegExp", "")
#endif

/**
 * Debug assert for regexp library
 * @ingroup REGEXP
 */
#if !defined(RE_ASSERTXD)
#    define RE_ASSERTXD(cond, what) ASSERT_XD(cond, "RegExp", what)
#endif

#include "fa.h"
#include "regexp.h"

