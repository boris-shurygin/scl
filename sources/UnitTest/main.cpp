/**
 * @file: UnitTest/main.cpp
 * Implementation and entry point for unit testing of SCL (Simple Compiler)
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utest_impl.h"

#undef UNIT_TEST_GUI

/**
 * The entry point for GUI version of SCL (Simple Compiler)
 */
int main(int argc, char **argv)
{
    Log::init();
    MemMgr::init();

    /** Test utils */
    if ( !uTestUtils())
        return -1;

    /** Test GraphImpl package */
    if ( !uTestGraphImpl())
        return -1;

    /** Test IR package */
    //if ( !uTestIR())
    //    return -1;
    
    MemMgr::deinit();
    Log::deinit();
    
    cout.flush();
    cerr.flush();
}
