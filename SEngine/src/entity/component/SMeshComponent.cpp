#include "entity/component/SMeshComponent.hpp"

#include "assets/SAssetManager.hpp"

namespace Solus
{

	std::vector<std::pair<SString, void*>> SMeshComponent::GetUniformData() const
	{
		std::vector<std::pair<SString, void*>> Data;

		std::pair<SString, void*> DataPair = { "u_color", (void*)&Color };
		Data.push_back(DataPair);

		return Data;
	}

}