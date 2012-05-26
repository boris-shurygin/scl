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
 * Debug assert for logs
 * @ingroup GraphBase
 */
#if !defined(LOG_ASSERTD)
#    define LOG_ASSERTD(cond, what) ASSERT_XD(cond, "Logs", what)
#endif

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
    
    /** Log colntrol contructor. 
     *  By default all logs are disabled, use add routine to register them.
     *  Use enable/disable to turn it on/off.
     */
    LogControl();
    
    /** Destructor */
    virtual ~LogControl();
    
    /** Register log that writes to file with given name */
    void add( LogId id, string prefix_str, UInt8 verbosity_level, string filename, bool enable_log = false); 
    
    /** Register log that writes to file with given name */
    void add( LogId id, string prefix_str, UInt8 verbosity_level, LogId parent_id, bool enable_log = false);
    
    /* Control of logs suppression */
    inline void enable( LogId id);  /**< Enable log with given id */
    inline void disable( LogId id); /**< Disable log with given id */

    /* Log printing interface */
    inline void log( LogId id, std::ostringstream& os); /**< Print string stream to log */
    inline void log( LogId id, char *mess, ...);        /**< Print formatted string to log */
    
    inline UInt8 verb( LogId id);     /**< Get verbosity level of log with given id */
    inline bool isEnabled( LogId id); /**< Check that log is enabled */

private:

    static const UInt8 max_verbosity = (UInt8)(-1);
    static const UInt32 max_buf_size = 256;

    bool registered[ LOGS_NUM];
    bool enabled[ LOGS_NUM];
    ostream* stream[ LOGS_NUM];
    string prefix[ LOGS_NUM];
    UInt8 verbosity[ LOGS_NUM];
    filebuf fb[ LOGS_NUM];
};

/**
 * Get verbosity level of log with given id
 */
inline UInt8 
LogControl::verb( LogId id)
{
    LOG_ASSERTD( registered[ id], "log id is not registered");
    return verbosity[ id];
}

/**
 * Check that log with given id is enabled
 */
inline bool 
LogControl::isEnabled( LogId id)
{
    LOG_ASSERTD( registered[ id], "log id is not registered");
    return enabled[ id];
}

/**
 * Print formated message into log
 */
inline void LogControl::log( LogId id, char *mess, ...)
{
    LOG_ASSERTD( registered[ id], "log id is not registered");
    if ( enabled[ id])
    {
        char buf[ max_buf_size];
        
        /* Fill buffer with formatted string */
        va_list args;
        va_start( args, mess);
        vsnprintf_s( buf, max_buf_size - 1, mess, args);
        va_end( args);

        /* Print string to stream */
        *(stream[ id]) << prefix[ id] << ": " << buf << endl;
    }
}

/**
 * Print string stream to log
 */
inline void
LogControl::log( LogId id, std::ostringstream& os)
{
    LOG_ASSERTD( registered[ id], "log id is not registered");
    if ( enabled[ id])
    {
        *(stream[ id]) << prefix[ id] << ": " << os.str() << endl;
    }
}
  
/**
 * Enable log with given id 
 */
inline void LogControl::enable( LogId id)
{
    LOG_ASSERTD( registered[ id], "log id is not registered");
    enabled[ id] = true;
}  

/**
 * Disable log with given id
 */
inline void LogControl::disable( LogId id)
{
    LOG_ASSERTD( registered[ id], "log id is not registered");
    enabled[ id] = false;
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