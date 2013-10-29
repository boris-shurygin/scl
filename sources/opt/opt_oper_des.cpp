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

/** Operations descriptions */
template <> MDes::MDesImpl()
{
    #include "opt_mdes_inc.hpp"
}

MDes opt_mdes;