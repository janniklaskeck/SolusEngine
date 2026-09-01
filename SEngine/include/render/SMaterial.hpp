#pragma once

namespace Solus
{
	class SOLUS_API SMaterial
	{
	public:

		virtual ~SMaterial() = default;

		virtual bool SetAndCompileShaders(const SAssetPath& VertexAsset, const SAssetPath& FragmentAsset) = 0;

	private:

		SString MaterialName;
	};
}