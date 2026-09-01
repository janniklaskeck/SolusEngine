#pragma once

#include <atomic>

namespace Solus
{
	class RefCounted
	{
		using RefCountType = uint32;

	public:

		RefCountType GetRefCount() const
		{
			return RefCount.load();
		}

		void Acquire() const
		{
			RefCount.fetch_add(1, std::memory_order_relaxed);
		}

		void Release() const
		{
			if (RefCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
			{
				delete this;
			}
		}

	protected:

		RefCounted() = default;
		virtual ~RefCounted() = default;

	private:

		mutable std::atomic<RefCountType> RefCount{ 0 };
	};

	template<typename T, typename = std::enable_if_t<std::is_base_of_v<RefCounted, T>>>
	class Ref
	{
	public:
		Ref() = default;

		Ref(T* Other)
		{
			Ptr = Other;
			
			if (Ptr)
			{
				Ptr->Acquire();
			}
		}

		Ref(const Ref& Other)
		{
			Ptr = Other.Ptr;
			
			if (Ptr)
			{
				Ptr->Acquire();
			}
		}

		Ref(Ref&& Other) noexcept
		{
			if (Other.Ptr)
			{
				//Other.Ptr->Release();
			}

			Ptr = Other.Ptr;

			Other.Ptr = nullptr;
		}

		~Ref()
		{
			if (Ptr)
			{
				Ptr->Release();
			}
		}

		Ref& operator=(T* Value)
		{
			if (Ptr == Value)
			{
				return *this;
			}

			if (Ptr)
			{
				//Ptr->Release();
			}

			Ptr = Value;

			if (Ptr)
			{
				Ptr->Acquire();
			}

			return *this;
		}

		Ref& operator=(const Ref& Other)
		{
			if (&Other == this)
			{
				return *this;
			}

			Ptr = Other.Ptr;
			
			if (Ptr)
			{
				Ptr->Acquire();
			}

			return *this;
		}

		Ref& operator=(Ref&& Other) noexcept
		{
			if (&Other == this)
			{
				return *this;
			}

			Ptr = Other.Ptr;

			if (Other.Ptr)
			{
				//Other.Ptr->Release();
			}

			Other.Ptr = nullptr;

			return *this;
		}

		int32 GetRefCount() const
		{
			if (!Ptr)
			{
				return 0;
			}

			return Ptr->GetRefCount();
		}

		T* Get() const noexcept 
		{ 
			return Ptr;
		}

		template<typename S>
		S* GetAs() const noexcept
		{
			return dynamic_cast<S*>(Ptr);
		}
		
		T& operator*() const noexcept 
		{
			return *Ptr;
		}

		T* operator->() const noexcept 
		{ 
			return Ptr;
		}
		
		explicit operator bool() const noexcept 
		{ 
			return Ptr != nullptr;
		}

	private:

		T* Ptr = nullptr;
	};
}