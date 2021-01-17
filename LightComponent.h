#pragma once
#include "Component.h"
#include "Globals.h"
#include <string>

class LightComponent : public Component
{
private:
	float* ambient_color{ new float[3]{0.4f, 0.4f, 0.4f } };
	float* light_color{ new float[3]{1.0f, 1.0f, 1.0f } };
	float* light_direction{ new float[3]{1.0f, 0.0f, 0.0f } };

public:
	LightComponent(GameObject* gameObject, std::string name);
	~LightComponent();
	void SetAmbientColor(float* rgb) { ambient_color = rgb; }
	void SetLightColor(float* rgb) { light_color = rgb; }
	void SetLightDirection(float* rgb) { light_direction = rgb; }
	float* GetAmbientColor() { return ambient_color; }
	float* GetLightColor() { return light_color; }
	float* GetLightDirection() { return light_direction; }

};

