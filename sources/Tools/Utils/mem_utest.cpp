/**
 * @file: mem_utest.cpp 
 * Implementation of testing of memory manager
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"
#include "mem.h"

using namespace Mem;

/** Test object class */
class TestObj: public Obj
{
	
public:	
	/** Some variable */
	int a;
};

/**
 * Reference to object
 */
typedef Ref< TestObj> ObjRef;

/**
 * Test smart pointers
 */
static bool
uTestRef()
{
	/** Test smart pointers behaviour */
    ObjRef ref = new TestObj(); /** Test constructor from pointer */
    ObjRef ref2; /** Test default constructor */

    /** Test operator bool() */
    ASSERT( !ref2 && ref);
    ASSERT( ref2 == NULL && ref != NULL);
    /** Test copy constructor */
    ref2 = ref;
    ASSERT( ref2 && ref);
    /** Test operator == ( ref) */
    ASSERT( ref == ref2);

    /** Test operator -> */
    ref->a = 2;
#ifdef USE_REF_COUNTERS
    ASSERT_X( ref->refCount() == 2, "reference utest", "incorrect reference counter");
#endif	
    bool catched = false;

    /** Test exception generation */
    try
    {
            delete ref;
    } catch( int a)
    {
            catched = true;
            ref2->a = a;
    }
    ASSERT_X( catched, "reference utest", "Exception is not catched");

    /** Test ref to pointer conversion and Obj destructor */
    delete ref2;
    return true;
}

/** Sample object used as a baseclass for more complicated pool-stored objects */
class PoolBase: public PoolObj 
{
    virtual void setVal( UInt32 val) = 0;
    virtual UInt32 val() const = 0;
};

/**
 * @brief Fairly complex pool-stored object
 * Class for testing complex objects stored in pool. Built with 
 * multiple inheritance, virtual functions and additional members.
 */
class MyPoolObj: public PoolBase, public SListIface< MyPoolObj, SListItem>
{
    UInt32 priv_field;
public:
    /** Some public fields */
    UInt32 a;
    UInt32 b;
    bool *called;
    /** Base class routines implementation */
    void setVal( UInt32 val)
    {
        priv_field = val;
    }
    UInt32 val() const
    {
        return priv_field;
    }
    ~MyPoolObj()
    {
        *called = true;
    }
};
/**
 * Test memory pools
 */
static bool
uTestPools()
{

    Pool *pool = new FixedPool< MyPoolObj>();
    MyPoolObj *p1 = new ( pool) MyPoolObj();
    MyPoolObj *p2 = new ( pool) MyPoolObj();
    bool called_destructor1 = false;
    bool called_destructor2 = false;

    ASSERT( p1 != p2);
    p1->a = 1;
    p2->a = 2;
    p1->b = 3;
    p2->b = 4;
    p1->called = &called_destructor1;
    p2->called = &called_destructor2;

    p1->setVal( 5);
    p2->setVal( 6);
    ASSERT( p1->a != p1->b);
    ASSERT( p1->a != p2->a);
    ASSERT( p1->b != p2->a);
    ASSERT( p1->b != p2->b);
    ASSERT( p1->a != p2->a);
    
    ASSERT( !called_destructor1);
    ASSERT( !called_destructor2);
    
    pool->destroy( p1);
    
    ASSERT( called_destructor1);
    ASSERT( !called_destructor2);

    pool->destroy( p2);
    
    ASSERT( called_destructor1);
    ASSERT( called_destructor2);

    /** More objects */
    MyPoolObj *obj = NULL;
    for ( int i = 0; i < 20000; i++)
    {
        MyPoolObj *prev_obj = obj;
        obj = new ( pool) MyPoolObj();
        obj->called = &called_destructor1;
        obj->attach( prev_obj);
        prev_obj = obj->next();
    }
    while ( isNotNullP( obj))
    {
        MyPoolObj *next = obj->next();
        pool->destroy( obj);
        obj = next;
    }
    delete pool;
    return true;
}


/**
 * Test smart pointers, objects and pools
 */
bool uTestMem()
{
    /** Test smart pointers */
    if ( !uTestRef())
        return false;
    /** Test memory pools */
    if ( !uTestPools())
        return false;
    return true;
}
