/**
 * @file: mem_generic_pool.h 
 * Implementation of generic memory pool with variable entry size
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_FIXED_POOL_H
#define MEM_FIXED_POOL_H

namespace Mem
{
    /**
     * @brief Memory pool with arbitrary-sized entries.
     * @ingroup Mem
     *
     * @details  
     * A <em>Generic pool</em> is a pool that allocates entries of the arbitrary size 
     */
    class GenericPool: public Pool
    {
    public:
        
        GenericPool(); /**< Create pool with default parameters */
        ~GenericPool();/**< Destroy the pool */
                
        void* allocate( size_t size); /**< Allocate new memory block */
        void deallocate( void *ptr);  /**< Free memory block */
        void destroy( void *ptr);     /**< Calls destructor of pooled object and frees memory */
    private:        
        /** Number of used entries */
        EntryNum entry_count;

        /* Internal routines */
    };

    /** Create pool with default parameters */
    GenericPool::GenericPool(): 
        entry_count( 0),
    {

    }

    /** Destroy the pool */
    GenericPool::~GenericPool()
    {
        /** Check that all entries are freed */
        MEM_ASSERTD( entry_count == 0, "Trying to delete non-empty pool");
    }

    /* Allocate new memory block */
    void* 
    GenericPool::allocate( size_t size)
    {
        void *ptr = malloc( size);
        entry_count++;
        return ptr;
    }

    /** Free memory block */
    template < class Data> 
    void
    GenericPool::deallocate( void *ptr)
    {
        /* 1. Check pointer */
        MEM_ASSERTD( isNotNullP( ptr), "Deallocation tried on NULL pointer");
        
        /* 2. Check entry count */
        MEM_ASSERTD( entry_count > 0, "Trying deallocate entry of an empty pool"); 

        free( ptr);
        entry_count--;
    }

    /** Functionality of 'operator delete' for pooled objects */
    void
    GenericPool::destroy( void *ptr)
    {
        /* 1. Check null pointer( in DEBUG mode) */
        MEM_ASSERTD( isNotNullP( ptr), "Destruction tried on NULL pointer");

        Data *data_p = static_cast< Data *>( ptr);
        
#ifdef CHECK_DELETE
        /* 2. Mark for deletion */
        data_p->toBeDeleted();
#endif
        /* 3. Call destructor */
        data_p->~Data();
        
        /* 4. Free memory */
        deallocate( ptr);
    }
}; /* namespace Mem */
#endif /* MEM_GENERIC_POOL_H */