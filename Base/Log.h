#ifndef LOG_H
#define LOG_H

#include <Base/Base.h>

#define LOG_DEBUG   0x01
#define LOG_INFO    0x02
#define LOG_WARNING 0x04
#define LOG_ERROR   0x08

typedef char LogChannel;

class Log {
public:
    static void EnableAllChannels();
    static void DisableAllChannels();
 
    static void EnableChannel(LogChannel channel);
    static void DisableChannel(LogChannel channel);

    static bool IsChannelEnabled(LogChannel channel);

    static Log& GetLogStream(LogChannel channel);
    static void Flush();

public:
    Log();
    virtual ~Log();
    
    void flush();

    template <typename T>
	Log& operator<<(const T &rhs);

private:
    static Log *outputStream;
    static LogChannel channelState;

private:
    std::ostream *_outputStream;
};

template <typename T>
Log& Log::operator<<(const T &rhs) {
    (*_outputStream) << rhs;
    return *this;
}

#define LogToChannel(channel, msg) \
    do { \
        if(Log::IsChannelEnabled(channel)) { \
            Log::GetLogStream(channel) << msg << "\n"; \
            Log::Flush(); \
        } \
    } while(false)


#define Debug(msg) LogToChannel(LOG_DEBUG,   msg)
#define Info(msg)  LogToChannel(LOG_INFO,    msg)
#define Warn(msg)  LogToChannel(LOG_WARNING, msg)
#define Error(msg) LogToChannel(LOG_ERROR,   msg)

#endif
