#pragma once

namespace Ellison_Engine
{
	class CTime
	{
	public:
		CTime(void);
		~CTime(void);

		void Initialize(void);

		void Update(void);

		float DeltaTime(void);
		float Time(void);

	private:
		LARGE_INTEGER _frameTime;
		LARGE_INTEGER _fixTime;
		LARGE_INTEGER _lastTime;
		LARGE_INTEGER _cpuTick;

		float _deltaTime;
		float _time;
	};
}