#pragma once

namespace Solus
{

	class SProcessor
	{
	public:

		virtual void Tick(float DeltaTime) = 0;

	public:

		virtual int32 GetPriority() const;

	};

}