#pragma once

#include "Mesh.h"

namespace Ellison_Engine
{
	class CMeshGenerator
	{
		struct VertexType
		{
			DirectX::XMFLOAT3 position;
      DirectX::XMFLOAT2 texture;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT3 tangent;
			DirectX::XMFLOAT3 binormal;
		};

		struct ModelType
		{
			float x, y, z;
      float tu, tv;
      float nx, ny, nz;
			float tx, ty, tz;
			float bx, by, bz;
		};

		struct TempVertexType
		{
			float x, y, z;
      float tu, tv;
			float nx, ny, nz;
		};

		struct VectorType
		{
			float x, y, z;
		};

	public:
		static IMesh* CreateTriangle(const float& distance);

		static IMesh* CreateMeshFromEEMFile(std::wstring filePath);

	private:
		static void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);

		static void CalculateNormal(VectorType, VectorType, VectorType&);
	};
}