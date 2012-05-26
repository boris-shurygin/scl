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
        unique_name[ id] = false;
    }
}

/**
 * Constructor for logs
 */
LogControl::~LogControl()
{
    for ( Int8 id = 0; id < LOGS_NUM; id++)
    {
        if ( registered[ id] && unique_name[ id])
        {
            stream[ id]->flush();
            delete stream[ id];
            fb[ id].close();
        }
    }
}

void 
LogControl::add( LogId id, string prefix_str, UInt8 verbosity_level, string filename, bool enable_log)
{
    ASSERT_X( id < LOGS_NUM, "Log implementation", "adding logs to custom stream is not supported yet");

    /* Check if we already have opened the file for some other log */
    bool already_opened = false;
    for ( Int8 tmp_id = 0; tmp_id < LOGS_NUM; ++tmp_id)
    {
        if ( registered[ id] )
        {
            if ( fname[ tmp_id] == filename)
            {
                already_opened = true;
                stream[ id] = stream[ tmp_id];
            }
        }
    }
    registered[ id] = true;
    enabled[ id] = enable_log;
    prefix[ id] = prefix_str;
    verbosity[ id] = verbosity_level;
    fname[ id] = filename;

    if ( !already_opened)
    {
        fb[ id].open ( filename, ios::out);
        stream[ id] = new ostream(&fb[ id]);
        unique_name[ id] = true;
    }
}

void
LogControl::add( LogId id, string prefix, UInt8 verbosity_level, LogId parent_id, bool enable_log)
{ ASSERT_X( 0, "Log implementation", "logs hierarchy is not supported yet");}