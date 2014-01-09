/**
 * @file: ir/cf.hpp
 * Compile-time constant representation
 * @ingroup IR
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace IR
{
    /** Types of constant */
    enum ConstType
    {
        CONST_TYPE_INTEGER,/**< Integer constant         */
        CONST_TYPE_FLOAT,  /**< Floating point constant  */
        CONST_TYPE_SYMBOL, /**< Symbolic constant        */
        CONST_TYPES_NUM    /**< Number of constant types */
    };

    /**
     * Constant representation
     * @ingroup IR
     */
    class Const
    {
    public:
        inline Const( Int64 int_val); /**< Constructor of integer constant        */
        inline Const( Double f_val);  /**< Constructor of floating point constant */
        inline Const( string str);    /**< Constructor of symbolic constant       */
        
        inline ConstType type()      const; /**< Get type of constant */

        inline Int64     intVal()    const; /**< Get integer constant value        */
        inline Double    floatVal()  const; /**< Get floating point constant value */
        inline string    strVal()    const; /**< Get integer constant value        */


        /** Print constant to stream */
        inline void toStream(ostream& os) const;
    private:
        /** Pod data representation */
        union ConstData
        {
            Int64 int_val;
            Double d_val;
        } data;
        
        std::string str_val;/**< Placeholder for string data */
        
        ConstType type_; /**< Type of the constant */
    };
 
    /** Constructor of integer constant */
    Const::Const( Int64 int_val): type_( CONST_TYPE_INTEGER)
    {
        data.int_val = int_val;
    }

    // Constructor of floating point constant
    Const::Const( Double f_val): type_( CONST_TYPE_FLOAT)
    {    
        data.d_val = f_val;
    }  

    // Constructor of symbolic constant
    Const::Const( string str): type_( CONST_TYPE_SYMBOL)
    {
        str_val = str;
    }    

    // Get type of constant
    inline ConstType Const::type() const
    {
        return type_;
    } 

    // Get integer constant value
    inline Int64 Const::intVal() const
    {
        IR_ASSERTD( type() == CONST_TYPE_INTEGER);
        return data.int_val;
    } 
    
    // Get floating point constant value
    inline Double Const::floatVal() const
    {
        IR_ASSERTD( type() == CONST_TYPE_FLOAT);
        return data.d_val;
    } 
    
    // Get integer constant value
    inline string Const::strVal() const
    {
        IR_ASSERTD( type() == CONST_TYPE_SYMBOL);
        return str_val;
    } 
    /** Print operand to stream */
    inline void Const::toStream(ostream& os) const
    {
        switch ( type())
        {
    case CONST_TYPE_INTEGER:    
                os << intVal();
                break;
        case CONST_TYPE_FLOAT:
                os << floatVal();
                break;
    case CONST_TYPE_SYMBOL:
                os << strVal();
                break;
            default:
                IR_ASSERTD( 0);
        }
    }

    inline 
    std::ostream& operator<<(std::ostream& os, const Const& c) 
    {   
        c.toStream(os);
        return os;
    } 

};

