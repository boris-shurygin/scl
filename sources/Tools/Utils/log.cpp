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
LogControlImpl::LogControlImpl() 
{
    for ( Int8 id = 0; id < LOGS_NUM; id++)
    {
        enabled[ id] = false;
        prefix[ id] = string("");
        verbosity[ id] = max_verbosity;
        stream[ id] = NULL;
    }
}

/**
 * Constructor for logs
 */
LogControlImpl::~LogControlImpl()
{
    for ( Int8 id = 0; id < LOGS_NUM; id++)
    {
        if ( enabled[ id] )
        {
            stream[ id]->flush();
            fb[ id].close();
            delete stream[ id];
        }
    }
}