#include "ComponentObject.h"


ComponentObject::ComponentObject()
{
	_enabled = true;
}


ComponentObject::~ComponentObject()
{
}

void ComponentObject::update(float dt)
{
	
}

ComponentObject* ComponentObject::create()
{
	ComponentObject *node = new ComponentObject();
	if (node)
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}
