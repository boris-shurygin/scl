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
    // Add default log
    Log::ptr()->add( LOG_DEFAULT, "Default log", 5, "log.txt");

    // Logging is disabled by default so we should not see these messages in the log file
    LOG( LOG_DEFAULT, "Shouldn't be seen 1");
    LOGS( LOG_DEFAULT, "Shouldn't be seen" << " ever 1");
    
    // Test printing
    Log::ptr()->enable( LOG_DEFAULT);
    LOG( LOG_DEFAULT, "Log message");
    LOGS( LOG_DEFAULT, "Log message " << "with stream");
    LOGV( LOG_DEFAULT, 4, "Fairly verbose message(%d) with formatted print\n", 4);
    LOGVS( LOG_DEFAULT, 4, "Fairly verbose message(" << 4 << ") with stream interface");
    
    // These messages are too verbose for currently set level (5) we should not see them in the log file
    LOGV( LOG_DEFAULT, 9, "Very verbose message(%d) with formatted print", 9);
    LOGVS( LOG_DEFAULT, 9, "Very verbose message(" << 9 << ") with stream interface");

    Log::ptr()->disable( LOG_DEFAULT);
    LOG( LOG_DEFAULT, "Shouldn't be seen 2");
    LOGS( LOG_DEFAULT, "Shouldn't be seen" << " ever 2");

    return true;
}
