#pragma once

#include <type_traits>

namespace Solus
{
	class SProcessor;

	namespace ProcessorPriority
	{
		constexpr int32 Default = 0;
		constexpr int32 Physics = 100;
		constexpr int32 Rendering = 1000;
	}

	class SProcessorManager
	{
	public:

		void Tick(float DeltaTime);

	public:

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<SProcessor, T>>>
		T& Emplace()
		{
			T* Element = new T;
			Processors.push_back(Element);

			SortProcessors();

			return *Element;
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<SProcessor, T>>>
		T& Get() const
		{
			SProcessor* Ptr = (*std::find_if(Processors.begin(), Processors.end(), [](auto Element) { return dynamic_cast<T*>(Element) != nullptr; }));
			T* ElementPtr = static_cast<T*>(Ptr);

			return *ElementPtr;
		}

	private:

		void SortProcessors();

	private:

		std::vector<SProcessor*> Processors;
	};

}