#include "UUID.h"

#include "BinaryReader.h"

#include <sstream>
#include <limits>
#include <random>
#include <iomanip>

namespace Solus
{
	SUUID SUUID::DEFAULT = SUUID();

	static std::random_device device;
	static std::mt19937 generator(device());
	static std::uniform_int_distribution<uint32_t> dist(1, std::numeric_limits<uint32_t>::max());

	SUUID::SUUID()
	{
		data = dist(generator);
	}

	SUUID SUUID::FromString(const std::string& valueString)
	{
		uint32_t readData;
		std::stringstream ss;
		ss << valueString;
		ss >> readData;
		SUUID suuid;
		suuid.data = readData;
		return suuid;
	}

	const std::string SUUID::ToString() const
	{
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(10) << data;
		return ss.str();
	}

	const uint32_t SUUID::GetRaw() const
	{
		return data;
	}

	bool SUUID::operator==(const SUUID& other)
	{
		return data == other.data;
	}

	bool SUUID::Equals(const SUUID& other) const
	{
		return data == other.data;
	}
}
