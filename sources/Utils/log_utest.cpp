/**
 * @file: log_utest.cpp 
 * Implementation of logging unit testing
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

using namespace Utils;

/**
 * Test logging package
 */
bool Utils::uTestLogs()
{
    UInt32 message_num = 0;
    
    // Add default log
    log()->add( LOG_DEFAULT, "Default log", 5, "log.txt");
    log()->add( LOG_UTEST, "Unit testing", 5, LOG_DEFAULT);

    // Logging is disabled by default so we should not see these messages in the log file
    LOG( LOG_DEFAULT, "Shouldn't be seen 1 %d", message_num++);
    LOGS( LOG_DEFAULT, "Shouldn't be seen" << " ever 1 " << message_num++);
    LOG( LOG_UTEST, "Shouldn't be seen %d", message_num++);

    // Test printing
    log()->enable( LOG_DEFAULT);
    LOG( LOG_DEFAULT, "Log message %d", message_num++);
    LOG_INC_INDENT( LOG_DEFAULT);

    LOGS( LOG_DEFAULT, "Log message " << "with stream " << message_num++);
    LOGS( LOG_DEFAULT, "A very very long logging message that needs to be written on multiple lines\n" 
                       << "the number of this long message is "
                       << message_num++);
    
    LOG_INC_INDENT( LOG_DEFAULT);
    
    LOGV( LOG_DEFAULT, 4, "Fairly verbose message (verb %d) with formatted print %d\n", 4, message_num++);
    LOG_DEC_INDENT( LOG_DEFAULT);
    
    LOGVS( LOG_DEFAULT, 4, "Fairly verbose message(" << 4 << ") with stream interface " << message_num++);
    LOG_DEC_INDENT( LOG_DEFAULT);

    // These messages are too verbose for currently set level (5) we should not see them in the log file
    LOGV( LOG_DEFAULT, 9, "Very verbose message(%d) with formatted print %d", 9, message_num++);
    LOGVS( LOG_DEFAULT, 9, "Very verbose message(" << 9 << ") with stream interface " << message_num++);

    log()->disable( LOG_DEFAULT);
    LOG( LOG_DEFAULT, "Shouldn't be seen 2 %d", message_num++);
    LOGS( LOG_DEFAULT, "Shouldn't be seen" << " ever 2 "  << message_num++);
    
    log()->enable( LOG_UTEST);

    // Test direct calls to log
    Log::ptr()->log( LOG_UTEST, "Utest log message %d", message_num++);
    Log::ptr()->log( LOG_DEFAULT, "Log message %d", message_num++);

    return true;
}
