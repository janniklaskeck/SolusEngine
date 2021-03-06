#pragma once

#include "Asset.generated.h"
#include "Engine/SolusEngine.h"
#include "Object/SObject.h"
#include "Utility/Vector.h"
#include "Utility/RTTI.h"

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <filesystem>

namespace Solus
{
	class SAsset;

	SOLUS_CLASS();
	class SOLUS_API Asset : public SObject
	{
		META(Asset, SObject)
	public:

		Asset();

		Asset(const Asset& other);
		Asset(Asset&& other) noexcept;

		~Asset();

		void Set(SAsset* _asset);
		SAsset* Get() const;

		template<typename T>
		T* GetAs() const;

		Asset& operator=(const Asset& other);
		Asset& operator=(Asset&& other) noexcept;
		Asset& operator=(SAsset* ptr);

		SUUID GetId() const;

		bool IsValid() const;

		SAsset* operator->() const;
		SAsset& operator*() const;

		bool operator==(const Asset& other) const;
		bool operator!=(const Asset& other) const;

		operator bool() const;

	private:
		SAsset* asset = nullptr;
	};

	template<typename T>
	T* Solus::Asset::GetAs() const
	{
		if (asset)
			return dynamic_cast<T*>(asset);
		return nullptr;
	}

}
