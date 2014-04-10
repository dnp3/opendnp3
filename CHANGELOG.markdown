### 2.0.0-M3 ###

## Refactoring / Improvements  - General ##
* To enable ports to other platforms and MCUs, the 1.1.x library was divided into 4 sub-libraries.
  * openpal - The Platform Abstraction Layer (PAL) provides abtract base classes for key abstractions like IPhysicalLayer, IExecutor, ITimer, etc.
  * opendnp3 - The core dnp3 library. It only depends on openpal and is platform independent C++11.
  * asiopal - Implements the PAL using boost::asio.  This PAL can be used on server platforms like Windows, Linux, & OSX. It only depends on openpal.
  * asiodnp3 - Provides a nice wrapper to opendnp3 for server platforms. Depends on all three libraries.

* Library uses a new parser and generator that vastly improves readability, maintainability, performance, and safety.
* Example applications for Atmel Studio for Arduino boards have been added. They are not yet production ready, but a proof of concept.
* Stack depth has been reduced to better suite embedded systems.
* Core library is now organized by layer making navigation, maintainence, and learning the library easier.
* All libraries are now exception-free. They can be compiled with -fno-exceptions.
* IExecutor now uses a "Runnable" which uses type-safe erasure/lambdas combined with static allocation
* Core library now uses sprintf instead of iostreams. All usage of std::string removed to improve portability.
* Removed some instances of recursion in the link layer CRC checking.
* The library no longer uses BOOST. Instead it uses the standalone verison (header only) of [ASIO](http://think-async.com/)
* Tests were ported from BOOST_TEST to [Catch](https://github.com/philsquared/Catch). Catch is header only and redistributed with the project.
* Core library no longer uses STL. This makes ports to MCUs much easier.
* Core library no longer uses dynamic memory allocation. Databases and buffers are either statically allocated or injected allowing the user to choose.
* Library no longer uses size_t as this type changes length on 32/64-bit platforms. Explicit types are now used from <cstdint>.
* Application layer objects and most enums are now generated using a companion Scala project in the 'generation' folder.
* Buffer wrappers "WriteBuffer" and "ReadOnlyBuffer" are now passed around internally instead of pairs of uint8_t* and uint32_t.
* Upgraded solution file to VS2013
* x86 and x64 outputs now build side-by-side
* LogEntry class no longer carries timestamp. This allows the timestamp to be set by the specific platform.
* Setpoint/Control terminology changed to AnalogOutput & BinaryOutput to correspond to the specification.
* Libraries no longer have separate src/include directories. This decision was premature.
* Clang is now an officially supported compiler and will also be used for static analysis. Project compiles with Clang 3.4.
* Logging is now explicitly controlled using the bitfields instead of enums. This allows dnp3 specific log levels to be defined without polluting openpal with their definitions.

## External Features ##

* Ticket 56 - Measurement types can now tell the user if the timestamp is valid or not (bool IsTimeValid() const)
* Ticket 43 - Master command callbacks now provide a type that can distinguish between local and remote failure, e.g. when the repsonse times out or the channel is closed.
* Ticket 47 - TCP client can now perform asynchronous DNS host resolution and try mutliple resolved addresses in a single "open" sequence.
* Ticket 56 - C++ channels can also specify a "strategy" for full control over the wait period for retry.
* Ticket 30 - Outstation API now provides control of the NeedTime IIN bit.
* Ticket 29 - Scans are now exposed as part of the master API. Callbacks can be registered to observe success and failure. Class scans may be defined dynamically and performed on demand.
* C++ TCP Client/Server can now configure native socket types with an optional functor. This allows platform-specific customization of things like TCP keep-alives.
* Master applications now specify "ISOEHandler" (Sequence of Events) instead of "IDataObserver". It allows users to discern between static and event types.
* Renamed outstation API "IDataObserver" to the more descriptive "IMeasurementLoader"
* Outstation now uses a simple interface "ITimeWriteHandler" to receive time write requests
* The start of a master or outstation is delayed until a Enable() method is called. Stacks may be dynamically paused/resumed at runtime with the Enable/Disable methods.
* Master now supports additional types:
  * Frozen Counters, Double-bit Binaries, and Octet Strings
* Outstation now supports additional types:
  * Frozen Counters, Double-bit Binaries, and AO/BO status events
* Java Future can now be called with an optional timeout
* Channels now have expoential backoff capability controlled by min/max retry parameters. 
* Each channel can low control the its log level dynamically via the SetLogFilters() method.

## Bug / conformance fixes ##
* Default integrity poll is now a Class 1/2/3/0 scan as per the specification.
* Outstation now reports events in correct order-of-occurence across all seven types it supports.
* Physical layer disconnects now observer the minRetry parameter. This prevents rapid connect/disconnect cycling when a server is closing connections.
* Transport layer can now receive non-fin packets with payload length between 1 and 249. It's very strange for an implementation to do this, but its actually in the spec.
* Count-of-zero headers are now explicitly disallowed and parsing is halted.
* Ticket 57 - When a READ is received while waiting for a confirm, any further retries should be canceled. (also fixed on 1.1.x)
* Ticket 34 - The transport layer now passes the link layer an "IBufferSegment". This allows an entire APDU to be written to the physical layer if unconfirmed data mode is used.
* Ticket 62 - Fixed a bug where the master scheduler could hang if the the channel was closed while waiting for response to a command.

## Removed ##
* VTO "routers" are now longer provided. 0-length strings of all types (Groups 110-113) are now explicitly not allowed.
* ASIOPAL now explicitly uses std::chrono::steady_clock for timers. There is no longer a typedef.
* Library no longer supports 4-octet qualifier codes. All indices used uint16_t as underlying type.
* Support for "delta" values has been removed. New implementations do not need to support this according to the spec.
