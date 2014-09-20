
#include "CriticalSection.h"

CriticalSection::CriticalSection(Resource res_) : res(res_)
{
	res.acquire();
}

CriticalSection::~CriticalSection()
{
	res.release();
}
	

