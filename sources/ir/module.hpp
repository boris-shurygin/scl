/**
 * @file:ir/cf.hpp
 * Declaration of translation unit description class
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace IR
{
    /**
     * @class Module
     * @brief Module class describes one translation unit as a list of functions and global information
     * @ingroup IR
     */
    template <class MDesType> class Module
    {
    public:
        typedef Function<MDesType> Func; 

        inline Module();  //< Constructor of an empty translation unit
        inline ~Module(); //< Destructor
        
        inline Func * newFunction();                   //< Create a new function
        inline Func * newFunction( string name);       //< Create a new named function
        inline Func * findFunctionByName( string name);//< Find function by its name string
        inline void removeFunction( Func *f);          //< Remove function from the module
        
        /** Find function with the given name, rename it and adjust the symbol table */
        inline void renameFunction( string name, string new_name); 
        /** Change function name and adjust the symbol table */
        inline void renameFunction( Func *f, string new_name);

        /** Print module to stream */
        inline void toStream(ostream& os);
    private:
        /** Pointer to the first function */
        Func* first_function;
        /** Functions map */
        map< string, Func *> func_table;
        
        /** Next function id */
        UInt32 f_id;
    };

    /**
     * Constructor of an empty translation unit
     */
    template <class MDesType> Module<MDesType>::Module():
        first_function( NULL), f_id( 0)
    {
    
    }

    /**
     * Module destructor
     */
    template <class MDesType> Module<MDesType>::~Module()
    {
        // Remove function list
        deleteList< Function<MDesType> >( first_function);
    }

    /**
     * Creates a new function and returns a reference to it. A reference to the
     * created function is also saved to the module's list of functions
     */
    template <class MDesType>
    Function<MDesType> *
    Module<MDesType>::newFunction( string name)
    {
        Function<MDesType> *f = new Function<MDesType>( this, name); // Create the new function 
        
        // Function list bookkeeping
        f->attach( first_function);
        first_function = f;
        
        // Symbol table mangement
        func_table[ name] = f;

        return f;
    }

    /**
     * Creates a new function and returns a reference to it. A reference to the
     * created function is also saved to the module's list of functions
     */
    template <class MDesType>
    Function<MDesType> *
    Module<MDesType>::newFunction()
    {
        //Compose a name for new function 
        string name("noname_");
        
        name.append( toString( f_id++));
        return newFunction( name);
    }

    /** Find function by its name string */
    template <class MDesType>
    Function<MDesType> *
    Module<MDesType>::findFunctionByName( string name)
    {
        IR_ASSERTXD( 0, "Not implemented");
        return NULL;
    }

    /** Remove function from the module */
    template <class MDesType>
    void
    Module<MDesType>::removeFunction( Func *f)
    {
        /** Replace first function if needed */
        if ( f == first_function)
        {
            first_function = f->next();
        }
        typename map< string, Func *>::iterator it = func_table.find( f->name);
        if ( it != func_table.end() )
        {
            func_table.erase( it);
        }
        delete f;
    }
        
    /** Find function with the given name, rename it and adjust the symbol table */
    template <class MDesType>
    void
    Module<MDesType>::renameFunction( string name, string new_name)
    {
        IR_ASSERTXD( 0, "Not implemented");
    }
    
    /** Change function name and adjust the symbol table */
    template <class MDesType>
    void
    Module<MDesType>::renameFunction( Func *f, string new_name)
    {
        IR_ASSERTXD( 0, "Not implemented");
    }

    /** Output module to stream */
    template <class MDesType>
    void
    Module<MDesType>::toStream( ostream& os)
    {
        /** Output module functions consequently */
        Func* f = first_function;
        while( f != NULL )
        {
            os << f;
            f = f->next();
        }
    }

    template <class MDesType> 
    std::ostream& operator<<(std::ostream& os, const Module<MDesType> &m) 
    {   
       m.toStream(os);
       return os;
    } 

    template <class MDesType> 
    std::ostream& operator<<(std::ostream& os, Module<MDesType> *m) 
    {   
        m->toStream(os);
        return os;
    } 

};
