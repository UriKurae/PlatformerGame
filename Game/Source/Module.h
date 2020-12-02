#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class Collider;

class Module
{
public:

	Module() : active(false)
	{}

	void Init(bool _active)
	{
		active = _active;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// Load and save functions for each module
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}
	virtual bool Save(pugi::xml_node&)
	{
		return true;
	}
	
	void Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}


public:

	SString name;
	bool active;

};

#endif // __MODULE_H__