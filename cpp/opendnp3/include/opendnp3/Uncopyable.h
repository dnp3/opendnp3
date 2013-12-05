#ifndef __UNCOPYABLE_H_
#define __UNCOPYABLE_H_

namespace opendnp3
{

/** Inherited classes will not have default copy/assignment.
*/
class Uncopyable
{
protected:
	Uncopyable() {} //allow construction/destruction
	~Uncopyable() {}

private:
	Uncopyable(const Uncopyable&); //prevent copy constructor
	Uncopyable& operator=(const Uncopyable&); //prevent assignment
};

}

#endif

