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
    UInt32 message_num = 0;
    
    // Add default log
    Log::ptr()->add( LOG_DEFAULT, "Default log", 5, "log.txt");
    Log::ptr()->add( LOG_UTEST, "Unit testing", 5, LOG_DEFAULT);

    // Logging is disabled by default so we should not see these messages in the log file
    LOG( LOG_DEFAULT, "Shouldn't be seen 1 %d", message_num++);
    LOGS( LOG_DEFAULT, "Shouldn't be seen" << " ever 1 " << message_num++);
    LOG( LOG_UTEST, "Shouldn't be seen %d", message_num++);

    // Test printing
    Log::ptr()->enable( LOG_DEFAULT);
    LOG( LOG_DEFAULT, "Log message %d", message_num++);
    LOGS( LOG_DEFAULT, "Log message " << "with stream " << message_num++);
    LOGV( LOG_DEFAULT, 4, "Fairly verbose message (verb %d) with formatted print %d\n", 4, message_num++);
    LOGVS( LOG_DEFAULT, 4, "Fairly verbose message(" << 4 << ") with stream interface " << message_num++);
    
    // These messages are too verbose for currently set level (5) we should not see them in the log file
    LOGV( LOG_DEFAULT, 9, "Very verbose message(%d) with formatted print %d", 9, message_num++);
    LOGVS( LOG_DEFAULT, 9, "Very verbose message(" << 9 << ") with stream interface " << message_num++);

    Log::ptr()->disable( LOG_DEFAULT);
    LOG( LOG_DEFAULT, "Shouldn't be seen 2 %d", message_num++);
    LOGS( LOG_DEFAULT, "Shouldn't be seen" << " ever 2 "  << message_num++);
    
    Log::ptr()->enable( LOG_UTEST);

    // Test direct calls to log
    Log::ptr()->log( LOG_UTEST, "Utest log message %d", message_num++);
    Log::ptr()->log( LOG_DEFAULT, "Log message %d", message_num++);

    return true;
}