/**
 * @file: mem_utest.cpp 
 * Implementation of testing of memory manager
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

using namespace Utils;
using namespace Mem;

/** Test object class */
class TestObj: public PoolObj< TestObj> // Configured to use default fixed pool
{
    
public:    
    /** Some variable */
    int a;
};

/**
 * Reference to object
 */
typedef Ptr< TestObj> ObjPtr;

/**
 * Test smart pointers
 */
static bool uTestRef( UnitTest *utest_p)
{
    DefaultPool< TestObj>::init();

    /** Test smart pointers behaviour */
    ObjPtr ref = new TestObj(); /** Test constructor from pointer */
    ObjPtr ref2; /** Test default constructor */
    ObjPtr ref3; /** Test default constructor */

    /** Test operator bool() */
    UTEST_CHECK( utest_p, !ref2 && ref);
    //UTEST_CHECK( utest_p, ref2 == NULL && ref != NULL);
    /** Test copy constructor */
    ref2 = ref;
    UTEST_CHECK( utest_p, ref2 && ref);
    /** Test operator == ( ref) */
    UTEST_CHECK( utest_p, ref == ref2);

    /** Test operator -> */
    ref->a = 2;

#ifdef USE_REF_COUNTERS
    UTEST_CHECK( utest_p, ref.refCount() == 2);//incorrect reference counter
#endif

    ref2.setNull();
    ref3 = ref;
    
    ref = 0;
    
    UTEST_CHECK( utest_p, isNullP( ref));//ref pointer should be null at this point

    ref3.destroy();
    DefaultPool< TestObj>::deinit();
    return true;
}

/** Sample object used as a baseclass for more complicated pool-stored objects */
class PoolBase
{
    virtual void setVal( UInt32 val) = 0;
    virtual UInt32 val() const = 0;
};

/**
 * @brief Fairly complex pool-stored object
 * Class for testing complex objects stored in pool. Built with 
 * multiple inheritance, virtual functions and additional members.
 */
class MyPoolObj: 
    public PoolBase,
    public SListIface< MyPoolObj>,
    public PoolObj< MyPoolObj, UseCustomFixedPool>
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
static bool uTestPools( UnitTest *utest_p)
{

    TypedPool< MyPoolObj> *pool = new TypedPool< MyPoolObj>();
    MyPoolObj *p1 = new ( pool) MyPoolObj();
    MyPoolObj *p2 = new ( pool) MyPoolObj();
    bool called_destructor1 = false;
    bool called_destructor2 = false;

    UTEST_CHECK( utest_p, p1 != p2);
    p1->a = 1;
    p2->a = 2;
    p1->b = 3;
    p2->b = 4;
    p1->called = &called_destructor1;
    p2->called = &called_destructor2;

    p1->setVal( 5);
    p2->setVal( 6);
    UTEST_CHECK( utest_p, p1->a != p1->b);
    UTEST_CHECK( utest_p, p1->a != p2->a);
    UTEST_CHECK( utest_p, p1->b != p2->a);
    UTEST_CHECK( utest_p, p1->b != p2->b);
    UTEST_CHECK( utest_p, p1->a != p2->a);
    
    UTEST_CHECK( utest_p, !called_destructor1);
    UTEST_CHECK( utest_p, !called_destructor2);
    
    pool->destroy( p1);
    
    UTEST_CHECK( utest_p, called_destructor1);
    UTEST_CHECK( utest_p, !called_destructor2);

    pool->destroy( p2);
    
    UTEST_CHECK( utest_p, called_destructor1);
    UTEST_CHECK( utest_p, called_destructor2);

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
bool Utils::uTestMem( UnitTest *utest_p)
{
    /** Test smart pointers */
    uTestRef( utest_p);

    /** Test memory pools */
    uTestPools( utest_p);
    return utest_p->result();
}
