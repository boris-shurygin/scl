/**
 * @file: utest.cpp 
 * Implementation of unit testing tools
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

using namespace Utils;

int TestDriver::fail_num = 0;
int TestDriver::success_num = 0;

bool TestDriver::runTest( std::string name, TestFuncPtr func)
{
	cout << "Running " << name <<":" << endl;
	
    bool res = func();

    if ( res )
    {
        success_num++;
		cout << "success" << endl;
    } else
	{
        cout << "fail" << endl;
        fail_num++;
	} 
    cout << endl;
    
    return res;
}