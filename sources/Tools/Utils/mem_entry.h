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
        assert( 0);
    }
    /**
     * Private destructor
     */
    template< size_t size>
    Entry< size>::~Entry()
    {
        assert( 0);
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
    
    /**
     * Entry in memory that holds user data
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