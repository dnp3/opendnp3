#ifndef __VISIBILITY_H_
#define __VISIBILITY_H_

#ifdef OPENPAL_LIMIT_VISIBILITY
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define  __attribute__ ((visibility ("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif

#endif // end of guards

