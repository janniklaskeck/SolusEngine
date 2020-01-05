#include "RTTI.h"

#include "Vector.h"

namespace Solus
{
	//--------------------------------------------------------
	// A type descriptor for int
	//--------------------------------------------------------

	struct TypeDescriptor_Int : TypeDescriptor
	{
		TypeDescriptor_Int() : TypeDescriptor{ "int", sizeof(int) }
		{}
		virtual void Print(const void* obj, size_t /* unused */) const override
		{
			std::cout << "int{" << *(const int*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* GetPrimitiveDescriptor<int>()
	{
		static TypeDescriptor_Int typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for uint64_t
	//--------------------------------------------------------

	struct TypeDescriptor_Uint64_t : TypeDescriptor
	{
		TypeDescriptor_Uint64_t() : TypeDescriptor{ "uint64_t", sizeof(uint64_t) }
		{}
		virtual void Print(const void* obj, size_t /* unused */) const override
		{
			std::cout << "int{" << *(const uint64_t*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* GetPrimitiveDescriptor<uint64_t>()
	{
		static TypeDescriptor_Uint64_t typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for float
	//--------------------------------------------------------

	struct TypeDescriptor_Float : TypeDescriptor
	{
		TypeDescriptor_Float() : TypeDescriptor{ "float", sizeof(float) }
		{}
		virtual void Print(const void* obj, size_t /* unused */) const override
		{
			std::cout << "float{" << *(const float*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* GetPrimitiveDescriptor<float>()
	{
		static TypeDescriptor_Float typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for double
	//--------------------------------------------------------

	struct TypeDescriptor_Double : TypeDescriptor
	{
		TypeDescriptor_Double() : TypeDescriptor{ "double", sizeof(double) }
		{}
		virtual void Print(const void* obj, size_t /* unused */) const override
		{
			std::cout << "double{" << *(const double*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* GetPrimitiveDescriptor<double>()
	{
		static TypeDescriptor_Double typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for bool
	//--------------------------------------------------------

	struct TypeDescriptor_Bool : TypeDescriptor
	{
		TypeDescriptor_Bool() : TypeDescriptor{ "bool", sizeof(bool) }
		{}
		virtual void Print(const void* obj, size_t /* unused */) const override
		{
			std::cout << "bool{" << *(const bool*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* GetPrimitiveDescriptor<bool>()
	{
		static TypeDescriptor_Bool typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for std::string
	//--------------------------------------------------------

	struct TypeDescriptor_StdString : TypeDescriptor
	{
		TypeDescriptor_StdString() : TypeDescriptor{ "std::string", sizeof(std::string) }
		{}
		virtual void Print(const void* obj, size_t /* unused */) const override
		{
			std::cout << "std::string{\"" << *(const std::string*) obj << "\"}";
		}
	};

	template <>
	TypeDescriptor* GetPrimitiveDescriptor<std::string>()
	{
		static TypeDescriptor_StdString typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Vec3f
	//--------------------------------------------------------

	struct TypeDescriptor_Vec3f : TypeDescriptor
	{
		TypeDescriptor_Vec3f() : TypeDescriptor{ "Vec3f", sizeof(Vec3f) }
		{}
		virtual void Print(const void* obj, size_t /* unused */) const override
		{
			Vec3f value = *(const Vec3f*)obj;
			std::cout << "Vec3f{\"x: " << value.x << "\ny: " << value.y << "\nz: " << value.z << "\"}";
		}
	};

	template <>
	TypeDescriptor* GetPrimitiveDescriptor<Vec3f>()
	{
		static TypeDescriptor_Vec3f typeDesc;
		return &typeDesc;
	}

}