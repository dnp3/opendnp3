#ifndef __LOCATION_H_
#define __LOCATION_H_

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifndef OPENDNP3_SUPPRESS_LOG_LOCATION
#define LOCATION __FILE__ "(" TOSTRING(__LINE__) ")"
#else
#define LOCATION ""
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_EXCEPTION(type, message) { throw type(LOCATION, message); }
#else
#define MACRO_THROW_EXCEPTION(type, message) throw type(LOCATION, "");
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_INDEX_OUT_OF_BOUNDS(index) { throw openpal::IndexOutOfBoundsException(LOCATION, index); }
#else
#define MACRO_THROW_INDEX_OUT_OF_BOUNDS(index) { throw  openpal::IndexOutOfBoundsException(LOCATION, index); }
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_EXCEPTION_WITH_CODE(type, message, code) { throw type(LOCATION, message, code); }
#else
#define MACRO_THROW_EXCEPTION_WITH_CODE(type, message, code) throw type(LOCATION, "", code);
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_EXCEPTION_COMPLEX(type, message) {\
	std::ostringstream oss;\
	oss << message;\
	throw type(LOCATION, oss.str()); }
#else
#define MACRO_THROW_EXCEPTION_COMPLEX(type, message) throw type(LOCATION, "");
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_EXCEPTION_COMPLEX_WITH_CODE(type, message, code) {\
	std::ostringstream oss; \
	oss << message; \
	throw type(LOCATION, oss.str(), code); }
#else
#define MACRO_THROW_EXCEPTION_COMPLEX_WITH_CODE(type, message, code) throw type(LOCATION, "", code);
#endif

#endif
