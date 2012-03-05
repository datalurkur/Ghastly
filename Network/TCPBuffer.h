#ifndef TCPBUFFER_H
#define TCPBUFFER_H

#include <Network/ConnectionBuffer.h>
#include <Network/TCPSocket.h>

class TCPBuffer: public ConnectionBuffer {
public:
    TCPBuffer(const NetAddress &dest, unsigned short localPort = 0);
	TCPBuffer(TCPSocket *establishedSocket);
    virtual ~TCPBuffer();

	void startBuffering();
    void stopBuffering();

    void doInboundBuffering();
	void doOutboundBuffering();

	int tcpSerialize(char *dest, const char *src, int size, int maxSize);
	int tcpDeserialize(const char *srcData, char **data, int &size);

private:
	// Make sure the Socket* is properly cast so the correct functions get called
	inline TCPSocket *getSocket() { return (TCPSocket*)_socket; }

private:
	char *_serializationBuffer;
};

typedef std::map<NetAddress,TCPBuffer*> TCPBufferMap;

#endif
