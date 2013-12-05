#ifndef __STATE_SINGLETON_H_
#define __STATE_SINGLETON_H_

#define MACRO_SINGLETON_INSTANCE(type) \
	private:\
		static type mInstance;\
	protected: \
		type(){}; \
	public:\
		static type* Inst(){ return &mInstance; }

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_NAME_SINGLETON_INSTANCE(type) \
	MACRO_SINGLETON_INSTANCE(type) \
	std::string Name() const { return #type; }
#else
#define MACRO_NAME_SINGLETON_INSTANCE(type) \
	MACRO_SINGLETON_INSTANCE(type)
#endif

#define MACRO_STATE_SINGLETON_INSTANCE(type) MACRO_NAME_SINGLETON_INSTANCE(type)

#endif

