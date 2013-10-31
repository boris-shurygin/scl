/**
 * @file: utils_utest.cpp 
 * Implementation of testing of utils
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

using namespace Utils;

/** Sample class */
class A
{
    int a;
    /** private constructors, assignment and destructor */
    A(){};
    A( const A&){};
    A& operator =( const A&){};
    ~A(){};
    /** Needed for singleton creation */
    friend class Single< A>;
};

/** Typedef for A encapsulated in singleton */
typedef Single< A> SingleA;

/**
 * Test simple singleton
 */
static bool uTestSingle()
{
    SingleA::init();

    A* a1 = SingleA::instance();
    A* a2 = SingleA::instance();
    ASSERT( areEqP( a1, a2));
    SingleA::deinit();
    return true;
}

#if 0
namespace UTestFinal
{
    class Base: Final< Base>
    {

    };

    class Derived: public Base
    {};
}
#endif

/**
 * Various tests
 */
static bool uTestMisc()
{
#if 0
    // We can create objects of the class Base
    UTestFinal::Base b;
    
    // This should trigger compiler error saying that the 
    // FinalHook<Base>() constructor is not accessible from Derived 
    UTestFinal::Derived *d = new UTestFinal::Derived(); // <-- compilation error (C2248 in MSVC 10) 
#endif
    return true;
}


/**
 * Test Utils package
 */
bool Utils::uTest()
{
    /** Various tests of auxiliary routines - REPLACED WITH BOOST UTF */
    if ( !uTestMisc())
        return false;
    /** test singleton  - REPLACED WITH BOOST UTF */
    if ( !uTestSingle())
        return false;
    /** Tets list classes */
    if ( !uTestList())
        return false;
    /** Test memory management */
    if ( !uTestMem())
        return false;
    /** Test memory management */
    if ( !uTestLogs())
        return false;

    return true;
}

