

jdk_home = if ENV['JDK_HOME']
  ENV['JDK_HOME']
elsif ENV['JAVA_HOME']
  ENV['JAVA_HOME']
else
  nil
end

unless jdk_home and File.directory? jdk_home
  puts 'To use the swig-java plugin, the JDK_HOME or JAVA_HOME environment variables must be set to your JDK directory, comment out the "require \'plugins/swigjava.rb\'" line in rakefile.rb to supress this message.'
else
  jdk_home = File.expand_path jdk_home #this will normailze to linux slashes in case we're using an ENV var from windows
  $jdk_home = jdk_home
  
  puts "Using jdk directory #{$jdk_home}"
  
  platform = case $hw_os #need this to get the correct directory for some of the jni includes
    when 'pc_cygwin'
      'win32'
    else
      'linux'
   end
  
  jdk_includes = ["#{jdk_home}/include","#{jdk_home}/include/#{platform}"]
  
  def add_swig_java_dependencies(name, options)
  
    includes = options[:includes].flatten # these already are paths
    includes << options[:dir] #the project directory should always be explicitly on the include path  
      
    package = if options[:java_package]
                    "-package #{options[:java_package]}"
                  else
                    ''
                  end
                  
    outdir = if options[:java_outdir]
                dir = "#{options[:dir]}/#{options[:java_outdir]}"
                directory dir #create the directory
                dir
              else
                nil
              end
    
    generated_sources = get_generated_swig_java_cpp(options[:dir])
    generated_sources.each do |cppfile|
      ifile = cppfile.ext('.i')
      file cppfile => outdir if outdir
      outdirective = ''
      outdirective = "-outdir #{outdir}" if outdir
      file cppfile => ifile do |t|
        # we need to add this define that gcc normally adds if we are on 64bit
        declares_necessary = []
        declares_necessary << '__x86_64__' if `uname -m`.strip == 'x86_64'
        declares = declares_necessary.collect{|d|"-D#{d}"}.join(' ')
        puts "creating swig cpp/java binding from #{ifile} (#{declares})"    
        sh "swig -c++ -java #{package} #{include_string(includes)} #{outdirective} #{declares} -o #{cppfile} #{ifile}"
      end
    end
    generated_sources
  end
  
  def get_generated_swig_java_cpp(dir)
    FileList["#{dir}/*.i"].ext('.cpp')
  end
  
  $pre_build_lambdas << lambda do |name, options|
    if options[:plugins] and (options[:plugins].include? 'swigjava')
      options[:includes] ||= []
      options[:includes] += jdk_includes  # add the jdk includes so that they are seen by both swig and C++          
    end
  end
  
  $build_lambdas << lambda do |name, options| 
    if options[:plugins] and (options[:plugins].include? 'swigjava')   
      add_swig_java_dependencies(name, options)
    end
  end
end
