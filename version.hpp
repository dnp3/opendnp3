#ifndef OPENDNP3_VERSION_HPP
#define OPENDNP3_VERSION_HPP

//  Caution, this is the only header that is guarenteed to change with every
//  release, including this header will cause a recompile every time a new
//  version is released.
//
//  OPENDNP3_VERSION % 100 is the patch level
//  OPENDNP3_VERSION / 100 % 1000 is the minor version
//  OPENDNP3_VERSION / 100000 is the major version

#define OPENDNP3_VERSION 100100

//  OPENDNP3_LIB_VERSION must be defined to be the same as OPENDNP3_VERSION
//  but as a *string* in the form "x_y[_z]" where x is the major version
//  number, y is the minor version number, and z is the patch level if not 0.

#define OPENDNP3_LIB_VERSION "1_1_0"

#endif
