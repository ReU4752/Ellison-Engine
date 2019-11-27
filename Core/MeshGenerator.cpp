#include "CoreStdafx.h"
#include "MeshGenerator.h"

namespace Ellison_Engine
{
	using namespace DirectX;
	using namespace DirectX::PackedVector;

	IMesh* CMeshGenerator::CreateTriangle(const float& distance)
	{
		struct VertexType
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

		VertexType* vertices;
		unsigned int vertexCount = 3;
		vertices = new VertexType[vertexCount];

		unsigned long* indices;
		unsigned int indexCount = 3;
		indices = new unsigned long[indexCount];

		vertices[0].position = XMFLOAT3(-distance, -distance, 0.0f);
		vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[1].position = XMFLOAT3(0.0f, distance, 0.0f);
		vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		vertices[2].position = XMFLOAT3(distance, -distance, 0.0f);
		vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		VERTEX_BUFFER_DESC vDesc;
		vDesc.vertices = vertices;
		vDesc.vertexByte = sizeof(VertexType);
		vDesc.numVertices = vertexCount;

		INDEX_BUFFER_DESC iDesc;
		iDesc.indices = indices;
		iDesc.indexByte = sizeof(unsigned long);
		iDesc.numIndices = indexCount;

		MESH_DESC mDesc;
		mDesc.vertexBuffer = IVertexBuffer::Create(vDesc);
		mDesc.indexBuffer = IIndexBuffer::Create(iDesc);

		IMesh* mesh = CMesh::Create(mDesc);
		if (!mesh)
		{
			delete[] vertices;
			vertices = nullptr;

			delete[] indices;
			indices = nullptr;

			return nullptr;
		}

		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;

		return mesh;
	}

