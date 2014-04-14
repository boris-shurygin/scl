/**
 * @file:ir/opt_mdes_inc.hpp
 * Macros for auto-generation of operations groups in OPT-level IR
 * @ingroup MDES
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */
#ifndef OPER_CLASS
#  define OPER_CLASS( name, func_prefix, ...)
#endif

OPER_CLASS( CF, Cf)

#undef OPER_CLASS
