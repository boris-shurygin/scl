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

#ifdef USE_MEM_EVENTS          
        inline MemEventId allocEvent();            /**< Get id of allocation event */
        inline void setAllocEvent( MemEventId id); /**< Set id of allocation event */

        inline MemEventId deallocEvent();            /**< Get id of deallocation event */
        inline void setDeallocEvent( MemEventId id); /**< Set id of deallocation event */
#endif
    private:
        /** Data memory */
        UInt8 data[ size];

#ifdef CHECK_ENTRY        
        /** Debug info: entry status */
        bool is_busy;
#endif

#ifdef USE_MEM_EVENTS        
        /** Debug info: alloc event */
        MemEventId alloc_event;
        /** Debug info: dealloc event */
        MemEventId dealloc_event;
#endif  

        /** Private constructor to prevent direct creation of such objects */
        Entry();
        /** Private destructor */
        ~Entry();
    };

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


#ifdef USE_MEM_EVENTS
    /** Get id of allocation event */
    template< size_t size>
    MemEventId
    Entry< size>:: allocEvent()
    {
        return alloc_event;
    }
    
    /** Set id of allocation event */           
    template< size_t size>
    void
    Entry< size>::setAllocEvent( MemEventId id)
    {
        alloc_event = id;
    }
    /** Get id of deallocation event */
    template< size_t size>
    MemEventId
    Entry< size>::deallocEvent()
    {
        return dealloc_event;
    }          
    /** Set id of deallocation event */ 
    template< size_t size>
    void
    Entry< size>::setDeallocEvent( MemEventId id)
    {
        dealloc_event = id;
    }
#endif
    /**
     * Entry in memory FixedPool that holds user data
     *
     * @ingroup MemImpl
     */
    template < class Data> class FixedEntry: public Entry< sizeof( Data)>
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
    template< class Data>
    FixedEntry< Data>::FixedEntry()
    {
        assert( 0);
    }
    /**
     * Private destructor
     */
    template< class Data>
    FixedEntry< Data>::~FixedEntry()
    {
        assert( 0);
    }
    /** Get position */
    template< class Data>
    ChunkPos 
    FixedEntry< Data>::pos() const
    {
        return my_pos;
    }
    /** Get position of next free chunk */
    template< class Data>
    ChunkPos
    FixedEntry< Data>::nextFree() const
    {
        return next_free_pos;
    }
    /** Set position */
    template< class Data>
    void
    FixedEntry< Data>::setPos( ChunkPos pos)
    {
        my_pos = pos;
    }
    /** Set position of next free chunk */
    template< class Data>
    void
    FixedEntry< Data>::setNextFree( ChunkPos next)
    {
        next_free_pos = next;
    }
};

#endif /* MEM_ENTRY_H */