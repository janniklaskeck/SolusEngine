#include "UUID.h"

#include "BinaryReader.h"

#include <Windows.h>
#include <sstream>

namespace Solus
{
	SUUID SUUID::DEFAULT = SUUID();

	SUUID::SUUID()
	{
		assert(UuidCreateNil((::UUID*)this) == RPC_S_OK);
	}

	bool SUUID::Create()
	{
		auto result = UuidCreateSequential((::UUID*)this);
		if (result == RPC_S_UUID_NO_ADDRESS)
			return false;
		return true;
	}

	SUUID SUUID::FromString(const std::string& valueString)
	{
		SUUID suuid;
		std::stringstream ss1;
		ss1 << std::hex << valueString.substr(0, 8);
		ss1 >> suuid.Data1;

		std::stringstream ss2;
		ss2 << std::hex << valueString.substr(8, 4);
		ss2 >> suuid.Data2;
		
		std::stringstream ss3;
		ss3 << std::hex << valueString.substr(12, 4);
		ss3 >> suuid.Data3;

		for (int i = 0; i < 8; i++)
		{
			std::stringstream ss;
			ss << valueString.substr(16 + i * 2, 2);
			ss >> suuid.Data4[i];
		}
		return suuid;
	}

	const std::string SUUID::ToString() const
	{
		std::stringstream ss;
		ss << std::hex << Data1;
		ss << std::hex << Data2;
		ss << std::hex << Data3;
		uint16_t bytePair;
		for (int i = 0; i < 8; i += 2)
		{
			unsigned char data[2] = { Data4[i + 1], Data4[i] };
			memcpy(&bytePair, data, 2);
			ss << std::hex << bytePair;
		}
		return ss.str();
	}

	bool SUUID::Equals(const SUUID& other) const
	{
		RPC_STATUS dummy;
		return UuidEqual(const_cast<UUID*>((::UUID*)this), const_cast<UUID*>((::UUID*)this), &dummy);
	}
}
