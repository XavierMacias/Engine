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
	
public:
	MaterialComponent(GameObject* gameObject, std::string name);
	~MaterialComponent();

	void SetMaterial(unsigned material) { textureID = material; }
	unsigned GetMaterial() { return textureID; };
	void SetDimensions(int w, int h) { width = w; height = h; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
};



#endif __material_component__