	IMesh* CMeshGenerator::CreateMeshFromEEMFile(std::wstring filePath)
	{
		////////
		std::ifstream fin;
		char input;
		unsigned int i;

		fin.open(filePath);

		if (fin.fail())
		{
			return nullptr;
		}

		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		unsigned int vertexCount;
		fin >> vertexCount;

		unsigned int indexCount = vertexCount;

		ModelType* models = new ModelType[vertexCount];
		if (!models)
		{
			fin.close();
			return nullptr;
		}

		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		for (i = 0; i < vertexCount; i++)
		{
			fin >> models[i].x >> models[i].y >> models[i].z;
			fin >> models[i].tu >> models[i].tv;
			fin >> models[i].nx >> models[i].ny >> models[i].nz;
		}

		fin.close();
		/////////////////

		int faceCount, i2, index;
		TempVertexType vertex1, vertex2, vertex3;
		VectorType tangent, binormal, normal;

		faceCount = vertexCount / 3;

		index = 0;

		for (i2 = 0; i2 < faceCount; i2++)
		{
			// Get the three vertices for this face from the model.
			vertex1.x = models[index].x;
			vertex1.y = models[index].y;
			vertex1.z = models[index].z;
			vertex1.tu = models[index].tu;
			vertex1.tv = models[index].tv;
			vertex1.nx = models[index].nx;
			vertex1.ny = models[index].ny;
			vertex1.nz = models[index].nz;
			index++;

			vertex2.x = models[index].x;
			vertex2.y = models[index].y;
			vertex2.z = models[index].z;
			vertex2.tu = models[index].tu;
			vertex2.tv = models[index].tv;
			vertex2.nx = models[index].nx;
			vertex2.ny = models[index].ny;
			vertex2.nz = models[index].nz;
			index++;

			vertex3.x = models[index].x;
			vertex3.y = models[index].y;
			vertex3.z = models[index].z;
			vertex3.tu = models[index].tu;
			vertex3.tv = models[index].tv;
			vertex3.nx = models[index].nx;
			vertex3.ny = models[index].ny;
			vertex3.nz = models[index].nz;
			index++;

			// Calculate the tangent and binormal of that face.
			CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

			// Calculate the new normal using the tangent and binormal. 
			CalculateNormal(tangent, binormal, normal);

			// Store the normal, tangent, and binormal for this face back in the model structure.
			models[index - 1].nx = normal.x;
			models[index - 1].ny = normal.y;
			models[index - 1].nz = normal.z;
			models[index - 1].tx = tangent.x;
			models[index - 1].ty = tangent.y;
			models[index - 1].tz = tangent.z;
			models[index - 1].bx = binormal.x;
			models[index - 1].by = binormal.y;
			models[index - 1].bz = binormal.z;
			models[index - 2].nx = normal.x;
			models[index - 2].ny = normal.y;
			models[index - 2].nz = normal.z;
			models[index - 2].tx = tangent.x;
			models[index - 2].ty = tangent.y;
			models[index - 2].tz = tangent.z;
			models[index - 2].bx = binormal.x;
			models[index - 2].by = binormal.y;
			models[index - 2].bz = binormal.z;
			models[index - 3].nx = normal.x;
			models[index - 3].ny = normal.y;
			models[index - 3].nz = normal.z;
			models[index - 3].tx = tangent.x;
			models[index - 3].ty = tangent.y;
			models[index - 3].tz = tangent.z;
			models[index - 3].bx = binormal.x;
			models[index - 3].by = binormal.y;
			models[index - 3].bz = binormal.z;
		}



#pragma region INITIALIZEBUFFER
		VertexType* vertices;
		unsigned long* indices;
		int i3;

		vertices = new VertexType[vertexCount];
		if (!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[indexCount];
		if (!indices)
		{
			return false;
		}

		for (i3 = 0; i3<vertexCount; i3++)
		{
			vertices[i3].position = DirectX::XMFLOAT3(models[i3].x, models[i3].y, models[i3].z);
			vertices[i3].normal = DirectX::XMFLOAT3(models[i3].nx, models[i3].ny, models[i3].nz);
			vertices[i3].tangent = DirectX::XMFLOAT3(models[i3].tx, models[i3].ty, models[i3].tz);
			vertices[i3].binormal = DirectX::XMFLOAT3(models[i3].bx, models[i3].by, models[i3].bz);
			vertices[i3].texture = DirectX::XMFLOAT2(models[i3].tu, models[i3].tv);

			indices[i3] = i3;
		}


		VERTEX_BUFFER_DESC vDesc;
		vDesc.vertices = vertices;
		vDesc.vertexByte = sizeof(VertexType);
		vDesc.numVertices = vertexCount;

		INDEX_BUFFER_DESC iDesc;
		iDesc.indices = indices;
		iDesc.indexByte = sizeof(unsigned long);
		iDesc.numIndices = indexCount;

		MESH_DESC mDesc;
		mDesc.vertexBuffer = IVertexBuffer::Create(vDesc);
		mDesc.indexBuffer = IIndexBuffer::Create(iDesc);

		IMesh* mesh = CMesh::Create(mDesc);
		if (!mesh)
		{
			delete[] vertices;
			vertices = nullptr;

			delete[] indices;
			indices = nullptr;

			return nullptr;
		}

		delete[] vertices;
		vertices = nullptr;

		delete[] indices;
		indices = nullptr;
#pragma endregion INITIALIZEBUFFER

		return mesh;
	}

	void CMeshGenerator::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
		VectorType& tangent, VectorType& binormal)
	{
		float vector1[3], vector2[3];
		float tuVector[2], tvVector[2];
		float den;
		float length;


		// Calculate the two vectors for this face.
		vector1[0] = vertex2.x - vertex1.x;
		vector1[1] = vertex2.y - vertex1.y;
		vector1[2] = vertex2.z - vertex1.z;

		vector2[0] = vertex3.x - vertex1.x;
		vector2[1] = vertex3.y - vertex1.y;
		vector2[2] = vertex3.z - vertex1.z;

		// Calculate the tu and tv texture space vectors.
		tuVector[0] = vertex2.tu - vertex1.tu;
		tvVector[0] = vertex2.tv - vertex1.tv;

		tuVector[1] = vertex3.tu - vertex1.tu;
		tvVector[1] = vertex3.tv - vertex1.tv;

		// Calculate the denominator of the tangent/binormal equation.
		den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
		tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
		tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

		binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
		binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
		binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

		// Calculate the length of this normal.
		length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

		// Normalize the normal and then store it
		tangent.x = tangent.x / length;
		tangent.y = tangent.y / length;
		tangent.z = tangent.z / length;

		// Calculate the length of this normal.
		length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

		// Normalize the normal and then store it
		binormal.x = binormal.x / length;
		binormal.y = binormal.y / length;
		binormal.z = binormal.z / length;
	}

	void CMeshGenerator::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
	{
		float length;


		// Calculate the cross product of the tangent and binormal which will give the normal vector.
		normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
		normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
		normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

		// Calculate the length of the normal.
		length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

		// Normalize the normal.
		normal.x = normal.x / length;
		normal.y = normal.y / length;
		normal.z = normal.z / length;
	}
}