#!/usr/bin/env ruby

require 'erb'

def get_files(pattern)    
  Dir.glob(pattern).join(" \\\n")
end

# library cpp
openpal_sources = get_files('cpp/libs/openpal/src/openpal/**/*.cpp')
asiopal_sources = get_files('cpp/libs/asiopal/src/asiopal/*.cpp')
opendnp3_sources = get_files('cpp/libs/opendnp3/src/opendnp3/**/*.cpp')
asiodnp3_sources = get_files('cpp/libs/asiodnp3/src/asiodnp3/**/*.cpp')

# tests
openpal_test_sources = get_files('cpp/tests/openpaltests/src/*.cpp')
opendnp3_test_sources = get_files('cpp/tests/opendnp3tests/src/*.cpp')

# includes 
installed_includes = get_files('cpp/libs/openpal/src/openpal/**/*.h')

template_string = File.read('Makefile.am.erb')
template  = ERB.new template_string
File.write('Makefile.am', template.result)
