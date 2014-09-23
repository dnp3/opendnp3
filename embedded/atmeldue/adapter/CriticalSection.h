
#ifndef CRITICAL_SECTION_H_
#define CRITICAL_SECTION_H_

#include "Resource.h"

class CriticalSection
{
	public:
	CriticalSection(Resource res_);
	~CriticalSection();
	
	private:
	
	Resource res;
	
	CriticalSection() = delete;
	CriticalSection(const CriticalSection&) = delete;
	CriticalSection& operator=(CriticalSection const&) = delete;
};


#endif
