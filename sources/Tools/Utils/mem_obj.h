/**
 * @file: mem_obj.h 
 * Implementation of memory object base class
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_OBJ_H
#define MEM_OBJ_H

/**
 * Namespace for memory manager
 * @ingroup Mem
 */
namespace Mem
{
    /**
     * Base class for all objects allocated in pools
     *
     * @ingroup Mem
     */    
    template < class ClientType, template < class T> class AllocationPolicy = UseDefaultFixedPool>
    class PoolObj: public AllocationPolicy< ClientType>
    {
#ifdef CHECK_DELETE
        bool to_be_deleted;
#endif  
        public:
#ifdef CHECK_DELETE
        /** Default constructor */
        PoolObj(): to_be_deleted( false){};

        /** Mark for deletion */
        inline void toBeDeleted()
        {
            MEM_ASSERTD( !to_be_deleted, "Tried to mark object for deletion more than once");
            to_be_deleted = true;
        }
#else
        /** Default constructor */
        inline PoolObj(){};
#endif
        /** Destructor is to be called by 'destroy' routine of pool class */
        virtual ~PoolObj()
        {
#ifdef CHECK_DELETE
            MEM_ASSERTD( to_be_deleted, "Deleted pool object not through pool interface. Probably operator delete used.");
#endif        
        }
    };
};

#endif /** MEM_OBJ_H */