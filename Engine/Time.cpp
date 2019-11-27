#include "EngineStdafx.h"
#include "Time.h"

namespace Ellison_Engine
{
	CTime::CTime(void)
		: _deltaTime(0), _time(0)
	{
	}

	CTime::~CTime(void)
	{
	}

	void CTime::Initialize(void)
	{
		QueryPerformanceCounter(&_frameTime);
		QueryPerformanceCounter(&_fixTime);
		QueryPerformanceCounter(&_lastTime);
		QueryPerformanceFrequency(&_cpuTick);
	}

	void CTime::Update(void)
	{
		QueryPerformanceCounter(&_frameTime);

		if (_frameTime.QuadPart - _lastTime.QuadPart > _cpuTick.QuadPart)
		{
			QueryPerformanceFrequency(&_cpuTick);
			_lastTime.QuadPart = _frameTime.QuadPart;
		}

		_deltaTime = static_cast<float>(_frameTime.QuadPart - _fixTime.QuadPart) / _cpuTick.QuadPart;
		_time += _deltaTime;

		_fixTime = _frameTime;
	}

	float CTime::DeltaTime(void)
	{
		return _deltaTime;
	}

	float CTime::Time(void)
	{
		return _time;
	}
}