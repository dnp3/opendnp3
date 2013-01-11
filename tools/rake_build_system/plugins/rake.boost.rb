
class Boost

#names of the various boost libraries
BOOST_SYSTEM_NAME = "boost_system"
BOOST_DATE_TIME_NAME = "boost_date_time"
BOOST_PROGRAM_OPTIONS_NAME = "boost_program_options"
BOOST_UNIT_TEST_NAME = "boost_unit_test_framework"

BOOST_LIB_NAMES = [BOOST_SYSTEM_NAME, BOOST_DATE_TIME_NAME, BOOST_PROGRAM_OPTIONS_NAME, BOOST_UNIT_TEST_NAME]

#figure out where the tools directory is
def self.get_tools_home  
    if ENV['TOOLS_HOME']      
      fix_path(ENV['TOOLS_HOME'].gsub(/\\/,'/'))            
    else
      abort 'The TOOLS_HOME environment variable is not set. On windows, set this in Control Panel -> System -> Advanced -> Environment variables. On linux add the following to the end of your ~/.bashrc export TOOLS_HOME=~/tools'
    end  
end

def self.get_boost_version
  if ENV['BOOST_VERSION']
    ENV['BOOST_VERSION']
  else
    abort 'Environment variable BOOST_VERSION must be set in your main rakefile or OS environment. Format is booost_1_xx'
  end
end

def self.get_includes_dir
  puts get_tools_home
  "#{get_tools_home}/boostlib/#{get_boost_version}/include".gsub(/\\/,'/')
end

def self.get_libs_dir
  __FILE__.pathmap("#{get_tools_home}/boostlib/#{get_boost_version}/#{$hw_os}")
end

def self.get_shared_libs
  ["-L#{get_libs_dir}"] + BOOST_LIB_NAMES.collect { |lib| "-l#{lib}" }
end

def self.get_static_libs
  BOOST_LIB_NAMES.collect { |lib| "#{get_libs_dir}/lib#{lib}.a" }
end

end
