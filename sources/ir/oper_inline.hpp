/**
 * @file oper_inline.hpp
 * Implmentation of AbstractOperation class inline routines
 */

#pragma once 

namespace IR
{

template <class MDesType> void 
Object<MDesType>::setVirtual( bool is_virtual)
{
    this->is_virtual = is_virtual;
}

template <class MDesType> void 
Object<MDesType>::setId( UInt32 id_)
{
    this->id_ = id_;
}

template <class MDesType> bool 
Object<MDesType>::isVirtual() const
{
    return is_virtual;
}

template <class MDesType> UInt32 
Object<MDesType>::id() const
{
    return id_;
}

template <class MDesType> 
Object<MDesType>::Object()
{
    setVirtual( true);
    setId( (UInt32) -1); 
}

template <class MDesType> 
Object<MDesType>::~Object()
{
}

template <class MDesType> void
Object<MDesType>::toStream(ostream& os) const
{
    typename MDesType::ObjDes *des = MDesType::objectDescription( type());
    
    if ( isVirtual() )
        os << 'v';
    os << des->prefix << id();    
}

template <class MDesType> void 
Operand<MDesType>::setObject( Object<MDesType>* obj)
{
    this->obj = obj;
}

template <class MDesType> void 
Operand<MDesType>::setImplicit( bool is_implicit)
{
    this->is_implicit = is_implicit;
}

template <class MDesType> void 
Operand<MDesType>::setConstValue( Const i_value)
{
    this->i_value = i_value;
}

template <class MDesType> void 
Operand<MDesType>::setType( OperandType type)
{
    this->type_ = type;
}

template <class MDesType> 
Object<MDesType> *
Operand<MDesType>::object() const
{
    return obj;
}

template <class MDesType> bool 
Operand<MDesType>::isImplicit() const
{
    return is_implicit;
}

template <class MDesType> Const 
Operand<MDesType>::constValue() const
{
    return i_value;
}

template <class MDesType>
Operation< MDesType> *
Operand<MDesType>::target() const
{
    return target_;
}

template <class MDesType>
void
Operand<MDesType>::setTarget( Operation< MDesType> *target_oper)
{
    target_ = target_oper;
}

template <class MDesType> OperandType
Operand<MDesType>::type() const
{
    return type_;
}

template <class MDesType> 
Operand<MDesType>::Operand(): target_( NULL), i_value( (Int64)0) 
{
    setObject( NULL);
    setImplicit( false);
    setType( OP_TYPES_NUM);
}

template <class MDesType> 
Operand<MDesType>::~Operand()
{

}

template <class MDesType> void
Operand<MDesType>::toStream(ostream& os) const
{
    switch ( type() )
    {
        case OP_TYPE_OBJ:
        {
            IR_ASSERTD( isNotNullP( object()));
            os << object();
            break;
        }
        case OP_TYPE_IMM:
            os << constValue(); 
            break;
        case OP_TYPE_TARGET:
        {
            IR_ASSERTD( isNotNullP( target()));
            os << "<label_" << target()->id() << '>';
            break;
        }
        case OP_TYPE_SET:
            break;
        case OP_TYPES_NUM:
            os << "{unset}";
            break;

        default:
            IR_ASSERTD( 0);
    }   
}
 
/** Get number of arguments */
template <class MDesName> UInt32 Operation<MDesName>::numArgs() const
{
    return MDesName::operDescription( name())->num_args; 
}

/** Get number of results */
template <class MDesName> UInt32 Operation<MDesName>::numRess() const
{
    return MDesName::operDescription( name())->num_ress; 
}

template <class MDesName> 
const Operand<MDesName>&
Operation<MDesName>::arg( unsigned i) const
{
    IR_ASSERTXD( i < numArgs(), "Invalid argument number");
    return args[ i];
}

template <class MDesName> 
const Operand<MDesName>&
Operation<MDesName>::res( unsigned i) const
{
    IR_ASSERTXD( i < numRess(), "Invalid result number");
    return ress[ i];
}

/** Set type of object */
template <class MDesType> 
void Object<MDesType>::setType( ObjName tp)
{
    type_ = tp;
}   

/** Set operation's argument object */
template <class MDesType> void 
Operation<MDesType>::setArgObj( UInt32 arg_num, Object< MDesType> *obj)
{
    IR_ASSERTD( arg_num < numArgs());
    
    /** Check that given operand can actually hold object according to MDes */
    typename MDesType::OperDes *des = MDesType::operDescription( name());
    typename MDesType::OpDes *op_des = des->argDes( arg_num);

    IR_ASSERTD( isNotNullP( op_des));
    IR_ASSERTD( op_des->isProperName( obj->type()) );
    IR_ASSERTD( args[ arg_num].type() != OP_TYPE_TARGET);

    /** Set proper operand type */
    args[ arg_num].setType( OP_TYPE_OBJ);

    /** Set operand object */
    args[ arg_num].setObject( obj);
}

/** Set operation's argument object */
template <class MDesType> void 
Operation<MDesType>::setArgTarget( UInt32 arg_num, Operation<MDesType> *target)
{
    IR_ASSERTD( arg_num < numArgs());
    
    /** Check that given operand can actually hold object according to MDes */
    typename MDesType::OperDes *des = MDesType::operDescription( name());
    typename MDesType::OpDes *op_des = des->argDes( arg_num);

    IR_ASSERTD( isNotNullP( op_des));
    IR_ASSERTD( op_des->canBeTarget() );

    /** Set proper operand type */
    args[ arg_num].setType( OP_TYPE_TARGET);

    /** Set operand object */
    args[ arg_num].setTarget( target);

}

/** Set operation's argument object */
template <class MDesType> void 
Operation<MDesType>::setArgImm( UInt32 arg_num, Const imm_val)
{
    IR_ASSERTD( arg_num < numArgs());
    
    /** Check that given operand can actually be immediate according to MDes */
    typename MDesType::OperDes *des = MDesType::operDescription( name());
    typename MDesType::OpDes *op_des = des->argDes( arg_num);

    IR_ASSERTD( isNotNullP( op_des));
    IR_ASSERTD( op_des->canBeImm() );
    IR_ASSERTD( args[ arg_num].type() != OP_TYPE_TARGET);

    /** Set proper operand type */
    args[ arg_num].setType( OP_TYPE_IMM);

    /** Set operand immediate value */
    args[ arg_num].setConstValue( imm_val);
}

/** Set operation's argument object */
template <class MDesType> void 
Operation<MDesType>::setResObj( UInt32 res_num, Object< MDesType> *obj)
{
    IR_ASSERTD( res_num < numRess());
    
    /** Check that given operand can actually hold object according to MDes */
    typename MDesType::OperDes *des = MDesType::operDescription( name());
    typename MDesType::OpDes *op_des = des->resDes( res_num);

    IR_ASSERTD( isNotNullP( op_des));
    IR_ASSERTD( op_des->isProperName( obj->type()) );

    /** Set proper operand type */
    ress[ res_num].setType( OP_TYPE_OBJ);

    /** Set operand object */
    ress[ res_num].setObject( obj);
}

/** Return type of object */
template <class MDesType> 
typename Object<MDesType>::ObjName
Object<MDesType>::type() const
{
    return type_;    
}

/** Set name of operation */
template <class MDesType> 
void Operation<MDesType>::setName( OperName nm)
{
    IR_ASSERTD( nm < MDesType::num_opers);
    name_ = nm;
}   

/** Return name of operation */
template <class MDesName> 
typename Operation<MDesName>::OperName
Operation<MDesName>::name() const
{
    return name_;    
}

/** Operation constructor */
template < class MDesType> 
Operation<MDesType>::Operation( DG <MDesType> *g):
    Node< DG< MDesType>, Operation< MDesType>, DepEdge<MDesType> >( g), // Dependence graph node constructor
    first_arg( NULL), // We don't have any arguments in the set on operation construction
    first_res( NULL), // We don't have any results in the set on operation construction
    node_( NULL)      // Node is not define at the moment of operation construction
{
    
}

/** Operation destructor */
template < class MDesType> 
Operation<MDesType>::~Operation()
{
    deleteList< Operand<MDesType> >( first_arg);
    deleteList< Operand<MDesType> >( first_res);
}


/** Print operand list to stream */
template < class MDesType> 
void
Operation<MDesType>::opListToStream(ostream& os, Operand< MDesType>* op) const
{
    if ( isNotNullP( op))
    {        
        bool first = true;
        os << '(';
        while ( isNotNullP( op) )
        {
            if ( !first )
            {
                os << ", ";   
            }
            else
            {
                first = false;
            }
            os << *op;
            op = op->next();
        }
        os << ')';
    }
}


/** Get the mnemonic string of the operation */
template < class MDesType> 
string &
Operation<MDesType>::nameStr() const
{
    typename MDesType::OperDes *des = MDesType::operDescription( name());

    return des->mnemonic_str;
}

/** Dump operation to stream */
template < class MDesType> 
void
Operation<MDesType>::toStream(ostream& os) const
{
    typename MDesType::OperDes *des = MDesType::operDescription( name());

    /* Print id */
    os << '{' << this->id() << "} ";
   
    /* Mnemonic string */
    os << des->mnemonic_str;

    UInt32 ress_printed = 0;

    /* Results */
    for ( UInt32 i = 0, first = 1; i < numRess(); i++)
    {  
        /* Skip results if it have to be res set */
        if ( des->resDes( i)->can_be_set) 
        {
            if ( isNotNullP( first_res))
            {
                if ( !first)
                   os << ',';   
                else
                   first = 0;
                
                os << ' ';
                
                opListToStream( os, first_res);
                ress_printed++;
            }
       } else
       {
           if ( !first)
               os << ',';   
           else
               first = 0;
           os << ' ' << res( i);
           ress_printed++;
       }
   }
   if ( ress_printed > 0)
      os << " =";
  
    /* Arguments */
    for ( UInt32 i = 0, first = 1; i < numArgs(); i++)
    {  
        /* Skip arguments if it have to be res set */
        if ( des->argDes( i)->can_be_set) 
        {
            if ( isNotNullP( first_arg))
            {
                if ( !first)
                   os << ',';   
                else
                   first = 0;
                os << ' ';
                opListToStream( os, first_arg);
            }
       } else
       {
           if ( !first)
               os << ',';
           else
               first = 0;   
           os << ' ' << arg( i);
       }
    }
    /* Newline */
    os << endl;
}

/** Detach from operation sequence */
template < class MDesType> 
void
Operation<MDesType>::detachFromSeq()
{
    return OperSeqList::detach();
}

/** Next in operation sequence */
template < class MDesType> 
Operation< MDesType> *
Operation<MDesType>::nextOper() const
{
    return OperSeqList::next();
}

/** Previous in operation sequence */
template < class MDesType> 
Operation< MDesType> *
Operation<MDesType>::prevOper() const
{
    return OperSeqList::prev();
}

/** Insert operation into sequence after the given operation */
template < class MDesType> 
void
Operation<MDesType>::insertAfter( Operation< MDesType> *prev_oper)
{
    OperSeqList::detach();
    prev_oper->OperSeqList::attach( this);
}

/** Insert operation into sequence before the given operation */
template < class MDesType> 
void
Operation<MDesType>::insertBefore( Operation< MDesType> *next_oper)
{
    OperSeqList::detach();
    next_oper->OperSeqList::attachInDir( this, LIST_DIR_RDEFAULT);
}

/** Get operation's function */
template < class MDesType> 
Function< MDesType> *
Operation<MDesType>::function() const
{
    return static_cast<Function< MDesType> *>(this->graph());
}

/** Add object argument */
template < class MDesType> 
const Operand< MDesType> *
Operation<MDesType>::addArgToSet( Object< MDesType> *obj)
{
    /** Check that the operation can actually have argument set according to MDes */
    typename MDesType::OperDes *des = MDesType::operDescription( name());
    IR_ASSERTD( des->canHaveArgSet() );

    /* Create new operand, set its type and assign object to it */
    Operand< MDesType> *op = new Operand< MDesType>();
    op->setType( OP_TYPE_OBJ);
    op->setObject( obj);
    
    /* Operand list bookeeping */
    op->attach( first_arg);
    first_arg = op;
    return op;
}

/** Add immediate argument */
template < class MDesType> 
const Operand< MDesType> *
Operation<MDesType>::addArgToSet( Const imm_val)
{
    /** Check that the operation can actually have argument set according to MDes */
    typename MDesType::OperDes *des = MDesType::operDescription( name());
    IR_ASSERTD( des->canHaveArgSet() );
    
    /* Create new operand, set its type and assign object to it */
    Operand< MDesType> *op = new Operand< MDesType>();
    op->setType( OP_TYPE_IMM);
    op->setConstValue( imm_val);
    
    /* Operand list bookeeping */
    op->attach( first_arg);
    first_arg = op;
    return op;
}

/** Add object result */
template < class MDesType> 
const Operand< MDesType> *
Operation<MDesType>::addResToSet( Object< MDesType> *obj)
{
    /** Check that the operation can actually have result set according to MDes */
    typename MDesType::OperDes *des = MDesType::operDescription( name());
    IR_ASSERTD( des->canHaveResSet() );

    /* Create new operand, set its type and assign object to it */
    Operand< MDesType> *op = new Operand< MDesType>();
    op->setType( OP_TYPE_OBJ);
    op->setObject( obj);
    
    /* Operand list bookeeping */
    op->attach( first_res);
    first_res = op;

    return op;
}

/** Get first argument operand in argument set */
template < class MDesType> 
Operand< MDesType> * 
Operation<MDesType>::firstArgInSet() const
{
    return first_arg;
}
    
/** Get first result in result set */
template < class MDesType> 
Operand< MDesType> * 
Operation<MDesType>::firstResInSet() const
{
    return first_res;
}

/** Get the control flow node of the operation */
template < class MDesType> 
CFNode< MDesType> *
Operation<MDesType>::node() const
{
    return node_;
}

// Set the control flow node of the operation
template < class MDesType> 
void 
Operation<MDesType>::setNode( CFNode< MDesType> *n)
{
    node_ = n;
}

// Check if the operation is last in its CF node
template < class MDesType> 
bool
Operation<MDesType>::isLastInNode() const
{
    if ( isNullP( node()) )
        return false;

    return areEqP( this->node()->lastOper(), this);
}

// Check if the operation is first in its CF node
template < class MDesType> 
bool
Operation<MDesType>::isFirstInNode() const
{
    if ( isNullP( node()) )
        return false;

    return areEqP( this->node()->firstOper(), this);
}

}; /* namespace IR */
