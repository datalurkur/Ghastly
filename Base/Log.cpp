#include <Base/Log.h>

Log* Log::outputStream = 0;
LogChannel Log::channelState = 0;

Log::Log(): _outputStream(&std::cout) {
}

Log::~Log() {
    _outputStream->flush();
}

void Log::EnableAllChannels() {
    channelState = ~0x00;
}

void Log::DisableAllChannels() {
    channelState = 0x00;
}

void Log::EnableChannel(LogChannel channel) {
    channelState |= channel;
}

void Log::DisableChannel(LogChannel channel) {
    channelState &= ~channel;
}

bool Log::IsChannelEnabled(LogChannel channel) {
    return (channelState & channel) != 0;
}

Log& Log::GetLogStream(LogChannel channel) {
    if(!outputStream) {
        outputStream = new Log();
    }
    return *outputStream;
}

void Log::Flush() {
    outputStream->flush();
}

void Log::flush() {
    _outputStream->flush();
}
