
#ifndef RESOURCE_H_
#define RESOURCE_H_

//anything that can be acquired and released
struct Resource
{
	void (*acquire)(void);
	void (*release)(void);
};

#endif
