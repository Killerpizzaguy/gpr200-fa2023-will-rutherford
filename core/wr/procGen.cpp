#include "procGen.h"

namespace wr {

	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		ew::MeshData myMesh;

		for (unsigned int row = 0; row <= subdivisions; row++)
		{
			for (unsigned int col = 0; col <= subdivisions; col++)
			{
				ew::Vertex vert;

				vert.pos.x = -width / 2 + width * ((float)col / subdivisions);
				vert.pos.y = 0;
				vert.pos.z = height / 2 - height * ((float)row / subdivisions);

				vert.uv.x = ((float)col / subdivisions);
				vert.uv.y = ((float)row / subdivisions);

				vert.normal = ew::Vec3(0, 1, 0);
				myMesh.vertices.push_back(vert);
			}
		}

		int indCol = subdivisions + 1;
		for (unsigned int row = 0; row < subdivisions; row++)
		{
			for (unsigned int col = 0; col < subdivisions; col++)
			{
				int start = row * indCol + col;
				myMesh.indices.push_back(start);
				myMesh.indices.push_back(start + 1);
				myMesh.indices.push_back(start + indCol + 1);
				myMesh.indices.push_back(start + indCol + 1);
				myMesh.indices.push_back(start + indCol);
				myMesh.indices.push_back(start);
			}
		}

		return myMesh;
	}

	void createRing(ew::MeshData* mesh, float r, int subdivisions, float y, bool facingUp)
	{
		float step = (ew::PI * 2) / subdivisions;
		for (unsigned int i = 0; i <= subdivisions; i++)
		{
			float theta = i * step;
			ew::Vertex vert;

			vert.pos.x = cos(theta)*r;
			vert.pos.z = sin(theta)*r;
			vert.pos.y = y;

			if (facingUp)
			{
				vert.normal.y = sin(y);
				vert.normal.x = 0;
				vert.normal.z = 0;

				vert.uv.x = cos(theta) * 0.5 + 0.5;
				vert.uv.y = sin(theta) * 0.5 + 0.5;
			}
			else
			{
				vert.normal.x = cos(theta);
				vert.normal.y = 0;
				vert.normal.z = sin(theta);

				vert.uv.x = (float)i / subdivisions;
				if (y > 0)
					vert.uv.y = 1;
				else
					vert.uv.y = 0;
			}
			mesh->vertices.push_back(vert);
		}
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData myMesh;

		float topY = height / 2;
		float botY = -topY;

		ew::Vertex topVert;
		topVert.pos.x = 0;
		topVert.pos.y = topY;
		topVert.pos.z = 0;

		topVert.normal.x = 0;
		topVert.normal.y = 1;
		topVert.normal.z = 0;

		topVert.uv.x = 0.5;
		topVert.uv.y = 0.5;

		myMesh.vertices.push_back(topVert);

		//make rings
		createRing(&myMesh, radius, numSegments, topY, true);
		createRing(&myMesh, radius, numSegments, topY, false);
		createRing(&myMesh, radius, numSegments, botY, false);
		createRing(&myMesh, radius, numSegments, botY, true);

		ew::Vertex botVert;
		botVert.pos.x = 0;
		botVert.pos.y = botY;
		botVert.pos.z = 0;

		botVert.normal.x = 0;
		botVert.normal.y = -1;
		botVert.normal.z = 0;

		botVert.uv.x = 0.5;
		botVert.uv.y = 0.5;

		myMesh.vertices.push_back(botVert);


		int col = numSegments + 1;
		for (unsigned int i = 0; i < col; i++)
		{
			myMesh.indices.push_back(0);
			myMesh.indices.push_back(i + 1);
			myMesh.indices.push_back(i);
		}
		
		for (unsigned int i = 0; i < col; i++)
		{
			int start = col + i;

			myMesh.indices.push_back(start);
			myMesh.indices.push_back(start + 1);
			myMesh.indices.push_back(start + col);
			myMesh.indices.push_back(start + col);
			myMesh.indices.push_back(start + 1);
			myMesh.indices.push_back(start + col + 1);
		}

		int lastVertIndex = myMesh.vertices.size() - 1;
		int start = lastVertIndex - col;
		for (unsigned int i = 0; i < col; i++)
		{
			myMesh.indices.push_back(lastVertIndex);
			myMesh.indices.push_back(start + i);
			myMesh.indices.push_back(start + i + 1);
		}

		return myMesh;
	}

	ew::MeshData createSphere(float radius, int subdivisions)
	{
		ew::MeshData myMesh;

		float tStep = (ew::PI * 2) / subdivisions;
		float pStep = ew::PI / subdivisions;

		for (unsigned int row = 0; row <= subdivisions; row++)
		{
			float p = pStep * row;
			for (unsigned int col = 0; col <= subdivisions; col++)
			{
				float t = tStep * col;
				ew::Vertex vert;

				vert.pos.x = radius * cos(t) * sin(p);
				vert.pos.y = radius * cos(p);
				vert.pos.z = radius * sin(t) * sin(p);

				vert.normal = ew::Normalize(vert.pos);

				vert.uv.x = ((float)col / subdivisions);
				vert.uv.y = ((float)row / subdivisions);

				myMesh.vertices.push_back(vert);
			}
		}

		unsigned int indCol = subdivisions + 1;

		for (unsigned int i = 0; i < subdivisions; i++)
		{
			myMesh.indices.push_back(indCol + i);
			myMesh.indices.push_back(i);
			myMesh.indices.push_back(indCol + i + 1);
		}

		for (unsigned int row = 1; row < subdivisions - 1; row++)
		{
			for (unsigned int col = 0; col < subdivisions; col++)
			{
				int start = row * indCol + col;
				myMesh.indices.push_back(start);
				myMesh.indices.push_back(start + 1);
				myMesh.indices.push_back(start + indCol);
				myMesh.indices.push_back(start + indCol);
				myMesh.indices.push_back(start + 1);
				myMesh.indices.push_back(start + indCol + 1);
			}
		}
		
		int pole = myMesh.vertices.size() - indCol;
		int start = pole - indCol;
		for (size_t i = 0; i < subdivisions; i++)
		{
			myMesh.indices.push_back(start + i);
			myMesh.indices.push_back(start + i + 1);
			myMesh.indices.push_back(pole + i);
		}
		return myMesh;
	}
}