#pragma once
#include <iostream>
#include <string>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
private:
	std::string name;
	GameObject* parent;
	std::vector<GameObject*> childrens;
};

