/**
 * @file: mem_alloc_policy.h 
 * Implementation of memory allocatin policy classes
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_ALLOC_POLICY_H
#define MEM_ALLOC_POLICY_H

/**
 * Namespace for memory manager
 * @ingroup Mem
 */
namespace Mem
{
    /**
     * Base class of alloction policy. Mostly everything is disabled.
     */
    class ObjAllocPolicy
    {
        /** Copy constructor disabled*/
        ObjAllocPolicy( ObjAllocPolicy &obj){};
        /** Assignment disabled */
        ObjAllocPolicy& operator = ( const ObjAllocPolicy& obj){ return *this;};
        /** Default operator 'new' is disabled */
        void *operator new ( size_t size){};
        /** Default operator 'delete' is disabled */
        void operator delete( void *ptr){};
        
        /** Default operator 'new' is disabled */
        void *operator new[] ( size_t size){};
        /** Default operator 'delete' is disabled */
        void operator delete[] ( void *ptr){};
        
        /** Placement new */
        inline void *operator new ( size_t size, Pool* pool){};
        /**
         * Operator 'delete' corresponding to placement new
         * WARNING: Compiler won't call this for deletion. 
         *          It is needed for freeing memory in case of exceptions in constructor
         */
        inline void operator delete( void *ptr, Pool* pool){};
    };


    /**
     * Object allocation policy that uses default fixed pool to allocate memory
     */
    template < size_t my_size> class UseDefaultFixedPool: public ObjAllocPolicy
    {
        public:
        /** Default operator 'new' is disabled */
        void *operator new( size_t size)
        {
             MEM_ASSERTD( my_size == size, "Size passed as new parameter must be equal to the one that"
                                           "was selected for the object with specialization of PoolObj");
             return DefaultPool<my_size>::ptr()->allocate( size);
        }
        /** Default operator 'delete' is disabled */
        void operator delete( void *ptr, size_t size)
        {
            MEM_ASSERTD( my_size == size, "Size passed as new parameter must be equal to the one that"
                                          "was selected for the object with specialization of PoolObj");
            DefaultPool<my_size>::ptr()->deallocate( ptr);
        }
    };
       
    /**
     * Object allocation policy that uses default generic pool to allocate memory, my_size parameter is ignored
     */
    template < size_t my_size> class UseGenericPool: public ObjAllocPolicy
    {
        public:
        /** Default operator 'new' is disabled */
        void *operator new( size_t size)
        {
             return DefaultGenericPool::ptr()->allocate( size);
        }
        /** Default operator 'delete' is disabled */
        void operator delete( void *ptr, size_t size)
        {
            DefaultGenericPool::ptr()->deallocate( ptr);
        }
    };

    /**
     * Object allocation policy that defines placement new/delete to operate on user-defined pool
     */
    template < size_t my_size> class UseCustomFixedPool: public ObjAllocPolicy
    {
        public:
        /**
         * Placement new
         */
        inline void *
        operator new ( size_t size, FixedPool< my_size>* pool)
        {
            return pool->allocate( size);
        }
        /**
         * Operator 'delete' corresponding to placement new
         * WARNING: Compiler won't call this for deletion. 
         *          It is needed for freeing memory in case of exceptions in constructor
         */
        inline void
        operator delete( void *ptr, FixedPool< my_size>* pool)
        {
            pool->deallocate( ptr);
        }
    };
};

#endif /* MEM_ALLOC_POLICY_H */
