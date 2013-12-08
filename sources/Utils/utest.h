/**
 * @file: Utils/utest.h 
 * @defgroup uTest
 * @brief Definition of unit testing tools
 * @ingroup Utils
 */
/*
 * Copyright (C) 2013  Boris Shurygin
 */
#pragma once

#ifndef UTEST_H
#define UTEST_H

#include <string>

namespace Utils
{
//Forward declaration of UnitTest class
class UnitTest;

/**
 * Type of simple testing function pointer
 */
typedef bool (*SimpleTestFuncPtr)();

/**
 * Type of elaborated testing function pointer
 */
typedef bool (*TestFuncPtr)( UnitTest *utest_p);

/**
 * Type of elaborated testing function pointer with out file name
 */
typedef bool (*TestFuncWFileNamePtr)( UnitTest *utest_p, std::string &name);

/**
 * Class for representation of individual check results
 */
class UTestCheck
{
public:
    /** The full constructor */
    UTestCheck( bool result, const std::string &check, const std::string &place):
        check_text( check), location( place), res( result)
    {}

private:
    std::string location;
    std::string check_text;
    bool res;
};

/**
 * Unit test description class
 */
class UnitTest
{
public:
    
    /** Create the empty unit test object and set it up for the work with given streams */
    UnitTest( std::ostream& log_strm = std::cerr, std::ostream& out_strm = std::cout);

    /* text stream accessors */
    inline std::ostream& out(){ return out_stream;}; //< Get stream for test output
    inline std::ostream& log(){ return log_stream;}; //< Get stream for log output
    
    /**
     * Remember results of individual check along with the textual
     * representation of the checked assertion and its location in the code
     */
    void saveCheckResult( bool res, const char *assertion, const char *file, int line);
    
    /** Get list of individual check results */
    std::list<UTestCheck>& results()
    {
        return checks;
    }

    bool result() { return main_res;}; // < Get the overall result of the test
    UInt32 numFails() const { return num_fail;} //< Get the number of failed assertions
    UInt32 numPasses() const { return num_success;} //< Get the number of passed assertions
private:
    std::ostream& out_stream;
    std::ostream& log_stream;
    std::list<UTestCheck> checks;
    UInt32 num_fail;
    UInt32 num_success;
    bool main_res;
};


#define UTEST_CHECK( utest_p, cond)\
     ( utest_p->saveCheckResult(cond, #cond,__FILE__,__LINE__))

/**
 * List of UnitTests with auto-deletion of referenced objects on list destruction
 */
class TestList: public std::list<UnitTest *> 
{
    typedef std::list<UnitTest *>::iterator Iterator;
public:
    
    ~TestList()
    {
        /* Delete all UnitTest objects */
        for ( Iterator it = begin(), end_it = end();
              it != end_it;
              ++it)
        {
            delete *it;        
        }
    }
};

/**
 * Test driver implementation
 */
class TestDriver
{
public:
    /** Run one particular test in simple mode */
    static bool runTest( std::string name, SimpleTestFuncPtr func);
    
    /** Run one particular test in elaborated mode */
    static bool runTest( std::string name, TestFuncPtr func, std::string out_file_name = std::string() );
    static bool runTest( std::string name, TestFuncWFileNamePtr func, std::string out_file_name = std::string() );

    /** Configure output stream */
    static void setLog( std::ostream &log_strm)
    {
        log_stream = &log_strm;
    }
    
    static int returnRes(); //< Generate return result for main
    static void printStats(); //< Print test statistics
    static void useLogFile( std::string name); //< Set output file name and open it
private:
    static void testHeader( std::string name);
    static void testFooter( std::string name);
    static void processResult( bool res, UnitTest *utest = 0);
    static std::ostream *log_stream;
    static std::ofstream file_log;
    static int fail_num;
    static int success_num;
    static TestList tests;
};

} // namespace Utils

#define RUN_TEST( name) Utils::TestDriver::runTest( #name, name)
#define RUN_TEST_OUT_FILE_CHECK( name, file) Utils::TestDriver::runTest( #name, name, std::string(file))

#endif // UTEST_H
