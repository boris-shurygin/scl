/**
 * @file:ir/opt_oper_des.c
 * Impelemtation of objects and operations in OPT-level IR
 * @ingroup MDES
 */
#include "opt_iface.hpp"

using namespace Opt;

template <> MDes::OperDes MDes::opers[ MDes::num_opers] = {};
template <> MDes::ObjDes MDes::objects[ MDes::num_objs] = {};

#define OPER( name, str, ...) initOperDes< __VA_ARGS__>( name, str);
#define OBJ( ...) initObjDes( __VA_ARGS__);
#define OPER_CLASSES( name, ...) { MDes::OperClass classes[] = {__VA_ARGS__}; initOperClasses( name, sizeof(classes)/sizeof(MDes::OperClass), classes);}

/*
 * Enclosed the MDes constructor specialization in IR namespace to resolve
 * 'specialization in different namespace compiler error'
 */
namespace IR
{
    /** Operations descriptions */
    template <> MDes::MDesImpl()
    {
        #include "opt_mdes_inc.hpp"
    }

};

namespace Opt
{
        /** Class check routines */
#define OPER_CLASS( name, func_prefix, ...) bool isOper ## func_prefix( Oper *oper){ return oper->archDes()->belongsToClass( name);}
#include "opt_oper_classes_inc.hpp"

}

MDes opt_mdes;

