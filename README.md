Copyright (c) 2010, 2011 Green Energy Corp.
Copyright (c) 2013 - 2014 Automatak LLC

Licensed under the terms of the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0.html) License v2.0.

Documentation
=============

Links to generated API documentation can be found on the [project homepage](http://www.automatak.com/opendnp3)
  
Companion documentation is in PDF format.

Overview
========

OpenDNP3 is a portable, scalable, and rigorously tested implementation 
of the DNP3 (www.dnp.org) protocol stack written in C++11. The library 
is optimized for the largest front end processor implementations
and outstation device simulations, although it performs very well on 
embedded linux ARM.

Idiomatic bindings are available for the .NET and Java.

Dependencies
============

The core 'opendnp3' library has no external dependencies. It uses an abstract Platform Abstraction Layer (PAL) that could
be implemented for other platforms. The project 'asiopal' implements the PAL using the stand-alone version of [ASIO](http://think-async.com/). This library is header-only.

[Catch](https://github.com/philsquared/Catch) is the unit testing framework and is distributed with the project.

