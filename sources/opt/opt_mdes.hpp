/**
 * @file:ir/opt_mdes.hpp
 * Description of objects and operations in OPT-level IR
 * @ingroup MDES
 */

#pragma once

namespace Opt
{
    /** Machine description for optimizer */
    typedef MDesImpl< Oper, OptOperName, OptObjectName, OptOperClass, OPT_OBJECTS_NUM, OPT_OPERS_NUM, 3, 1, OPT_OPER_CLASSES_NUM> MDes;

    /* Feasible operands descriptions */
    typedef MDes::OpType<OPT_OBJECTS_NUM, 32, CANNOT_BE_IMM, CAN_BE_TARGET> target;
    typedef MDes::OpType<OPT_OBJECTS_NUM, 32, CANNOT_BE_IMM, CANNOT_BE_TARGET, CAN_BE_SET> arg_set;
    typedef MDes::OpType<OPT_OBJECTS_NUM, 32, CANNOT_BE_IMM, CANNOT_BE_TARGET, CAN_BE_SET> res_set;
    typedef MDes::OpType<Reg, 32, CAN_BE_IMM> reg_imm_32;
    typedef MDes::OpType<Reg, 32, CANNOT_BE_IMM> reg_32;
    typedef MDes::OpType<OPT_OBJECTS_NUM, 32, CAN_BE_IMM> imm_32;
  
};
