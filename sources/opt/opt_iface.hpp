/**
 * @file: opt_iface.hpp
 * Interface of optimizer library
 * @defgroup OPT Optimizer
 */
#pragma once 

#include "../ir/ir_iface.hpp"
#include "opt_mdes.hpp"

/** Middle-end optimizer of SCL */
namespace Opt
{
    /** Typedefs for IR classes used in optimizer's intermediate representation */
    typedef IR::Module< MDes> Module;
    typedef IR::Function< MDes> Function;
    typedef IR::CFG< MDes> CFG;
    typedef IR::CFNode< MDes> CFNode;
    typedef IR::CFEdge< MDes> CFEdge;
    typedef IR::Operation< MDes> Oper;
    typedef IR::Object< MDes> Obj;

    bool uTest();
};
