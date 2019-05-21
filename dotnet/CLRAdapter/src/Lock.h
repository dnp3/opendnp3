#ifndef OPENDNP3CLR_LOCK_H
#define OPENDNP3CLR_LOCK_H

using namespace System::Threading;

private ref class Lock
{
public:
	Lock( Object^ pObject ) : m_pObject( pObject )
	{
		Monitor::Enter( m_pObject );
	}

	~Lock()
	{
		Monitor::Exit( m_pObject );
	}

private:

	Object^ m_pObject;
};

#endif
