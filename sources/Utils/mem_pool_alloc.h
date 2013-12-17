/**
 * @file: mem_pool_alloc.h 
 * Implementation of adapter of pools for using them as STL-like allocator
 */
/*
 * Utils library in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef MEM_POOL_ALLOC_H
#define MEM_POOL_ALLOC_H

namespace Mem
{

/**
 * Allocator that uses TypedPool for allocation logic implementation
 */
template <class T>
class PoolAllocator
{
public:
    // type definitions
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    /**
     * rebind allocator to type U
     */
    template <class U>
    struct rebind
    {
        typedef PoolAllocator<U> other;
    };

    /**
     * Get the address of value
     */
    pointer address( reference value) const
    {
        return &value;
    }
    /**
     * Get the constant pointer to the value
     */
    const_pointer address( const_reference value) const
    {
        return &value;
    }

    PoolAllocator() throw() {}                     //< Constructor. Empty as the allocator has no state
    PoolAllocator(const PoolAllocator&) throw() {} //< Constructor. Empty as the allocator has no state
    template <class U>  PoolAllocator (const PoolAllocator<U>&) throw() {}
    ~PoolAllocator() throw() {}

    /**
     * Return maximum number of elements that can be allocated( on Linux) and the maximum size of the container on Windows
     */
    size_type max_size() const throw() 
    {
#if defined ( OS_LINUX)
        return MemImpl::MAX_CHUNK_ENTRIES_NUM;
#elif defined ( OS_WIN)
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
#endif
    }

    // allocate but don't initialize num elements of type T
    pointer allocate (size_type num, const void* = 0) 
    {
        return (pointer) pool().allocate( sizeof(T), num);
    }

    void construct (pointer p, const T& value)
    {
        // use placement new to call constructor
        new((void*)p)T(value);
    }

    void destroy (pointer p)
    {
        // destroy objects by calling their destructor
        p->~T();
    }

    // deallocate storage p of deleted elements
    void deallocate (pointer p, size_type num) {
        pool().deallocate( p);
    }
private:
    //Automatically handled pool singleton
    TypedPool<T> &pool()
    {
       return AutoSingle< TypedPool<T> >::instance(); 
    }
};

/**
 * Two instantiations of Pool allocator are interchangeable
 */
template <class T1, class T2>
bool operator== ( const PoolAllocator<T1>&,
                  const PoolAllocator<T2>&) throw()
{
    return true;
}

template <class T1, class T2>
bool operator!= (const PoolAllocator<T1>&,
                const PoolAllocator<T2>&) throw()
{
    return false;
}

};

#endif