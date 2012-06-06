/**
 * @file: mem_entry.h 
 * Implementation of memory entry
 */
/*
 * Utils/Mem library in Showgraph tool
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_ENTRY_H
#define MEM_ENTRY_H

namespace MemImpl
{
    /**
     * Debug info for a memory entry
     *
     * @ingroup MemImpl
     */
    class DebugInfo
    {
               
#ifdef MEM_CHECK_POOL                
        inline Pool* pool() const;      /**< Get pointer to pool */
        inline void setPool( Pool* pl); /**< Set pointer to pool */
#endif        

#ifdef USE_REF_COUNTERS
        inline RefNumber refCount() const; /**< Get the number of references */
        inline void incRefCount();         /**< Increase reference count */
        inline void decRefCount();         /**< Decrease reference count */
#endif

#ifdef USE_MEM_EVENTS          
        inline MemEventId allocEvent();            /**< Get id of allocation event */
        inline void setAllocEvent( MemEventId id); /**< Set id of allocation event */

        inline MemEventId deallocEvent();            /**< Get id of deallocation event */
        inline void setDeallocEvent( MemEventId id); /**< Set id of deallocation event */
#endif

    private:
#ifdef MEM_CHECK_POOL
        /** Pointer to pool */
        Pool *_pool;
#endif

#ifdef USE_REF_COUNTERS
        /** Counter for references */
        RefNumber ref_count; /* Additional memory used, the overhead of counted references */
#endif

#ifdef USE_MEM_EVENTS        
        /** Debug info: alloc event */
        MemEventId alloc_event;
        /** Debug info: dealloc event */
        MemEventId dealloc_event;
#endif 
    };

#ifdef USE_REF_COUNTERS
    /** Get the number of references */
    inline RefNumber DebugInfo::refCount() const
    {
        return ref_count;
    }
    /** Increase reference count */
    inline void DebugInfo::incRefCount()
    {
        ref_count++;
    }
    /** Decrease reference count */
    inline void DebugInfo::decRefCount()
    {
        assertd( ref_count > 0);
        ref_count--;
    }
#endif
    
#ifdef MEM_CHECK_POOL
    /** Get pointer to pool */
    inline Pool* DebugInfo::pool() const
    {
        return _pool;
    }
    /** Set pointer to pool */
    inline void DebugInfo::setPool( Pool* pl)
    {
        _pool = pl;
    }
#endif

#ifdef USE_MEM_EVENTS
    /** Get id of allocation event */
    MemEventId
    DebugInfo::allocEvent()
    {
        return alloc_event;
    }
    
    /** Set id of allocation event */           
    void
    DebugInfo::setAllocEvent( MemEventId id)
    {
        alloc_event = id;
    }
    /** Get id of deallocation event */
    MemEventId
    DebugInfo::deallocEvent()
    {
        return dealloc_event;
    }          
    /** Set id of deallocation event */ 
    void
    DebugInfo::setDeallocEvent( MemEventId id)
    {
        dealloc_event = id;
    }
#endif
    /**
     * Entry in memory that holds user data
     *
     * @ingroup MemImpl
     */
    template < size_t size> class Entry
    {
    public:
        /** Get pointer to data */
        inline void *dataMem();

#ifdef CHECK_ENTRY   
        inline bool isBusy();                   /**< Check if entry has busy flag */
        inline void setBusy( bool busy = true); /**< Set busy flag */
#endif
#ifdef USE_DEBUG_INFO
        inline DebugInfo& debugInfo(); /**< Get debug info pointer */
#endif
    private:
        
#ifdef USE_DEBUG_INFO
        /** Debug info */
        DebugInfo debug_info;

        friend DebugInfo *getDebugInfo( void *ptr);
#endif        
        /** Data memory */
        UInt8 data[ size];

#ifdef CHECK_ENTRY        
        /** Debug info: entry status */
        bool is_busy;
#endif

        /** Private constructor to prevent direct creation of such objects */
        Entry();
        /** Private destructor */
        ~Entry();
    };

    /** Routine for getting pointer to debug info by pointer to data */
    inline DebugInfo *getDebugInfo( void *ptr);
    {
        MEM_ASSERTD( isNotNullP( ptr), "Data pointer can't be null");
        MEM_ASSERTD( offsetof( Entry<8>, data) == offsetof( Entry<63>, data),
                     "Debug info offsets should be equal for every possible entry size");

        size_t offset = offsetof( Entry<8>, data);
        return (DebugInfo *)(ptr - offset);
    }

    /**
     * Private constructor to prevent direct creation of such objects
     */
    template< size_t size>
    Entry< size>::Entry()
    {
        MEM_ASSERTD( 0, "Entry constructor called");
    }
    /**
     * Private destructor
     */
    template< size_t size>
    Entry< size>::~Entry()
    {
        MEM_ASSERTD( 0, "Entry destructor called");
    }

    /**
     * Private destructor
     */
    template< size_t size>
    void *
    Entry< size>::dataMem()
    {
        return &data;
    }

#ifdef CHECK_ENTRY
    /** Check if entry has busy flag */
    template< size_t size>
    bool
    Entry< size>::isBusy()
    {
        return is_busy;
    }
    
    /** Set busy flag */
    template< size_t size>
    void
    Entry< size>::setBusy( bool busy = true)
    {
        is_busy = busy;
    }
#endif

#ifdef USE_DEBUG_INFO
    /**< Get debug info pointer*/
    template< size_t size>
    DebugInfo&
    Entry< size>::debugInfo()
    {
        return debug_info;
    }
#endif

    /**
     * Entry in memory FixedPool that holds user data
     *
     * @ingroup MemImpl
     */
    template < size_t size> class FixedEntry: public Entry< size>
    {
    public:
        /** Get position */
        inline ChunkPos pos() const;
        /** Get position of next free chunk */
        inline ChunkPos nextFree() const;
        /** Set position */
        inline void setPos( ChunkPos pos);
        /** Set position of next free chunk */
        inline void setNextFree( ChunkPos next);
    private:
        /** Classes fixed pool and chunk should have access to data and constructors */
        friend class Chunk<Data>;

        /** Own position of this entry in chunk */
        ChunkPos my_pos;
        /** Position of next free entry in chunk */
        ChunkPos next_free_pos;

        /** Private constructor to prevent direct creation of such objects */
        FixedEntry();
        /** Private destructor */
        ~FixedEntry();
    };

    /**
     * Private constructor to prevent direct creation of such objects
     */
    template< size_t size>
    FixedEntry< size_t size>::FixedEntry()
    {
        MEM_ASSERTD( 0, "Constructor of fixed entry shouldn't be called ever");
    }
    /**
     * Private destructor
     */
    template< size_t size>
    FixedEntry< size_t size>::~FixedEntry()
    {
        MEM_ASSERTD( 0, "Destructor of fixed entry shouldn't be called ever");
    }

    /** Get position */
    template< size_t size>
    ChunkPos 
    FixedEntry< size_t size>::pos() const
    {
        return my_pos;
    }
    /** Get position of next free chunk */
    template< size_t size>
    ChunkPos
    FixedEntry< size_t size>::nextFree() const
    {
        return next_free_pos;
    }
    /** Set position */
    template< size_t size>
    void
    FixedEntry< size_t size>::setPos( ChunkPos pos)
    {
        my_pos = pos;
    }
    /** Set position of next free chunk */
    template< size_t size>
    void
    FixedEntry< size_t size>::setNextFree( ChunkPos next)
    {
        next_free_pos = next;
    }
};

#endif /* MEM_ENTRY_H */