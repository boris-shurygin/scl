/**
 * @file: ir/predecls.h
 * Predeclarations for interface of IR library
 */

#pragma once 

#include "../Utils/utils_iface.h"
#include "../Graph/graph_iface.h"

/**
 * Debug assert for IR library
 **/
#if !defined(IR_ASSERTD)
#    define IR_ASSERTD(cond) ASSERT_XD(cond, "IR", "")
#endif

/**
 * Debug assert for IR library
 **/
#if !defined(IR_ASSERTXD)
#    define IR_ASSERTXD(cond, what) ASSERT_XD(cond, "IR", what)
#endif

/** 
 * Operand const type  
 */
typedef UInt64 UConst;

/**
 * Operand type
 */
enum OperandType
{
    OP_TYPE_OBJ,
    OP_TYPE_IMM,
    OP_TYPE_TARGET,
    OP_TYPE_SET,
    OP_TYPES_NUM
};

/**
 * Directions of operand type 
 */
enum OpDir
{
    OPERAND_DIR_ARG,
    OPERAND_DIR_RES,
    OPERAND_DIR_USE = OPERAND_DIR_ARG,
    OPERAND_DIR_DEF = OPERAND_DIR_RES,
    OPERAND_DIRS_NUM
};

#define IR_ARGUMENTS_NUM (2)
#define IR_RESULTS_NUM (1)

namespace IR
{
    template <class MDesType> class  Function;
    template <class MDesType> class  Module;

    template <class MDesType> class  DG;
    template <class MDesType> class  Operation;
    template <class MDesType> class  DepEdge;

    template <class MDesType> class  CFG;
    template <class MDesType> class  CFNode;
    template <class MDesType> class  CFEdge;
};

