/**
 * @file: utils_utest.cpp 
 * Implementation of testing of utils
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

/**
 * Test logging package
 */
bool uTestLogs();

/** Sample class */
class classA
{
private:
    /** private constructors, assignment and destructor */
    classA(){};
    classA( const classA&){};
    classA& operator =( const classA&){};
    ~classA(){};
    /** Needed for singleton creation */
    friend class Single< classA>;
};

/** Typedef for classA encapsulated in singleton */
typedef Single< classA> SingleA;

/**
 * Test simple singleton
 */
bool uTestSingle()
{
    SingleA::init();

    classA* a1 = SingleA::instance();
    classA* a2 = SingleA::instance();
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
bool uTestMisc()
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
bool uTestUtils()
{
    /** Various tests of auxiliary routines */
    if ( !uTestMisc())
        return false;
    
    /** test options parser */
    if ( !uTestConf())
        return false;

    /** test logs */
    if ( !uTestLogs())
        return false;

    /** test singleton */
    if ( !uTestSingle())
        return false;

    /** Tets list classes */
    if ( !uTestList())
        return false;

    /** Test memory management */
    if ( !uTestMem())
        return false;

    return true;
}
