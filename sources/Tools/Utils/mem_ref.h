/**
 * @file: mem_ref.h 
 * Implementation of memory reference template
 */
/*
 * Utils library in ShowGraphImpl tool
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_REF_H
#define MEM_REF_H

namespace Mem
{
   /**
     * Memory reference template
     * @ingroup Mem
     */
    template < class PtrObj> class Ptr
    {
    private:
        PtrObj *ptr;
    public:
        /** Default constructor */
        Ptr(): ptr( NULL){};
        /** Copy constructor !!! NO uTEST !!! */
        Ptr( const Ptr< PtrObj>& orig): ptr( orig.ptr)
        {
#ifdef USE_REF_COUNTERS
            ptr->incRefCount();
#endif
        }
        /** Assignement operator overloading */
        Ptr< PtrObj> & operator=( const Ptr< PtrObj>& orig)
        {
            ptr = orig.ptr;
#ifdef USE_REF_COUNTERS
            ptr->incRefCount();
#endif
            return *this;
        }
        /**
         * Constructor from pointer
         * Used in Ptr ref = new Obj(...) initialization
         * !!! No uTEST !!!
         */
        Ptr( PtrObj* p): ptr( p)
        {
#ifdef USE_REF_COUNTERS
            if ( ptr != 0)
                ptr->incRefCount();
#endif    
        }
        /**
         * Assignement of pointer
         * Used in Ptr ref; ref = new Obj(...) expression
         */
        Ptr< PtrObj> & operator=( PtrObj* p)
        {
#ifdef USE_REF_COUNTERS
            /** Decrement object's ref count */
            if ( ptr != 0)
                ptr->decRefCount();
#endif    
            /** Assign a new pointer */
            ptr = p;
#ifdef USE_REF_COUNTERS
            /** Increment ref count */
            if ( ptr != 0)
                ptr->incRefCount();
#endif    
            return *this;
        }
        /** Destructor */
        ~Ptr()
        {
#ifdef USE_REF_COUNTERS
            if ( ptr != 0)
                ptr->decRefCount();
#endif
        }
        /** Member access operator */
        inline PtrObj* operator->()
        {
            return ptr;
        }
        /** Equals operator */
        inline bool operator == ( Ptr< PtrObj> &r)
        {
            return ptr == r.ptr;
        }
        /** Conversion to boolean */
        inline operator bool()
        {
            return ptr != NULL;
        }
        /**
         * Conversion to Pointer
         * For using ref as delete operator argument.
         * Use in other expressions is prohibited. Unfortunatelly this is not ( can't be?) enforced.
         */
        inline operator PtrObj*()
        {
                        PtrObj *ret_val = ptr;
            assertd( ret_val != NULL);
            /** Decrement object's ref count */
            if ( ptr != 0)
            {
#ifdef USE_REF_COUNTERS
                ptr->decRefCount();    
#endif
                ptr = 0;
            }
            return ret_val;
        }
    };
};
#endif /* MEM_REF_H */