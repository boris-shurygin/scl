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
    RUN_TEST( Utils::uTest);
    /** Test Graph package */
    RUN_TEST( uTestGraph);
    /** Test optimizer package */
    RUN_TEST( Opt::uTest);

    MemMgr::deinit();
    Log::deinit();
    
    cout.flush();
    cerr.flush();

    return 0;
}
