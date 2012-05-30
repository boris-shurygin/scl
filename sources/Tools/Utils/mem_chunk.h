/**
 * @file: mem_chunk.h 
 * Implementation of memory chunk
 */
/*
 * Utils/Mem library in Showgraph tool
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_CHUNK_H
#define MEM_CHUNK_H

namespace MemImpl
{
    /**
     * Chunk lists identificators
     * @ingroup MemImpl
     */
    enum ChunkListType
    {
        CHUNK_LIST_ALL,
        CHUNK_LIST_FREE,
        CHUNK_LISTS_NUM
    };

    /**
     * Memory chunk representation
     * @ingroup MemImpl
     */
    template<class Data> class Chunk: 
        public MListIface< Chunk< Data>, // List client data
                           MListItem< CHUNK_LISTS_NUM>, // base class: pure multi-list item
                           CHUNK_LISTS_NUM > // Lists number
    {
        void *dummy_ptr; //for alignment
        /** position of first free entry */
        ChunkPos free_entry;
        /** busy entries num */
        ChunkPos busy;
        /** Get chunk for given number */
        inline FixedEntry<Data> *entry( ChunkPos pos);

    public:
#ifdef CHECK_CHUNKS
        void *pool;       
        /** Get chunk's first busy entry */
        inline FixedEntry<Data> *firstBusyEntry();
#endif
        /** Constructor */
        inline Chunk();
        /** Check if this chunk has free entries */
        inline bool isFree() const;
        /** Check if this chunk is empty */
        inline bool isEmpty() const;
        /** Initialization of chunk */
        inline void initialize();
        /** Allocate on entry */
        inline Data *allocateEntry();
        /** Deallocate one entry */
        inline void deallocateEntry( FixedEntry<Data> *e);
        /** For some reason GCC asks for it :( */
        inline void operator delete( void* mem);
        /** Placement new */
        inline void *operator new ( size_t size, void* mem);
        /**
         * Operator 'delete' corresponding to placement new
         * WARNING: Compiler won't call this for deletion. 
         *          It is needed for freeing memory in case of exceptions in constructor
         */
        inline void operator delete( void* ptr, void* mem);
    };
    
    /** Constructor */
    template <class Data> 
    Chunk< Data>::Chunk()
    {
        FixedEntry<Data> *e = NULL;
        
        for ( int i = 0; i < MAX_CHUNK_ENTRIES_NUM; i++)
        {
            e = ( FixedEntry< Data> *)( (UInt8 *) this 
                                       + sizeof( Chunk< Data>) 
                                       + sizeof( FixedEntry< Data>) * i);
            /** Initialization of every entry */
            e->setPos( i);
            e->setNextFree( i + 1);
#ifdef CHECK_ENTRY
            e->setBusy( false);
#endif
#ifdef USE_MEM_EVENTS        
            e->setAllocEvent( 0);
            e->setDeallocEvent( 0);
#endif 
        }
        MEM_ASSERTD( e->nextFree() == UNDEF_POS, "Chunk size constant and undefined value do not match");
        free_entry = 0;
        busy = 0;
    }
    /** Placement new */
    template <class Data> 
    void*
    Chunk< Data>::operator new ( size_t size, void* mem)
    {
        return mem;
    }
    
    /** For some reason GCC asks for it :( */
    template <class Data>
    void
    Chunk< Data>::operator delete( void* mem)
    {

    }
    /**
     * Operator 'delete' corresponding to placement new
     * WARNING: Compiler won't call this for deletion. 
     *          It is needed for freeing memory in case of exceptions in constructor
     */
    template <class Data> 
    void
    Chunk< Data>::operator delete( void* ptr, void* mem)
    {
    
    }
  
    /** Get entry by number */
    template<class Data> 
    FixedEntry< Data>*
    Chunk< Data>::entry( ChunkPos pos)
    {
        MEM_ASSERTD( pos != UNDEF_POS, "Requested entry with undefined number");
        return ( FixedEntry< Data> *)( (UInt8 *) this 
                                  + sizeof( Chunk< Data>) 
                                  + sizeof( FixedEntry< Data>) * pos);
    }
   
#ifdef CHECK_CHUNKS        
    /** Get chunk's first busy entry */
    template<class Data> 
    FixedEntry< Data>*
    Chunk< Data>::firstBusyEntry()
    {
        FixedEntry<Data> *e = NULL;
        
        for ( int i = 0; i < MAX_CHUNK_ENTRIES_NUM; i++)
        {
            e = ( FixedEntry< Data> *)( (UInt8 *) this 
                                       + sizeof( Chunk< Data>) 
                                       + sizeof( FixedEntry< Data>) * i);
            if ( e->isBusy())
                return e;
        }
        return NULL;
    }
#endif

    /** Check if this chunk has free entries */
    template<class Data> 
    bool 
    Chunk< Data>::isFree() const
    {
        return free_entry != UNDEF_POS;
    }
    /** Check if this chunk is empty */
    template<class Data> 
    bool 
    Chunk< Data>::isEmpty() const
    {
        return busy == 0;
    }      
    /** Allocate one entry */
    template<class Data> 
    Data*
    Chunk< Data>::allocateEntry()
    {
        MEM_ASSERTD( this->isFree(), "Trying to allocated entry in a full chunk");
        
        FixedEntry< Data> *e = entry( free_entry);
#ifdef CHECK_ENTRY
        e->setBusy( true);
#endif
#ifdef USE_MEM_EVENTS        
        e->setAllocEvent( Mem::MemMgr::instance()->allocEvent());
#endif        
        Data *res = static_cast<Data *>( e->dataMem());
        free_entry = e->nextFree();
        busy++;
        return res;
    }
    /** Deallocate one entry */
    template<class Data> 
    void
    Chunk< Data>::deallocateEntry( FixedEntry<Data> *e)
    {
        MEM_ASSERTD( busy > 0, "Trying to deallocate entry of an empty chunk");
#ifdef CHECK_ENTRY
        MEM_ASSERTD( e->isBusy(), 
                     "Trying to deallocate entry that is free. Check deallocation event ID");
        e->setBusy( false);
#endif
#ifdef USE_MEM_EVENTS        
        e->setDeallocEvent( Mem::MemMgr::instance()->deallocEvent());
#endif 
        e->setNextFree( free_entry);
        free_entry = e->pos();
        busy--;
    }
}; /* namespace MemImpl */
#endif /* MEM_CHUNK_H */