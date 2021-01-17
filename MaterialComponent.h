#ifndef __material_component__
#define __material_component__

#include "Component.h"
#include <string>

class MaterialComponent : public Component
{
	enum MaterialType
	{
		DIFFUSE,

	};
private:
	std::string materialName;
	int width, height;
	unsigned textureID;
	float nSpec = 2.0f;
	float kSpec = 0.04f;
	float kDiff = 0.7f;
	
public:
	MaterialComponent(GameObject* gameObject, std::string name);
	~MaterialComponent();

	void SetMaterial(unsigned material) { textureID = material; }
	unsigned GetMaterial() { return textureID; };
	void SetDimensions(int w, int h) { width = w; height = h; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	float GetNSpec() { return nSpec; }
	float GetKSpec() { return kSpec; }
	float GetKDiff() { return kDiff; }
	void SetNSpec(float n) { nSpec = n; }
	void SetKSpec(float n) { kSpec = n; }
	void SetKDiff(float n) { kDiff = n; }
};



#endif __material_component__
