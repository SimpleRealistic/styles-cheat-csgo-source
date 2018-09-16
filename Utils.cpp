#include "Utils.h"
#include "Vector.h"


bool Utils::Clamp(Vector &angles) {
	Vector a = angles;
	Normalize(a);
	ClampAngles(a);

	if (isnan(a.x) || isinf(a.x) ||
		isnan(a.y) || isinf(a.y) ||
		isnan(a.z) || isinf(a.z)) {
		return false;
	}
	else {
		angles = a;
		return true;
	}
}

void Utils::Normalize(Vector& angle) {
	while (angle.x > 89.0f) {
		angle.x -= 180.f;
	}
	while (angle.x < -89.0f) {
		angle.x += 180.f;
	}
	while (angle.y > 180.f) {
		angle.y -= 360.f;
	}
	while (angle.y < -180.f) {
		angle.y += 360.f;
	}
}

void Utils::Normalize(float& angle) {
	while (angle > 180.f) {
		angle -= 360.f;
	}
	while (angle < -180.f) {
		angle += 360.f;
	}
}

void Utils::ClampAngles(Vector &angles) {
	if (angles.y > 180.0f)
		angles.y = 180.0f;
	else if (angles.y < -180.0f)
		angles.y = -180.0f;

	if (angles.x > 89.0f)
		angles.x = 89.0f;
	else if (angles.x < -89.0f)
		angles.x = -89.0f;

	angles.z = 0;
}