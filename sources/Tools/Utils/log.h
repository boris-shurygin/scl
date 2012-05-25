/**
 * @file: log.h 
 * Implementation of logging functionality
 * @defgroup Logs Stream-based log class
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

/**
 * Log identificator
 * @ingroup Logs
 */
enum LogId
{
    /** LOG default */
    LOG_DEFAULT,
    /** Number of logs */
    LOGS_NUM
};

/**
 * @brief Logging control class
 *
 * @ingroup Logs
 * @details
 * Log class allows for structured output of internal/debug info
 * 
 */
class LogControl
{
public:
    LogControl();
    virtual ~LogControl();
    inline void add( LogId id, string prefix_str, UInt8 verbosity_level, string filename);
    inline void add( LogId id, string prefix_str, UInt8 verbosity_level, ostream &stream);
    inline void add( LogId id, string prefix_str, UInt8 verbosity_level, LogId parent_id);
    inline void log( LogId id, std::ostringstream& os);
    inline void log( LogId id, char *mess, ...);
    inline UInt8 verb( LogId id);
    inline bool isEnabled( LogId id);
   
private:

    static const UInt8 max_verbosity = (UInt8)(-1);
    static const UInt32 max_buf_size = 256;

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
LogControl::verb( LogId id)
{
    return verbosity[ id];
}

/**
 * Check that log with given id is enabled
 */
inline bool 
LogControl::isEnabled( LogId id)
{
    return enabled[ id];
}

/**
 *
 */
inline void 
LogControl::add( LogId id, string prefix_str, UInt8 verbosity_level, string filename)
{
    enabled[ id] = true;
    prefix[ id] = prefix_str;
    verbosity[ id] = verbosity_level;
    fb[ id].open ( filename, ios::out);
    stream[ id] = new ostream(&fb[ id]);
}


inline void LogControl::add( LogId id, string prefix, UInt8 verbosity_level, ostream &stream)
{ ASSERT_X( 0, "Log implementation", "adding logs to custom stream is not supported yet");}

inline void LogControl::add( LogId id, string prefix, UInt8 verbosity_level, LogId parent_id)
{ ASSERT_X( 0, "Log implementation", "logs hierarchy is not supported yet");}

/**
 * Print formated message into log
 */
inline void LogControl::log( LogId id, char *mess, ...)
{
    if ( enabled[ id])
    {
        char buf[ max_buf_size];
        va_list args;
        va_start( args, mess);
        vsnprintf_s( buf, max_buf_size - 1, mess, args);
        va_end( args);
        *(stream[ id]) << prefix[ id] << ": " << buf << endl;
    }
}

/**
 * Get stream for paticular log id and verbosity level
 */
inline void
LogControl::log( LogId id, std::ostringstream& os)
{
    if ( enabled[ id])
    {
        *(stream[ id]) << prefix[ id] << ": " << os.str() << endl;
    }
}



/**
 * @brief Singleton of logging control class
 *
 * @ingroup Logs
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
    Log::ptr()->disable( LOG_ID2); // Messages from log with LOG_ID2 wont be shown
}

LOG( LOG_ID1, VERBOSITY_LEVEL,"my favourite constant is %d", 10);
LOGS( LOG_ID2, VERBOSITY_LEVEL, "my message");
@endcode
 *
 * @sa LogControl
 */
typedef Single< LogControl> Log;

#ifndef NO_LOGS
#  define LOGV( log_id, verbosity, message, ...) \
     if ( Log::ptr()->isEnabled( log_id)\
          && verbosity <= Log::ptr()->verb( log_id) ) Log::ptr()->log( log_id, message, __VA_ARGS__)
#  define LOGVS( log_id, verbosity, message)\
    if ( bool cond = true\
         && Log::ptr()->isEnabled( log_id)\
         && verbosity <= Log::ptr()->verb( log_id) ) for ( std::ostringstream os; cond; cond = false)\
         {os << message;Log::ptr()->log( log_id, os);}
#  define LOG( log_id, message, ...) LOGV( log_id, 0, message, __VA_ARGS__)
#  define LOGS( log_id, message) LOGVS( log_id, 0, message)
#else
#  define LOGV( log_id, verbosity, message, ...)
#  define LOGVS( log_id, verbosity, message)
#  define LOG( log_id, verbosity, message)
#  define LOGS( log_id, verbosity, message)
#endif

#endif /* UTILS_LOG_H */