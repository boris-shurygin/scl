/**
 * @file: opt_iface.hpp
 * Interface of optimizer library
 * @defgroup OPT Optimizer
 */
#pragma once 

#include "../ir/ir_iface.hpp"


/** Middle-end optimizer of SCL */
namespace Opt
{
    using namespace IR;

#define OBJ( name, ...) name,

    /** Object types */
    enum OptObjectName
    {
#include "opt_mdes_inc.hpp"
        OPT_OBJECTS_NUM
    };

    /** Objects descriptions */

#define OPER( name, mnem, ...) name,

    /** Operation names */
    enum OptOperName
    {
#include "opt_mdes_inc.hpp"
        
        OPT_OPERS_NUM
    };

#define OPER_CLASS( name, func_prefix, ...) name,

    /** Operation classes */
    enum OptOperClass
    {
#include "opt_oper_classes_inc.hpp"

        OPT_OPER_CLASSES_NUM
    };

    class Oper;
}

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
    typedef IR::Object< MDes> Obj;
    
    /** Unit test for optimizer */
    bool uTest( UnitTest *utest);
};

#include "opt_oper.hpp"




