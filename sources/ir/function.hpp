/**
 * @file:ir/cf.hpp
 * Function description
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace IR
{

/**
 * @brief Representation of function
 * @ingroup IR
 */
template <class MDesType> class Function: 
    public DG< MDesType>,
    public SListIface<Function <MDesType> >
{
public:
    Function( Module<MDesType> *m);                   /**< Create function in given module */
    Function( Module<MDesType> *m, string func_name); /**< Create named function in given module */
    
    virtual ~Function();  /**< Delete function */

    inline Module<MDesType> *module() const; /**< Get the module of the function */
    inline CFG<MDesType> &cfg() ;            /**< Get control flow graph of the function */
        
    /** Create new operation */
    inline Operation< MDesType> * newOper( typename MDesType::OperName name);
        
    /** Create new operaiton and insert it into sequence */
    inline Operation< MDesType> * newOperAfter( typename MDesType::OperName name, Operation< MDesType> *prev_oper); 
        
    /** Create new object */
    inline Object< MDesType> * newObject( typename MDesType::ObjName name);
    
    /** Create new object */
    inline Object< MDesType> * newArg( UInt16 arg_num, typename MDesType::ObjName name);

    /** Get function's first operation in sequence */
    inline Operation< MDesType> * firstOper() const;

    /** Set function's first operation in sequence */
    inline void setFirstOper( Operation< MDesType> * oper);

    /** Print function to stream */
    inline void toStream(ostream& os);

    /** Get the function name */
    inline string name() const;

    /** Get the function name */
    inline void setName( string new_name);
private:
    Operation< MDesType> *newNode();   /**< Create new dep graph node (operation without a name) */
    Module< MDesType> *module_p;       /**< Pointer to the module */
    list< Object< MDesType> *> objects;/**< List of function's objects */
    CFG< MDesType> cfg_;               /**< Control flow graph */
        
    /** First operation in linear sequence of operaitons in this function */
    Operation< MDesType> *first_oper_in_seq;
    /** Next object id */
    UInt32 obj_id[ MDesType::num_objs];
    /** Name of the function */
    string name_;
    /** Arguments */
    vector< Object< MDesType> *> args;
};

/** Get the function name */
template <class MDesType> string Function< MDesType>::name() const
{
    return name_;
}

/** Get the function name */
template <class MDesType> void Function< MDesType>::setName( string new_name)
{
    name_ = new_name;
}
    
/** Get function's first operation in sequence */
template <class MDesType> Operation< MDesType> *  Function< MDesType>::firstOper() const
{
    return first_oper_in_seq;
}

/** Set function's first operation in sequence */
template <class MDesType> void Function< MDesType>::setFirstOper( Operation< MDesType> * oper)
{
    first_oper_in_seq = oper;
}

/** 
 * Create function in given module
 */
template <class MDesType> Function< MDesType>::Function( Module<MDesType> *m): module_p( m) 
{
    for ( int i = 0; i < MDesType::num_objs; ++i)
    {
        obj_id[ i] = 0;
    }
}

/** 
 * Create named function in given module
 */
template <class MDesType> Function< MDesType>::Function( Module<MDesType> *m, string func_name):
    module_p( m), name_( func_name) 
{
    for ( int i = 0; i < MDesType::num_objs; ++i)
    {
        obj_id[ i] = 0;
    }
}

/** 
 * Function destructor
 */
template <class MDesType> 
Function< MDesType>::~Function()
{
    for ( typename list< Object< MDesType> *>::iterator it = objects.begin(), end =  objects.end();
            it != end;
            ++it)
    {
        delete *it;
    }
}

/** Get the module of the function */
template <class MDesType> Module< MDesType> *Function<MDesType>::module() const
{
    return module_p;
}
/** Get control flow graph of the function */
template <class MDesType> CFG<MDesType> &
Function< MDesType>::cfg()
{
    return cfg_;
}

template <class MDesType>
Operation< MDesType> * 
Function< MDesType>::newNode()
{
    return DG< MDesType>::newNode();
}

template <class MDesType>
Operation< MDesType> * 
Function< MDesType>::newOper( typename MDesType::OperName name)
{
    IR_ASSERTD( MDesType::checkName( name) );
    Operation< MDesType> * oper = newNode();
    oper->setName( name);
    return oper;
}
template <class MDesType>
Operation< MDesType> * 
Function< MDesType>::newOperAfter( typename MDesType::OperName name, Operation< MDesType> *prev_oper)
{
    IR_ASSERTD( MDesType::checkName( name) );
    Operation< MDesType> * oper = newNode();
    oper->setName( name);
    oper->insertAfter( prev_oper);
    return oper;
}

template <class MDesType>
Object< MDesType> * 
Function< MDesType>::newObject( typename MDesType::ObjName name)
{
    Object< MDesType> * obj = new Object< MDesType>();
    obj->setType( name);
    obj->setId( obj_id[ name]++);
    objects.push_back( obj);
    return obj;
}

template <class MDesType>
Object< MDesType> * 
Function< MDesType>::newArg( UInt16 arg_num, typename MDesType::ObjName name)
{
    Object< MDesType> * obj = newObject( name);
    if ( ( arg_num + 1 ) > args.size() )
    {
        args.resize( arg_num + 1);
    }
    args[ arg_num] = obj;
    return obj;
}

template <class MDesType>
void
Function< MDesType>::toStream(ostream& os)
{
    Marker m = MarkerManager::newMarker();

    /** Print function header */
    os << "Function " << name_;

    /** Print arguments */
    os << "( ";
    for ( typename vector< Object< MDesType> *>::iterator it = args.begin(),
                                                end = args.end();
         it != end; ++it)
    {
        os << *it;
    }

    os << ")" << endl;

    /* If CFG is valid print CFG instead of operation sequence */
    if ( this->cfg().isValid())
    {
        os << this->cfg();
        return;
    }

    /* Find the target operations */
    for ( Operation< MDesType> * oper = DG< MDesType>::firstNode();
            isNotNullP( oper);
            oper = oper->nextNode())
    {
        for ( UInt32 i = 0; i < oper->numArgs(); i ++)
        {
            if ( oper->arg( i).isTarget())
            {
                Operation< MDesType> * target = oper->arg(i).target();
                target->mark( m);
            }
        }
    }

    /* Iterating over operations in functions */
    for ( Operation< MDesType> *oper = firstOper();
            isNotNullP( oper);
            oper = oper->nextOper())
    {
        if ( oper->isMarked( m))
            os << "label_" << oper->id() << ':' << endl;
        os << oper;
        IR_ASSERTD( areEqP( oper->function(), this) );
    }

    this->freeMarker( m);
}

template <class MDesType> 
std::ostream& operator<<(std::ostream& os, const Function<MDesType> &f) 
{   
    f.toStream(os);
    return os;
} 

template <class MDesType> 
std::ostream& operator<<(std::ostream& os, Function<MDesType> *f) 
{   
    f->toStream(os);
    return os;
} 

};
