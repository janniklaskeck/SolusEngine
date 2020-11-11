#pragma once
#include "Engine/SolusEngine.h"

#include <string>

namespace Solus
{
	class SOLUS_API SUUID
	{
	public:
		SUUID();

		bool Create();

		bool Equals(const SUUID& other) const;

		static SUUID DEFAULT;

		static SUUID FromBytes(const char* bytes);

		const std::string ToString() const;

	private:
		unsigned long Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char Data4[8];
	};

	inline bool operator==(const SUUID& first, const SUUID& other)
	{
		return first.Equals(other);
	}

	inline bool operator!=(const SUUID& first, const SUUID& other)
	{
		return !(first == other);
	}
}

namespace std
{
	template<> struct hash<Solus::SUUID>
	{
		size_t operator()(const Solus::SUUID& guid) const noexcept
		{
			const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&guid);
			std::hash<std::uint64_t> hash;
			return hash(p[0]) ^ hash(p[1]);
		}
	};
}