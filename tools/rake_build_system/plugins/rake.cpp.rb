#Helpers related to C++ development
require File.join(File.dirname(__FILE__), 'rake.cpp.enviroment')

# extract all dependencies from a dependency file, returning an array of dependencies for object file
# corresponding to the input dep file
def dependencies(path)
  deps = []
  open(path) do |file|
    deps = file.read.scan(/ [^\s|\\]+/).collect { |s| s.strip }
  end
  deps.reject!{|s| s.include?("/opt") || s.include?("/usr") || s.include?("include/boost")}
  deps
end

def filtered_dependencies(path)
  deps = dependencies(path)
  deps.reject!{|f| !File.exists? f}
  deps
end

#turn an array of path incldues into a string suitable for g++
def include_string(arr)
 arr.map{ |s| "-I#{s}" }.join(' ')
end

def make_dep_file(source, dep_file, includes)
  inc = include_string(includes)
  sh "#{$CC} -M #{source} #{inc} #{$CC_PREPROCESSOR} > #{dep_file}"
end

def make_obj_file(source, obj_file, includes, cc_flags, warn_flags)
  inc = include_string(includes)
  warn = warn_flags.join(' ')
  cc_joined = cc_flags.join(' ')
  cmd = "#{$CC} #{cc_joined} #{warn} -c -o #{obj_file} #{source} #{inc} #{$CC_PREPROCESSOR}"
  puts "compiling #{source} with #{$CC} #{cc_joined} #{warn} #{$CC_PREPROCESSOR}"
  sh cmd
end

def make_library(lib, obj)
  puts "Creating archive: #{lib}"
  sh "#{$AR} #{lib} #{obj}"
  puts "Indexing archive: #{lib}"
  sh "#{$RANLIB} #{lib}"
end

def make_shared_library(lib, obj, link_options, libs)
  puts "Creating shared library: #{lib}"
  sh "#{$CC} -shared -o #{lib} #{obj} #{link_options.join(' ')} #{libs.join(' ')}"
end

def make_executable(exe,obj,libs)
  puts "Linking: #{exe}"
  libtool_prefix = "libtool --mode=link "
  libtool_prefix = "" if $hw_os == 'pc_linux_arm'
  cc = $CC.gsub('ccache ','')
  sh "#{libtool_prefix}#{cc} #{obj.join(' ')} #{libs.join(' ')} -o #{exe}"
end



def add_cpp_based_target(name, options)

  namespace name do
  
    target_name = options[:target] #the target file that we are creating for this project, (.a, .exe, etc)

    includes = options[:includes].flatten # these already are paths
    includes << options[:dir] #the project directory should always be explicitly on the include path

    includes.each do |path|
      unless File.directory? path
        puts "In project #{name}, include directory does not exist: #{path}"
      end
    end

    excludes = options[:cppexcludes] || [] #some source files may be excluded from the build
    
    # override the source extension for cpp using the :src_ext option
    src_ext = 
      if options[:src_ext]
        options[:src_ext]
      else
        '.cpp'
      end
        
    dir = options[:dir]
    obj_dir = "#{dir}/#{$OBJ_DIR}"          #directory where the objects, dep files, and libraries go
    target = "#{obj_dir}/#{target_name}"    #full path the the target

    src_list = FileList["#{dir}/**/*#{src_ext}"].reject { |f| excludes.include? f.pathmap('%f') }

    # the project may have a way to create dyanmic source code from other dependencies      
    if options[:dynamic_src]
      options[:dynamic_src].each do |f|
        src_list << f unless src_list.include? f
      end
    end

    #create a map from source to object files
    obj_to_src = src_list.inject({}) do |sum, src|     
      sum.merge! src.pathmap("#{obj_dir}/%f").ext('.o') => src
    end
                  
    directory obj_dir # task for directory
    file target => obj_dir #the library depends on the obj directory being created

    cc_flags = (ENV['debug'] || options[:cc_release_flags].nil?) ? $CC_FLAGS : options[:cc_release_flags]

    warn_flags = []
    warn_flags.concat($WARN_FLAGS)
    warn_flags.concat(options[:warn_flags]) if options[:warn_flags]

    #explicitly declare dependencies between .o and source files
    obj_to_src.each do |obj, src|
      dep_file = obj.ext('dep')
      file obj => src do |t|
        make_dep_file src, dep_file, includes
        make_obj_file src, t.name, includes, cc_flags, warn_flags
      end
    end

    desc 'Remove and regenerate dependency files'
    task :updatedep do
      `rm #{obj_dir}/*.dep`
      obj_to_src.each do |obj, src|
        puts "Generating dependencies for #{source}"
        make_dep_file src, obj.ext('.dep'), includes
      end
    end

    task :build => :dependencies

    task :dependencies => src_list do
      print "loading existing dependencies for #{name}... "
      obj_to_src.keys.each do |obj|
        dep = obj.ext('.dep')
        file obj => filtered_dependencies(dep) if File.exists? dep
      end
      puts 'done.'
    end

    # should generate the rule for building the target
    yield target, obj_to_src, includes

    desc "Build and archive the #{name} library"
    task :build => target

    desc "Clobber the #{name} project"
    task :clobber do |t|
      puts "clobber1: rm -R -f #{obj_dir}"
      sh "rm -R -f #{obj_dir}"
    end

    desc "Copy the target"
    task :copytarget, [:dest, :name, :sudo] => target do |task, args|
      cmd = "cp #{target} #{args[:dest]}/#{args[:name]}"      
      cmd = "sudo " + cmd if args[:sudo]
      sh "#{cmd}"      
    end
    
    desc "Run remote tests"
    task [:testremote, :host, :user, :passwd, :xml] => target do |task, args|
      #require here so that we don't blow up w/o net-ssh/openssl
      require File.join(File.dirname(__FILE__), 'rake.crosstest.rb')
      CrossTest::do_test_sequence(args[:host], args[:user], args[:passwd], target, args[:xml])
    end

  end #end namespace
