#pragma once

#include "Item.h"
#include "Animation.h"

class GreenGem : public Item
{
public:
	GreenGem(iPoint pos);
	~GreenGem();

	bool Start() override;
	bool Update(float dt) override;
	void Draw() override;
	bool CleanUp() override;

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) override;

private:
	Animation idleAnim;
};