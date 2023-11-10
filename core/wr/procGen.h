#pragma once
#include "../ew/mesh.h"
namespace wr {
	ew::MeshData createSphere(float radius, int subdivisions);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions);
}
