 #pragma once

namespace Ellison_Engine
{
	class IRenderer
	{
	public:
		IRenderer();
		virtual ~IRenderer();

		virtual void Render() = 0;

	protected:
		virtual bool Initialize() = 0;
		virtual void Release() = 0;
	};
}