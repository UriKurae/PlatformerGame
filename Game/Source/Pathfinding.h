#pragma once
#include "Module.h"
#include "Enemy.h"

#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"

#include "SDL/include/SDL_pixels.h"

class SDL_Texture;
class SDL_Rect;
class SDL_Color;
class TileSet;

class Player;

#define COST_MAP_SIZE	100

class PathFinding: public Module
{
public:

	PathFinding();
	virtual ~PathFinding();

public:

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp();


public:
	void ResetPath(iPoint start);
	void DrawPath(DynArray<iPoint> &path);
	int MovementCost(int x, int y) const;
	DynArray<iPoint>* ComputePath(int x, int y);
	bool IsWalkable(int x, int y) const;
	void PropagateAStar(Player* player);

private:
	PQueue<iPoint> frontier;
	List<iPoint> visited;
	uint newCost;
	iPoint pathGoal;

	List<iPoint> breadcrumbs;
	uint costSoFar[COST_MAP_SIZE][COST_MAP_SIZE];
	DynArray<iPoint> path;

	// L12: AStar (A*) variables
	iPoint goalAStar;			// Store goal target tile
	bool finishAStar = false;	// Detect when reached goal

	SDL_Texture* tileX;
};