AC_INIT([opendnp3], [2.0.1], [jadamcrain@automatak.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_COPYRIGHT([(C) 2010, 2011 Green EneryCorp. (C) 2013-2015 Automatak LLC. All right reserved.])
AM_INIT_AUTOMAKE([subdir-objects])

AC_PREFIX_DEFAULT([/usr]) #works best for Ubuntu
AC_CONFIG_FILES([Makefile])

AC_LANG([C++]) # tell autoconf that we're using C++
AX_CXX_COMPILE_STDCXX_11([noext]) #require support for C++11

LT_INIT #initialize libtools

# optional support for lcov/gcov
AC_ARG_WITH([lcov],
	[AS_HELP_STRING([--with-lcov@<:@=ARG@:>@],
	[lcov support])],
	[CXXFLAGS="--coverage -g -O0 -std=gnu++11"
	AC_MSG_NOTICE([--with-lcov=yes])],
	[AC_MSG_NOTICE([--with-lcov=no])])

AC_OUTPUT #actually output the configuration

