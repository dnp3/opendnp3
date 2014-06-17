Change Log
==============

### 1.1.1-RC5 ###
- Fixed reference to --with-jni-include-path in configure.ac

### 1.1.1-RC4 ###
- Added on-demand integrity scans to C++ and Java API

### 1.1.1-RC3 ###
- Fixed bug where master task scheduler could get hung if a connection
  is closed while the stack is waiting for a response
- Added support for VS2013

### 1.1.1-RC2 ###
- Fixed conformance issue with unsolicited
- Fixed a bug where slave would cache deferred requests even after a
  layer close
- Added tests for unsol/read collisions
- Added debug logging for master state transitions
- Simplified LogAdapter to resolve Tomcat issues
- Cache LogEntry clazz/constructor for Java

### 1.1.1-RC1 ###
- Simplified LogAdapter to resolve Tomcat issues
- Cache LogEntry clazz/constructor for Java
- Bumped Debian changelog (Issue #35)
- Fixed typing issues caught by g++
- Split wall-time and monotonic-time into 2 interfaces as a stop-gap fix
- Time source now uses system clock, not the timer clock
- Made maxRequestFragmentSize public (Issue #26)
- Added libopendnp3-java-demos package (Issue #22)
- Added JARs to libopendnp3-java DEB package (Issue #22)
- Need ```addClasspath=true``` in pom.xml
- Added .gitignore to hide _target_ build directories
- Split Java demos into separate POM containers (Issue #23)
- Added libopendnp3-java Debian package support (Issue #22)
- Removing C++ demo autoconf/automake files
- Added support for C++ components (Issue #22)
- Added files to support building on a Debian system
- Added pthreads checks and increased compile verbosity (Issue #21)
- Fixed various compiler warnings shown with ```-Werror```
- Fixed boolean logic (Issue #19)
- Build C++ demos as part of the main build effort (Issue #21)
- Detect need for OPENDNP3_BOOST_TIMER_PATCH (Issue #19)
- Revised Java detection methodology (Issue #20)
- Reformatted autoconf/automake template files
- Changed include guard in Types.h to avoid libc collision
- Fixed file permissions on cpp and java sources
- Added a missing conf option
- Made the default class assignment CLASS1 for .NET bindings

### 1.1.0 ###
- Updated Java POMs for 1.1.0 release
- Fixed SCM URL in pom.xml
- Link buffer now resets state on close
- Finished applying Apache license to all files
- Restored GCC 4.6/4.7 monotonic vs steady clock handling
- Tweaks to make log entry's high resolution system time
- Fixed issue with std::thread::hardware_concurrency() returning 0
- Fixed a bug that was screwing up Java class assignment
- Fixed several conformance issues
- Fixed a bug in the outstation where parameters were being permanently
  set
- Fixed quality bug introduced with auto-for loop
- Removed spurious test output

### 1.0.0 ###
- Migrated to Boost 1.47.0
- Added Virtual Terminal Object (VTO) support with standard
  implementation and experimental stream support
- Migrated all external numeric types to use boost type definitions for
  interoperability
- Added autotools support thanks to Chris Verges - Rake may be replaced
  entirely with autotools in a future release
- Added Astyle code formatter to project build systems
- Master/Slave can now both notify of their current state using an enumeration
- TestSet includes address scanning feature to detect devices that don't
  support broadcast addressing
- sub-Masters on same channel can now use same address if talking to
  different slaves
- Log messages can now contain well known key-value pairs
- Management class 'AsyncStackManager' now automatically runs when
  stacks are added, no Start() call
- AsyncStackManger blocks when RemovePort() or RemoveStack() are called
  until the operation has completed
- Fixed bugs: 18, 15, 9, 7

### 0.9.4 ###
- Rebased java namespace to org.totalgrid.reef.protocol.dnp3
- Java SO static loading can be overriden using system property
  ```org.totalgrid.reef.protocol.dnp3.nostaticload```
- Fixed a major bug in slave unsolcited mode on event buffer overflow.

### 0.9.3 ###
- Fixed parsing bug in testset that caused erroneous pulse repititions
- Fixed a bug in DataPoll.cpp that caused stack to crash if it eas
  configured with an empty exception scan
- Added an address scanning feature to the TestSet
- Removed ASIOIncludes.h and replaced with preprocessor defines for all
  platforms.
- Fix for issue #9
- Jar now require shared library to be install on java.library.path
