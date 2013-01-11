
if ENV['SH_VERBOSE'] #set on command line with SH_VERBOSE=true to see all subcommands
 verbose(true)
else
 verbose(false)
end

# first thing to do is figure out the platform we're on
$hw_os = case RUBY_PLATFORM.strip
  when /linux/
    # we don't actually care if we are building for i686 or x86_64
    #`uname -sm`.strip.gsub(/\s/,'_')
    # TODO: replace Linux_i686 with just linux in rest of build system
    'Linux_i686'
  when /cygwin/
    'pc_cygwin'
  else
    RUBY_PLATFORM
end


def fix_path(path)
  case $hw_os
  when 'pc_cygwin'
     # need to strip off line ending from cygpath
    path#`cygpath #{path}`.strip
  when 'Linux_i686'
    path
  else
    path
  end
end

def add_projects(projects)

  #get the additional options from the project files
  projects.each do |name, options|
    $options = nil #expect that the require statement sets this variable
    require "./#{options[:dir]}/rake.project.rb"
    unless $options
      puts "Require for #{name} did not set the $options variable"
      Process.exit 1
    end
    options.merge! $options
    options[:target_path] = "#{options[:dir]}/#{$OBJ_DIR}"
  end
  
  
  #Do a final pass to configure the projects now that all the options are in place
  projects.each do |name, options|
  
    if platform_ok options
      
      $pre_build_lambdas.each { |lam| lam.call(name, options) } # give any plugin's pre-builds the chance to add information to options
      $build_lambdas.each { |lam| lam.call(name, options) }     # call all the plugin's build functions
      
    else
    
     puts "Ignoring project #{name} for platform #{$hw_os}"
     projects.delete name
    end
  end
  
  $post_build_lambdas.each { |lam| lam.call }
  
end

def platform_ok(options)
  if options[:allowed_platforms]
    options[:allowed_platforms].include? $hw_os
  else
    true
  end
end

DEFAULT_INCLUDES = []

$pre_build_lambdas = []
$build_lambdas = []
$post_build_lambdas = []
