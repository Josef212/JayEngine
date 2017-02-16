#ifndef __MODULE__
#define __MODULE__

#include "Globals.h"
#include <string>
#include "Event.h"

class FileParser;
struct PhysBody3D;

class Module
{
private :
	bool enabled;

public:
	std::string name;

	Module(bool startEnabled = true) : enabled(startEnabled)
	{
	}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return enabled;
	}

	void Enable()
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	bool Disable()
	{
		bool ret = true;
		if (enabled == true)
		{
			enabled = false;
			ret = CleanUp();
		}
		return ret;
	}

	virtual bool Init(FileParser* conf)
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool Save(FileParser* sect)
	{
		return true;
	}

	virtual bool Load(FileParser* sect)
	{
		return true;
	}

	virtual void DrawDebug()
	{}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void OnGlobalEvent(const Event& e)
	{}
};

#endif // !__MODULE__