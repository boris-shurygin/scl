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
    
    Log::ptr()->Register( LOG_DEFAULT, "log ", "log.txt");

    LOG( LOG_DEFAULT, 0, "Log message");
    LOGS( LOG_DEFAULT, 0, "Log message " << "with stream");

    /** Test utils */
    if ( !uTestUtils())
        return -1;

    /** Test graph package */
    if ( !uTestGraph())
        return -1;
    
    MemMgr::deinit();
    Log::deinit();

}