end #end method

def add_static_lib(name, options)

  add_cpp_based_target(name, options) do |target, obj_to_src, includes|

      file target => obj_to_src.keys do |t| #rule for making the target
        make_library target, obj_to_src.keys.join(' ')
      end

  end

end #end method

def add_shared_lib(name, options)

  dep_libs = get_dep_libs(name, options)
    
  link_options = []
  link_options = options[:link_options] if options[:link_options]

  add_cpp_based_target(name, options) do |target, obj_to_src, includes|
      file target => dep_libs
      dep_libs += options[:libs] if options[:libs]
      file target => obj_to_src.keys do |t| #rule for making the target
        make_shared_library target, obj_to_src.keys.join(' '), link_options, dep_libs
      end
  end

end #end method

def get_dep_libs(name, options)

  if options[:project_libs]
    options[:project_libs].collect do |l|
      proj = $projects[l]
      if $projects[l]
       "#{$projects[l][:dir]}/#{$OBJ_DIR}/#{$projects[l][:target]}"
      else
        puts "Unknown dependency #{l} in #{name}"
        exit -1        
      end      
    end
  end
end

def add_executable(name, options)

  dep_libs = get_dep_libs(name, options)

  add_cpp_based_target(name, options) do |target, obj_to_src, includes|

    file target => dep_libs  #force a relink if a lib changes
    task :build => options[:project_libs].collect { |libname| "#{libname}:build" } #force a load of the dependencies for the sub-libraries

    task :run => :build

    desc 'runs the executable with optional cmd line arguments'
    task :run, [:cmdline] => target do |task, args|
      puts "#{target} #{args[:cmdline] ? args[:cmdline] : ''}"
      sh   "#{target} #{args[:cmdline] ? args[:cmdline] : ''}"
      run_coverage( name, options) if $RELEASE_TYPE == 'coverage'
    end

    desc 'blindly run the executable -- no build dependency'
    task :runblind, [:cmdline] => target do |task, args|
      puts "#{target} #{args[:cmdline] ? args[:cmdline] : ''}"
      sh   "#{target} #{args[:cmdline] ? args[:cmdline] : ''}"
      run_coverage( name, options) if $RELEASE_TYPE == 'coverage'
    end

    file target => obj_to_src.keys do |t|
      make_executable target, obj_to_src.keys, (dep_libs + options[:libs] + $WINSOCK_LIBS)
    end

  end

end

