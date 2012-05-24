/**
 * @file: log.h 
 * Implementation of logging functionality
 * @defgroup Log Stream-based log class
 *
 * Implementation of logging functionality
 * @ingroup Utils
 */
/*
 * Utils library in SCL (Simple Compiler)
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef UTILS_LOG_H
#define UTILS_LOG_H

enum LogId
{
    /** LOG default */
    LOG_DEFAULT,
    /** Number of logs */
    LOGS_NUM
};

/**
 * Implementation of logging control
 */
class LogControlImpl
{
public:
    LogControlImpl();
    virtual ~LogControlImpl();
    inline void Register( LogId id, string prefix_str, string filename);
    inline void Register( LogId id, string prefix_str, ostream &stream);
    inline void Register( LogId id, string prefix_str, LogId parent_id);
    inline void log( LogId id, char *mess, ...);
    inline UInt8 verb( LogId id);
    inline ostream& log( LogId id);
    
private:

    static const UInt8 max_verbosity = (UInt8)(-1);

    bool enabled[ LOGS_NUM];
    ostream* stream[ LOGS_NUM];
    string prefix[ LOGS_NUM];
    UInt8 verbosity[ LOGS_NUM];
    filebuf fb[ LOGS_NUM];
};

/**
 * 
 */
inline UInt8 
LogControlImpl::verb( LogId id)
{
    return verbosity[ id];
}

/**
 *
 */
inline void 
LogControlImpl::Register( LogId id, string prefix_str, string filename)
{
    enabled[ id] = true;
    prefix[ id] = prefix_str;
    
    fb[ id].open ( filename, ios::out);
    stream[ id] = new ostream(&fb[ id]);
}


inline void LogControlImpl::Register( LogId id, string prefix, ostream &stream){}
inline void LogControlImpl::Register( LogId id, string prefix, LogId parent_id){}
inline void LogControlImpl::log( LogId id, char *mess, ...){}

/**
 * Get stream for paticular log id and verbosity level
 */
inline ostream& 
LogControlImpl::log( LogId id)
{
    return *stream[ id];
}



/**
 * @brief Logging control class
 *
 * @ingroup Utils
 * @details
 * Log class allows for structured output of internal/debug info
 * 
 * Usage example:
 * @code
//Register logs with identificators LOG_ID1 and LOG_ID2
Log::ptr()->Register( LOG_ID1, "filename");
Log::ptr()->Register( LOG_ID2, LOG_ID1); // Stream of log with LOG_ID2 redirected to log LOG_ID1
 
// Configure logging
Log::ptr()->setVerbosityLevel( VERBOSITY_LEVEL_MAX);
if ( some_cond)
{
    Log::instance()->disable( LOG_ID2); // Messages from log with LOG_ID2 wont be shown
}

Log::ptr()->log( LOG_ID1, VERBOSITY_LEVEL,"qasdfasdf %d", 10);
Log::ptr()->log( LOG_ID2, VERBOSITY_LEVEL) << "my message" << endl;
@endcode
 */
typedef Single< LogControlImpl> Log;

#ifndef NO_LOGS
#  define LOG( log_id, verbosity, message) if ( verbosity <= Log::ptr()->verb( log_id))\
                                                Log::ptr()->log( log_id, message)
#  define LOGS( log_id, verbosity, message) if ( verbosity <= Log::ptr()->verb( log_id))\
                                                Log::ptr()->log( log_id) << message << endl
#else
#  define LOG( log_id, verbosity, message)
#  define LOGS( log_id, verbosity, message)
#endif

#endif /* UTILS_LOG_H */