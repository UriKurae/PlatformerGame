//#pragma once
//
//#include "Collider.h"
//#include "Module.h"
//
//
//class Collisions: public Module
//{
//public:
//	// Constructor
//	// Fills all collision matrix data
//	Collisions();
//
//	// Destructor
//	~Collisions();
//
//	// Called at the beginning of the application loop
//	// Removes all colliders pending to delete
//	// Checks for new collisions and calls its listeners
//	bool PreUpdate() override;
//
//	// Called at the middle of the application loop
//	// Switches the debug mode on/off
//	bool Update(float dt) override;
//
//	// Called at the end of the application loop
//	// Draw all colliders (if debug mode is enabled)
//	bool PostUpdate() override;
//
//	// Removes all existing colliders
//	bool CleanUp() override;
//
//	// Adds a new collider to the list
//	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
//
//	// Removes the collider memory and removes it from the colliders array
//	void RemoveCollider(Collider* collider);
//
//	// Draws all existing colliders with some transparency
//	void DebugDraw();
//
//	inline uint GetColliderCount() const { /*return colliderCount;*/ };
//private:
//	// All existing colliders in the scene
//	//Collider* colliders[MAX_COLLIDERS] = { nullptr };
//
//	// The collision matrix. Defines the interaction for two collider types
//	// If set two false, collider 1 will ignore collider 2
//	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
//
//	// The amount of colliders loaded into the array
//	//uint colliderCount = 0;
//
//	// Simple debugging flag to draw all colliders
//	bool debug = false;
//};
//
