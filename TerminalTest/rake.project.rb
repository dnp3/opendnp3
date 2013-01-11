
$options = {
:target => 'terminaltest.exe',
:project_libs => [:terminal, :apltesttools, :apl],
:includes => [Boost::get_includes_dir, DEFAULT_INCLUDES],
:libs => Boost::get_static_libs + $PLATFORM_LIBS
}
