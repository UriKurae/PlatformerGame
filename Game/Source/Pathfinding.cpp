#include "App.h"
#include "Pathfinding.h"
#include "Map.h"
#include "List.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"
#include "Enemy.h"
#include "Textures.h"


PathFinding::PathFinding() : Module()
{
	name.Create("pathfinding");
}

// Destructor
PathFinding::~PathFinding()
{}

bool PathFinding::Awake(pugi::xml_node & conf)
{
	return true;
}

bool PathFinding::Start()
{
	tileX = app->tex->Load("Assets/Textures/Cross.png");
	return true;
}

bool PathFinding::PreUpdate()
{
	return true;
}

bool PathFinding::Update(float dt)
{
	return true;
}

bool PathFinding::PostUpdate()
{
	return true;
}

void PathFinding::Draw()
{
}


bool PathFinding::CleanUp()
{
	return true;
}

void PathFinding::ResetPath(iPoint start)
{
	frontier.Clear();
	visited.Clear();
	breadcrumbs.Clear();

	frontier.Push(start, 0);
	visited.Add(start);
	breadcrumbs.Add(start);

	memset(costSoFar, 0, sizeof(uint) * COST_MAP_SIZE * COST_MAP_SIZE);
}

void PathFinding::DrawPath(DynArray<iPoint> &pathToDraw)
{
	iPoint point;

	// Draw visited
	ListItem<iPoint>* item = visited.start;

	/*while (item)
	{
		point = item->data;
		TileSet* tileset = app->map->GetTilesetFromTileId(1162);
		
		SDL_Rect rec = tileset->GetTileRect(1162);
		iPoint pos = app->map->MapToWorld(point.x, point.y);

		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
	
		item = item->next;
	}*/

	//// Draw frontier
	//for (uint i = 0; i < frontier.Count(); ++i)
	//{
	//	point = *(frontier.Peek(i));
	//	TileSet* tileset = app->map->GetTilesetFromTileId(1161);

	//	SDL_Rect rec = tileset->GetTileRect(1161);
	//	iPoint pos = app->map->MapToWorld(point.x, point.y);

	//	app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
	//}

	// Draw path
	for (uint i = 0; i < pathToDraw.Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(pathToDraw[i].x, pathToDraw[i].y);
		app->render->DrawTexture(tileX, pos.x, pos.y);
	}
}

int PathFinding::MovementCost(int x, int y) const
{
	int ret = -1;

	if ((x >= 0) && (x < app->map->data.width) && (y >= 0) && (y < app->map->data.height))
	{
		int id = app->map->data.layers.start->next->next->data->Get(x, y);

		if (id == 1161)
		{
			ret = 0;
		}
		else if (id == 1170)
		{
			ret = 0;
		}
		else if (id == 1162)
		{
			ret = 0;
		}
		else
		{
			ret = 3;
		}
	}

	return ret;
}

DynArray<iPoint>* PathFinding::ComputePath(int x, int y)
{
	path.Clear();
	iPoint goal = app->map->WorldToMap(x, y);

	int index = 0;
	path.PushBack(goal);
	while (goal != visited.start->data)
	{
		index = visited.Find(goal);
		goal = breadcrumbs[index];
		path.PushBack(goal);
	}

	 path.PushBack(visited.start->data);

	return &path;
}


bool PathFinding::IsWalkable(int x, int y) const
{
	// L10: TODO 3: return true only if x and y are within map limits
	// and the tile is walkable (tile id 0 in the navigation layer)
	ListItem<MapLayer*>* lay = app->map->data.layers.start;
	if (lay->data->Get(x, y) != 0)
	{
		return (x >= 0) && (x <= app->map->data.width) && (y >= 0) && (y <= app->map->data.height);
	}

}

void PathFinding::PropagateBFS(Player* player)
{
	iPoint curr;
	int x = player->GetPosition().x;
	int y = player->GetPosition().y;

	goalAStar = app->map->WorldToMap(x, y);

	while (frontier.Pop(curr))
	{
		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		if (curr == goalAStar)
			break;

		for (uint i = 0; i < 4; ++i)
		{
			if (MovementCost(neighbors[i].x, neighbors[i].y) > 0)
			{
				if (visited.Find(neighbors[i]) == -1)
				{
					frontier.Push(neighbors[i], 0);
					visited.Add(neighbors[i]);

					// L11: TODO 1: Record the direction to the previous node 
					// with the new list "breadcrumps"
					breadcrumbs.Add(curr);
				}
			}
		}
	}
}

void PathFinding::PropagateDijkstra()
{
	// L11: TODO 3: Taking BFS as a reference, implement the Dijkstra algorithm
	// use the 2 dimensional array "costSoFar" to track the accumulated costs
	// on each cell (is already reset to 0 automatically)
	iPoint curr;
	if (frontier.Pop(curr))
	{
		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		for (uint i = 0; i < 4; ++i)
		{
			newCost = costSoFar[curr.x][curr.y] + MovementCost(neighbors[i].x, neighbors[i].y);

			if (MovementCost(neighbors[i].x, neighbors[i].y) > 0)
			{
				if (visited.Find(neighbors[i]) == -1)
				{
					costSoFar[neighbors[i].x][neighbors[i].y] = newCost;
					frontier.Push(neighbors[i], newCost);
					visited.Add(neighbors[i]);
					breadcrumbs.Add(curr);
				}
			}
		}
	}
}

void PathFinding::PropagateAStar(Player* player)
{
	iPoint curr;
	goalAStar = app->map->WorldToMap(player->GetPosition().x, player->GetPosition().y);
	while (frontier.Pop(curr))
	{

		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		//This works if the pathFinding is a while, it's used for early exit when we find the objective.
		if (curr == goalAStar)
			break;

		for (uint i = 0; i < 4; ++i)
		{
			int cost = MovementCost(neighbors[i].x, neighbors[i].y);
			int g = neighbors[i].DistanceManhattan(visited.start->data);
			int h = neighbors[i].DistanceManhattan(goalAStar);
			if (cost > 0)
			{
				cost += costSoFar[curr.x][curr.y];
				if ((visited.Find(neighbors[i]) == -1) || (cost < costSoFar[neighbors[i].x][neighbors[i].y]))
				{
					costSoFar[neighbors[i].x][neighbors[i].y] = cost;

					int f = g + h;
					frontier.Push(neighbors[i], h);
					visited.Add(neighbors[i]);
					breadcrumbs.Add(curr);
				}
			}
		}
	}
}