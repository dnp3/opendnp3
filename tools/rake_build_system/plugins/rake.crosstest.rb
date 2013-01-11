require 'net/ftp'
require 'net/ssh'

module CrossTest

  def self.put(host, user, passwd, file)
    ftp = Net::FTP.new(host, user, passwd)
    STDOUT.sync = true
    ftp.putbinaryfile(file) { |block| print '*' }    
    ftp.close
  end
  
  def self.get(host, user, passwd, remote_file, local_file)
    ftp = Net::FTP.new(host, user, passwd)
    STDOUT.sync = true
    ftp.getbinaryfile(remote_file, local_file) { |block| print '*' }
    ftp.close
  end

  def self.run_tests(host, user, passwd, file)
    Net::SSH.start(host, user, :password => passwd) do |ssh|
    ssh.exec! "rm report.xml"
      ssh.exec! "chmod +x #{file}"
      ssh.exec! "./#{file} --output_format=XML --log_level=all --report_level=no > report.xml"
      ssh.exec! "rm #{file}"
    end
  end
  
  def self.do_test_sequence(host, user, passwd, file, testfile)
    self.put(host, user, passwd, file)
    self.run_tests(host, user, passwd, File.basename(file))
    self.get(host, user, passwd, 'report.xml', testfile)
  end
  
end