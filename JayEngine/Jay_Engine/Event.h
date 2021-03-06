#ifndef __EVENT__
#define __EVENT__

struct Event
{
	enum eventType
	{
		PLAY,
		PAUSE,
		STOP,
		WIN_RESIZE,
		FILE_DROPPED,
		GAME_OBJECT_DESTROYED,
		INVALID
	}type;

	union
	{
		struct
		{
			const char* str;
		}string;
		struct
		{
			int x, y;
		}point2d;
		struct
		{
			void* ptr;
		}pointer;
	};

	Event(eventType type) : type(type)
	{}
};

#endif // !__EVENT__
