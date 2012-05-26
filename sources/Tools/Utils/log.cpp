/**
 * @file: log.cpp 
 * Implementation of logging
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

/**
 * Constructor for logs
 */
LogControl::LogControl() 
{
    for ( Int8 id = 0; id < LOGS_NUM; id++)
    {
        enabled[ id] = false;
        registered[ id] = false;
        prefix[ id] = string("");
        verbosity[ id] = max_verbosity;
        stream[ id] = NULL;
    }
}

/**
 * Constructor for logs
 */
LogControl::~LogControl()
{
    for ( Int8 id = 0; id < LOGS_NUM; id++)
    {
        if ( registered[ id] )
        {
            stream[ id]->flush();
            fb[ id].close();
            delete stream[ id];
        }
    }
}

void 
LogControl::add( LogId id, string prefix_str, UInt8 verbosity_level, string filename, bool enable_log)
{
    ASSERT_X( id < LOGS_NUM, "Log implementation", "adding logs to custom stream is not supported yet");

    registered[ id] = true;
    enabled[ id] = enable_log;
    prefix[ id] = prefix_str;
    verbosity[ id] = verbosity_level;
    fb[ id].open ( filename, ios::out);
    stream[ id] = new ostream(&fb[ id]);
}

void
LogControl::add( LogId id, string prefix, UInt8 verbosity_level, LogId parent_id, bool enable_log)
{ ASSERT_X( 0, "Log implementation", "logs hierarchy is not supported yet");}