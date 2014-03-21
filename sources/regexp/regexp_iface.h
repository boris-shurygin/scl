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

#define ENABLE_RE_LOGS

#ifdef ENABLE_RE_LOGS
#  define RE_LOGS( message) LOGVS( LOG_REG_EXP, 0, message)
#  define RE_LOG_INC_INDENT LOG_INC_INDENT( LOG_REG_EXP)
#  define RE_LOG_DEC_INDENT LOG_DEC_INDENT( LOG_REG_EXP)
#else
#  define RE_LOGS( message)
#  define RE_LOG_INC_INDENT
#  define RE_LOG_DEC_INDENT
#endif

#include "fa.h"
#include "regexp.h"

