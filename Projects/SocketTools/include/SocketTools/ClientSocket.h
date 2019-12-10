#pragma once

#include <cstdint>

#include <SocketTools/Socket.h>

namespace socket_tools
{
	class SOCKET_TOOLS_API ClientSocket :
		public Socket
	{
	public:
		using Socket::Socket;

		ClientSocket(
			SocketMode mode,
			Protocol protocol,
			AddressFamily family,
			const char* address,
			const char* port);

		template<typename T, size_t size>
		size_t Send(const T(&data)[size])
		{
			return Send(
				reinterpret_cast<const uint8_t*>(data),
				size * sizeof(T));
		}

		template<typename T, size_t size>
		size_t Receive(T(&data)[size])
		{
			return Receive(
				reinterpret_cast<uint8_t*>(data),
				size * sizeof(T));
		}

		size_t Send(const uint8_t* data, size_t size);
		size_t Receive(uint8_t* data, size_t size);
	};
}