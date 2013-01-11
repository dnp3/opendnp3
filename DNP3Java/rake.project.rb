
$options = {
:allowed_platforms => ['pc_cygwin', 'Linux_i686'],
:target => 'libdnp3java.so',
:cc_release_flags => '-O1 -fPIC', #compiling the swig binding at O2 and above was causing programs to seg fault
:includes => [Boost::get_includes_dir, DEFAULT_INCLUDES],
:link_options => [],
:libs => $PLATFORM_LIBS + Boost::get_static_libs + $WINSOCK_LIBS,
:project_libs => [:dnp3, :apl],
:plugins => ['swigjava'], #swigjava specific options
:java_package => 'org.totalgrid.reef.protocol.dnp3', 
:java_outdir => 'src/main/java/org/totalgrid/reef/protocol/dnp3'
}

