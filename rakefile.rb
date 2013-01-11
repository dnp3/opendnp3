
$: << './tools/rake_build_system' #make sure this rakefile can see all the build system files

ENV['BOOST_VERSION']='boost_1_52_0'

require 'rake/clean'
CLOBBER.include('doc')					# remove any doxygen-generated files

require 'rake.environment.rb'           # this sets up a slew of environment constants
require 'plugins/rake.cpp.rb'           # this provides the facilities for declaring C++ style builds
require 'plugins/rake.boost.rb'         # provides location and names of the boost libraries
require 'plugins/rake.swigjava.rb'      # provides helpers for creating cpp/java bindings using swig
require 'plugins/rake.source.rb'        # provides licensing helpers

DEFAULT_INCLUDES << __FILE__.pathmap('%d')

$DNP3_JAVA = !ENV['java'].nil?

#format is name => options, :dir is required
$projects = {
:apl => {:dir => 'APL' },
:testapl => {:dir => 'TestAPL'},
:apltesttools => {:dir => 'APLTestTools'},
:dnp3 => {:dir => 'DNP3'},
:dnp3test => {:dir => 'DNP3Test'},
:slavedemo => {:dir => 'demos/slave-cpp'},
:masterdemo => {:dir => 'demos/master-cpp'},
}

$projects[:dnp3java] = {:dir => 'DNP3Java'} if $DNP3_JAVA

add_projects($projects) #removes projects that are not valid for $hw_os

SOURCE_PROJECTS = [:apl, :testapl, :apltesttools, :dnp3, :dnp3test]
SOURCE_DIRS = SOURCE_PROJECTS.collect { |p| $projects[p][:dir] }

desc 'Generate doxygen html docs for the project'
task :document do
  `doxygen ./config/doxygen.config`
end

desc 'Generate a sloccount report for the project'
task :sloccount do
  dirs = SOURCE_DIRS + ['tools/rake_build_system']
  `sloccount --wide --details #{dirs.join(' ')} > sloccount.sc`
end



