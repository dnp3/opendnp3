
def get_files_to_process(root, extensions)
  files = []
  extensions.each do |ext|
    files.concat(FileList["#{root}/**/*#{ext}"])
  end  
  files
end

def get_license_text(license, comment)
  license_text = File.foreach(license).collect do |line| 
    comment + line
  end.join
end

def add_license(dirs, options, license)
 
  files = dirs.inject([]) do |sum,dir|
    sum += get_files_to_process(dir, options[:exts])
  end
  
  puts "adding #{license} from the #{files.size} files matching #{options[:exts].join(', ')}"
  
  license_text = get_license_text(license, options[:comment])
  
  first_line = license_text.lines.first
  
  files.each do |h|
    txt = File.open(h).read
    next if txt.index(first_line) == 0
    f = File.open("#{h}.tmp", 'w')
    f.write license_text
    f.write txt
    f.close
    FileUtils.rm(h)
    FileUtils.mv("#{h}.tmp",h)
  end
end

def remove_license(dirs, options, license)
  
  files = dirs.inject([]) do |sum,dir|
    sum += get_files_to_process(dir, options[:exts])
  end
  
  puts "removing #{license} from the #{files.size} files matching #{options[:exts].join(', ')}"
  
  license_text = get_license_text(license, options[:comment])
  license_size = license_text.size
   
  files.each do |h|
    txt = File.open(h).read
    next if txt.index(license_text) != 0
    f = File.open("#{h}.tmp", 'w')
    f.write txt[license_size..-1]
    f.close
    FileUtils.rm(h)
    FileUtils.mv("#{h}.tmp",h)
  end
end
