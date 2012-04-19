#include <Base/Log.h>

Log* Log::OutputStream = 0;
LogChannel Log::ChannelState = 0;

SDL_mutex *Log::LogLock = 0;

Log::Log(): _outputStream(&std::cout) {
}

Log::~Log() {
    _outputStream->flush();
}

void Log::Setup() {
    if(!LogLock) {
        LogLock = SDL_CreateMutex();
    }
    if(!OutputStream) {
        OutputStream = new Log();
    }
    EnableAllChannels();
}

void Log::Teardown() {
    DisableAllChannels();
    if(LogLock) {
        SDL_DestroyMutex(LogLock);
        LogLock = 0;
    }
    if(OutputStream) {
        delete OutputStream;
        OutputStream = 0;
    }
}

void Log::SetLock() {
    SDL_LockMutex(LogLock);
}

void Log::ReleaseLock() {
    SDL_UnlockMutex(LogLock);
}

void Log::EnableAllChannels() {
    ChannelState = ~0x00;
}

void Log::DisableAllChannels() {
    ChannelState = 0x00;
}

void Log::EnableChannel(LogChannel channel) {
    ChannelState |= channel;
}

void Log::DisableChannel(LogChannel channel) {
    ChannelState &= ~channel;
}

bool Log::IsChannelEnabled(LogChannel channel) {
    return (ChannelState & channel) != 0;
}

Log& Log::GetLogStream(LogChannel channel) {
    return *OutputStream;
}

void Log::Flush() {
    OutputStream->flush();
}

void Log::flush() {
    _outputStream->flush();
}
