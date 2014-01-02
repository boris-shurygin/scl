/**
 * @file:lowering/low_iface.h
 * Interface of lowering package
 * @defgroup LOW Lowering
 */

#pragma once

#include "../syntax/stx_iface.h"
#include "../opt/opt_iface.hpp"

/**
 * Debug assert for lowering library
 * @ingroup LOW
 */
#if !defined(LOW_ASSERTD)
#    define LOW_ASSERTD(cond) ASSERT_XD(cond, "Lowering", "")
#endif

/**
 * Debug assert for lowering library
 * @ingroup LOW
 */
#if !defined(LOW_ASSERTXD)
#    define LOW_ASSERTXD(cond, what) ASSERT_XD(cond, "Lowering", what)
#endif

#include "arithm2opt.h"

