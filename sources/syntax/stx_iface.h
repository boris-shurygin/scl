/**
 * @file:syntax/stx_iface.h
 * Interface of syntax package
 * @defgroup STX Syntax
 */
#pragma once

#include "../Tools/tools_iface.h"

/**
 * Debug assert for syntax library
 * @ingroup STX
 */
#if !defined(STX_ASSERTD)
#    define STX_ASSERTD(cond) ASSERT_XD(cond, "Syntax", "")
#endif

/**
 * Debug assert for syntax library
 * @ingroup STX
 */
#if !defined(STX_ASSERTXD)
#    define STX_ASSERTXD(cond, what) ASSERT_XD(cond, "Syntax", what)
#endif

#include "ast.h"
#include "arithmetics.h"

