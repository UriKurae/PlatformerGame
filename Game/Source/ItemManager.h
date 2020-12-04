#pragma once

#include "Module.h"
#include "Item.h"

#include "List.h"

class SDL_Texture;

class ItemManager : public Module
{
public:
	
	ItemManager();
	~ItemManager();

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	Item* AddItem(ItemType type, iPoint pos);

	void DeleteItem(Item* item);

public:
	SDL_Texture* texture;

private:
	List<Item*> items;
};