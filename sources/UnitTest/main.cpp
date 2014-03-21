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
    MemMgr::init();

    /** Test utils */
    Utils::uTest();

    /** Test Graph package */
    RUN_TEST( uTestGraph);
    /** Test optimizer package */
    RUN_TEST_OUT_FILE_CHECK( Opt::uTest, "opt_utest.txt");
    
    /*
     * NOTE: For the unit tests below the logging can be usefull, 
     * the logging itself is tested above in Utils::uTest
     */
    Log::init();
    
    log()->add( LOG_REG_EXP, "RegExp", -1, "reg_exp_log.txt");
    log()->enable( LOG_REG_EXP);
    
    RUN_TEST( RegExp::uTestFA);
    RUN_TEST( RegExp::uTest);
    RUN_TEST_OUT_FILE_CHECK( Syntax::Arithmetics::uTest, "stx_utest.txt");
    RUN_TEST_OUT_FILE_CHECK( Lowering::uTestArithm, "low_utest_arithm.txt");

    Log::deinit();
    

    MemMgr::deinit();
    
    cout.flush();
    cerr.flush();
    TestDriver::printStats();
    return TestDriver::returnRes();
}
