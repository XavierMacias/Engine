#ifndef __material_component__
#define __material_component__

#include "Component.h"
#include <string>

class MaterialComponent : public Component
{
private:
	std::string materialName;
	
public:
	MaterialComponent(GameObject* gameObject);
	~MaterialComponent();

	void setMaterial(int material);
	void getMaterial();
};



#endif __material_component__