# Generate code coverage results.
def run_coverage( name, options)
  dep_dirs = options[:project_libs].collect{ |lib| $projects[lib][:dir] + "/#{$hw_os}/#{$RELEASE_TYPE}" }
  lcov_dir  = "#{$projects[name][:dir]}/#{$hw_os}/#{$RELEASE_TYPE}"
  lcov_out  = "#{lcov_dir}/lcov.out"
  if ENV['LCOV_HTML']
    lcov_html = ENV['LCOV_HTML'] + "/#{lcov_dir}"
    sh "mkdir -p #{lcov_html}"
  else
    lcov_html = "#{lcov_dir}/html"
  end

  puts "lcov -d #{$projects[name][:dir]}/#{$hw_os}/#{$RELEASE_TYPE} #{dep_dirs.collect{|x| ' -d '+x}} -b #{Dir.pwd} -c -o #{lcov_out}"
  sh   "lcov -d #{$projects[name][:dir]}/#{$hw_os}/#{$RELEASE_TYPE} #{dep_dirs.collect{|x| ' -d '+x}} -b #{Dir.pwd} -c -o #{lcov_out}"

  Dir.mkdir(lcov_html) unless File.directory? lcov_html
  puts "genhtml -o #{lcov_html} #{lcov_out}"
  sh   "genhtml -o #{lcov_html} #{lcov_out}"

  puts "\nHTML coverage results are at:\n#{lcov_html}/index.html"

  # Add a symbolic link for gcov. Bitten will pass "coverage" as the prefix.
  dir  = $projects[name][:dir]
  link = "#{$RELEASE_TYPE}-#{dir}"
  sh "ln -s #{dir}/#{$hw_os}/#{$RELEASE_TYPE} #{link}" unless File.exists? link
  # Add symbolic links for dependencies (for gcov again).
  dep_dirs = options[:project_libs].each do |lib|
     dir  = $projects[lib][:dir]
     link = "#{$RELEASE_TYPE}-#{dir}"
     sh "ln -s #{dir}/#{$hw_os}/#{$RELEASE_TYPE} #{link}" unless File.exists? link
  end

end


def create_header_only_distribution(name, options)

  namespace name do

    task :collect_headers do
      options[:headers].each do |project, files|

        # we need to recreate the right directories so the includes <PSI/ will work
        source_dir_name = $projects[project][:dir].split[-1]
        local_target_dir = "#{options[:dir]}/#{source_dir_name}"

        # clear and recreate the output dir
        FileUtils.rm_rf(local_target_dir) if File.exists?(local_target_dir)
        Dir.mkdir(local_target_dir)


        if options[:header_file]
          license_text = File.open("#{options[:dir]}/#{options[:header_file]}").read
          files.each do |h|
            f = File.open("#{local_target_dir}/#{h}", 'w')
            f.write license_text
            f.write File.open("#{$projects[project][:dir]}/#{h}").read
            f.close
          end
        else
          headers = files.collect{|f|"#{$projects[project][:dir]}/#{f}"}
          FileUtils.cp headers, local_target_dir
        end
      end
    end

    obj_dir = "#{options[:dir]}/#{$OBJ_DIR}"            #directory where the objects, dep files, and libraries go
    directory obj_dir

    task :collect_libs => obj_dir
    task :collect_libs => options[:project_libs].collect{|l| "#{l}:build"}
    task :collect_libs do
      # go and collect the static libraries from the other projects
      options[:project_libs].each do |l|
        sh "cp #{$projects[l][:dir]}/#{$OBJ_DIR}/#{$projects[l][:target]} #{obj_dir}"
      end
      # copy the static libraries but ignore the dynamic ones (Ex: -lpthread)
      options[:libs].reject{|l| l =~ /^-[lL]/}.each do |l|
        sh "cp #{l} #{obj_dir}"
      end
    end

    task :build => :collect_libs

    task :run_pre_package_commands => [:collect_headers] do
      Dir.chdir(options[:dir]) do
        options[:pre_package_commands].each do |command_hash|
          command_hash.each do |command, args|
            # if the command is a symbol and a project name we replace it with that projects target exe name
            command = "#{$OBJ_DIR}/#{$projects[command][:target]}" if command.is_a?(Symbol) and $projects[command]
            sh "#{command} #{args}"
          end
        end
      end
    end

    desc "Make a package of the headers and libraries for distribution"
    task :package => [:build, :run_pre_package_commands] do
      Dir.chdir(options[:dir]) do
        sh "tar -czvf #{options[:target]} #{options[:files_to_package].join(' ')}"
      end
    end

    desc "Clobber the #{name} project"
    task :clobber do |t|
      puts "clobber2: rm -R -f #{obj_dir}"
      sh "rm -R -f #{obj_dir}"
    end

  end
end #end method


def add_cpp_dependecies(name, options)
  case options[:target]
    when /\.tar\.gz/
      create_header_only_distribution name, options
    when /\.a$/
      add_static_lib name, options
    when /\.exe$/
      add_executable name, options
    when /\.so$/
      add_shared_lib name, options
  end
end

def add_global_cpp_tasks

  desc 'Creates all libraries and executables'
  task :default => $projects.collect { |name, opt| "#{name}:build" }

  desc 'Updates all C++ dependencies'
  task :updatedep => ($projects.collect { |name, opt| "#{name}:updatedep" })

  desc 'Clobbers all of the build targets'
  task :clobber => $projects.collect { |name, opt| "#{name}:clobber" }
  
end


$build_lambdas << lambda { |name, options| add_cpp_dependecies(name,options) }
$post_build_lambdas << lambda { add_global_cpp_tasks }
