/**
 * File: utest_impl.h - Header for unit testing of libraries and tools of SCL (Simple Compiler)
 * Copyright (C) 2012  Boris Shurygin
 */
#include "../Tools/tools_iface.h"

/**
 * Test Utils package
 */
bool uTestUtils();

/**
 * Graph library unit testing
 */
bool uTestGraph();

/**
 * UnitTesting of GUI
 */
bool uTestGUI(int argc, char **argv);

/**
 * Frontend unit testing
 */
bool uTestFE();
