
#ifndef __CRITICAL_SECTION_H_
#define __CRITICAL_SECTION_H_

class CriticalSection
{
	public:
	CriticalSection();
	~CriticalSection();
	
	private:
	CriticalSection(const CriticalSection&) = delete;
	CriticalSection& operator=(CriticalSection const&) = delete;
};


#endif
