/**
 * @file:ir/opt_mdes_inc.hpp
 * Macros for auto-generation of objects and operations in OPT-level IR
 * @ingroup MDES
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */
/** No pragma once, file may be included several times */
#ifndef OBJ
#  define OBJ( name, prefix, max_size, num, storage_class, can_be_virtual)
#endif
#ifndef OPER
#  define OPER( name, mnem, ...)
#endif
#ifndef OPER_CLASSES
#  define OPER_CLASSES( name, ...)
#endif

/* Objects */
OBJ( Reg, "t", 32, 0, REGISTER, true)

/* Movs */
OPER( Mov,  "mov",  Args<reg_imm_32>, Ress<reg_32> )

/* Arithmetic Operations */
OPER( Add,  "add",  Args<reg_imm_32, reg_imm_32>, Ress<reg_32> )
OPER( Sub,  "sub",  Args<reg_imm_32, reg_imm_32>, Ress<reg_32> )
OPER( Mul,  "mul",  Args<reg_imm_32, reg_imm_32>, Ress<reg_32> )
OPER( Div,  "div",  Args<reg_imm_32, reg_imm_32>, Ress<reg_32> )

/* Memory operations */
OPER( Ld,   "ld",   Args<reg_imm_32>            , Ress<reg_32> ) OPER_CLASSES( Ld, MEM, MEM_READ)
OPER( St,   "st",   Args<reg_imm_32, reg_imm_32>, Ress<>       ) OPER_CLASSES( St, MEM, MEM_WRITE)

/* Branches */
OPER( Br,   "br",   Args<target>                    , Ress<> ) OPER_CLASSES( Br,   CF)
OPER( Brl,  "brl",  Args<reg_32, reg_imm_32, target>, Ress<> ) OPER_CLASSES( Brl,  CF)
OPER( Brg,  "brg",  Args<reg_32, reg_imm_32, target>, Ress<> ) OPER_CLASSES( Brg,  CF)
OPER( Brle, "brle", Args<reg_32, reg_imm_32, target>, Ress<> ) OPER_CLASSES( Brle, CF)
OPER( Brge, "brge", Args<reg_32, reg_imm_32, target>, Ress<> ) OPER_CLASSES( Brge, CF)
OPER( Bre,  "bre",  Args<reg_32, reg_imm_32, target>, Ress<> ) OPER_CLASSES( Bre,  CF)
OPER( Brne, "brne", Args<reg_32, reg_imm_32, target>, Ress<> ) OPER_CLASSES( Brne, CF)

/* Frame control */
OPER( Call, "call", Args<reg_imm_32, arg_set>, Ress<res_set> )
OPER( Ret,  "ret" , Args<arg_set>           , Ress<>         )

#undef OPER
#undef OBJ
#undef OPER_CLASSES

