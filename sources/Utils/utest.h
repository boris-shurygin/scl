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

/**
 * Type of testing function pointer
 */
typedef bool (*TestFuncPtr)();

/**
 * Test driver implementation
 */
class TestDriver
{
public:
    /** Run one particular test */
    static bool runTest(std::string name, TestFuncPtr func);
private:
    static int fail_num;
    static int success_num;
};

} // namespace Utils

#define RUN_TEST( name) Utils::TestDriver::runTest( #name, name);

#endif // UTEST_H
