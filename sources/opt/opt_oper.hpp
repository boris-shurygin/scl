/**
 * @file:ir/opt_mdes.hpp
 * Description of objects and operations in OPT-level IR
 * @ingroup MDES
 */
#pragma once

namespace Opt
{
/**
 * Operation class for optimizer
 */
class Oper: public IR::Operation< MDes>
{
public:
    typedef OptOperName OperName;
    typedef OptObjectName ObjectName;
    typedef OptOperClass OperClass;

    Oper( IR::DG<MDes> *g): IR::Operation< MDes>( g) {};

    /** Class check routines */
#define OPER_CLASS( name, func_prefix, ...) bool is ## func_prefix(){ return archDes()->belongsToClass( name);}
#include "opt_oper_classes_inc.hpp"

};


    /** Class check routines */
#define OPER_CLASS( name, func_prefix, ...) bool isOper ## func_prefix( Oper *oper);
#include "opt_oper_classes_inc.hpp"

} // namespace Opt