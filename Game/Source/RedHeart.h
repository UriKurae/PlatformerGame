#pragma once

#include "Item.h"
#include "Animation.h"

class RedHeart : public Item
{
public:
	RedHeart(iPoint pos);
	~RedHeart();

	bool Start() override;
	bool Update(float dt) override;
	void Draw() override;
	bool CleanUp() override;

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) override;

private:
	Animation idleAnim;
};
