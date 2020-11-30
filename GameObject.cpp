#include "GameObject.h"

GameObject::GameObject()
{
    name = "";
}

GameObject::~GameObject()
{
    childrens.clear();
}