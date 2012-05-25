/**
 * @file: log_utest.cpp 
 * Implementation of logging unit testing
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

/**
 * Test logging package
 */
bool uTestLogs()
{
    //Add some logs
    Log::ptr()->add( LOG_DEFAULT, "Default log", 5, "log.txt");

    LOG( LOG_DEFAULT, "Log message");
    LOGS( LOG_DEFAULT, "Log message " << "with stream");
    LOGV( LOG_DEFAULT, 4, "Fairly verbose message(%d) with formatted print\n", 4);
    LOGVS( LOG_DEFAULT, 4, "Fairly verbose message(" << 4 << ") with stream interface");
    LOGV( LOG_DEFAULT, 9, "Very verbose message(%d) with formatted print", 9);
    LOGVS( LOG_DEFAULT, 9, "Very verbose message(" << 9 << ") with stream interface");
    return true;
}
