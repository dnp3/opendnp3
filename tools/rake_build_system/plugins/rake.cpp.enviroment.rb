
# when buidling for ARM we have to modify PATH to point first to the cross compiler
if ENV['arm']
  ENV['PATH'] = case $hw_os
    when 'pc_cygwin'
      '/opt/crosstool/gcc-3.3.4-glibc-2.3.2/arm-unknown-linux-gnu/arm-unknown-linux-gnu/bin/:/opt/crosstool/gcc-3.3.4-glibc-2.3.2/arm-unknown-linux-gnu/bin/:' + ENV['PATH']
    when 'Linux_i686'
      '/usr/local/opt/crosstool/arm-linux/gcc-3.3.4-glibc-2.3.2/bin:/usr/local/opt/crosstool/arm-linux/gcc-3.3.4-glibc-2.3.2/arm-linux/bin:' + ENV['PATH']
    else
      puts "Don't know what cross-compiler to use for #{$hw_os}"
      Process.exit(1)
    end
  $hw_os = 'pc_linux_arm'
end

# defaults for compiler, archiver, and indexer
$CC = 'g++ -std=c++0x'

$AR = 'ar cruv'
$RANLIB = 'ranlib'
$LD = 'ld'

COMPILER_VERSION = case $hw_os
 when 'pc_cygwin'
  '-gcc34'
 when 'Linux_i686'
  '-gcc41'
 when 'pc_linux_arm'
  '-gcc33'
end

preprocessor = case $hw_os
 when 'pc_cygwin'
  ['_WIN32_WINNT=0x0501', '__USE_W32_SOCKETS']
 when 'pc_linux_arm'
  ['ARM','BOOST_ASIO_DISABLE_EPOLL']
 else []
end

preprocessor += ['BOOST_ASIO_ENABLE_CANCELIO', 'BOOST_REGEX_NO_LIB','BOOST_LIB_DIAGNOSTIC']

if ENV['PREPROCESSOR']
  preprocessor << ENV['PREPROCESSOR']
end

$CC_PREPROCESSOR = preprocessor.collect { |d| "-D#{d}"}.join(' ')

$WINSOCK_LIBS = case $hw_os
  when 'pc_cygwin'
    ['/lib/w32api/libwsock32.a','/lib/w32api/libws2_32.a']
  else
    []
end

def get_release_cc_flags
  case $hw_os
    when 'pc_linux_arm'
      ['-O3']
    when 'pc_cygwin'       
      ['-O3']
    else
      ['-O3 -fpic']
  end	
end

def get_debug_cc_flags
  case $hw_os
    when 'pc_linux_arm'
      []
    else
      ['-g -fpic']
  end	
end

def get_coverage_cc_flags
  case $hw_os
    when 'pc_linux_arm'
      []
    else
      ['-g -fPIC -O0 -fprofile-arcs -ftest-coverage -DNDEBUG -DPSI_LOGALL']
  end	
end

#By default, the build is set to debug
$WARN_FLAGS   = ['-Wall', '-Wno-strict-aliasing']
$RELEASE_TYPE = ENV['debug'] ? 'debug' : (ENV['coverage'] ? 'coverage' : 'release')
$CC_FLAGS     = case $RELEASE_TYPE
  when 'release'
    get_release_cc_flags()
  when 'debug'
    get_debug_cc_flags()
  when 'coverage'
    get_coverage_cc_flags()
  else
    puts "Unknown $RELEASE_TYPE = #{$RELEASE_TYPE}"
    Process.exit 1
end

$OBJ_DIR = "#{$hw_os}/#{$RELEASE_TYPE}"

$PLATFORM_LIBS = case $hw_os
  when 'pc_cygwin'
    ['-lpthread']
  when 'Linux_i686'
    ['-lpthread']
  when 'pc_linux_arm'
    ['-lpthread']
  else
    []
end

RUBY_INC = case $hw_os
  when 'pc_cygwin'
    '/lib/ruby/1.8/i386-cygwin'
  when 'Linux_i686'
    '/usr/lib/ruby/1.8/i486-linux/'
end

$PLATFORM_LIBS << '-lgcov' if $RELEASE_TYPE == 'coverage'
