#include "camera.h"
#include "transform.h"

namespace wr {

	Camera::Camera()
	{
		this->position = ew::Vec3(0,0,5);
		this->target = ew::Vec3(0,0,0);
		this->fov = 60;
		this->aspectRatio = 1080/720;
		this->nearPlane = 0.1;
		this->farPlane = 100;
		this->orthographic = true;
		this->orthoSize = 6;
	}

	Camera::Camera(ew::Vec3 position, ew::Vec3 target, float fov, float aspectRatio, float nearPlane, float farPlane, bool orthographic, float orthoSize)
	{
		this->position = position;
		this->target = target;
		this->fov = fov;
		this->aspectRatio = aspectRatio;
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;
		this->orthographic = orthographic;
		this->orthoSize = orthoSize;
	}

	ew::Mat4 Camera::ViewMatrix() //World->View
	{
		return LookAt(position, target, ew::Vec3(0, 1, 0));
	}
	ew::Mat4 Camera::ProjectionMatrix() //View->Clip
	{
		if (orthographic)
		{
			return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
		}
		else 
		{
			return Perspective(ew::Radians(fov), aspectRatio, nearPlane, farPlane);
		}
	}
}