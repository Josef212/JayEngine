#ifndef __MODULE_H__
#define __MODULE_H__

#include <string>

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

	bool isEnabled() const
	{
		return enabled;
	}

	void enable()
	{
		if (enabled == false)
		{
			enabled = true;
			start();
		}
	}

	bool disable()
	{
		bool ret = true;
		if (enabled == true)
		{
			enabled = false;
			ret = cleanUp();
		}
		return ret;
	}

	//virtual bool init(FileParser* conf)
	virtual bool init()
	{
		return true; 
	}

	virtual bool start()
	{
		return true;
	}

	virtual update_status preUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status postUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool cleanUp() 
	{ 
		return true; 
	}

	virtual void drawDebug()
	{
	
	}

	virtual void onCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}
};

#endif // !__MODULE_H__