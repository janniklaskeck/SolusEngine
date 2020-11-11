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

	SUUID SUUID::FromBytes(const char* bytes)
	{
		SUUID suuid;

		suuid.Create();
		const std::string str = suuid.ToString();

		BinaryReader reader((void*)str.c_str(), sizeof(SUUID));
		suuid.Data1 = (long)reader.ReadInt32();
		suuid.Data2 = (short)reader.ReadInt16();
		suuid.Data3 = (short)reader.ReadInt16();
		reader.ReadBytes(suuid.Data4, 8);
		return suuid;
	}

	const std::string SUUID::ToString() const
	{
		std::stringstream ss;
		ss.write((const char*)&Data1, sizeof(long));
		ss.write((const char*)&Data2, sizeof(short));
		ss.write((const char*)&Data3, sizeof(short));
		ss.write((const char*)&Data4, sizeof(char) * 8);
		return ss.str();
	}

	bool SUUID::Equals(const SUUID& other) const
	{
		RPC_STATUS dummy;
		return UuidEqual(const_cast<UUID*>((::UUID*)this), const_cast<UUID*>((::UUID*)this), &dummy);
	}
}
