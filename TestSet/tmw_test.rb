# 
# Licensed to Green Energy Corp (www.greenenergycorp.com) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  Green Enery Corp licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
#  
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
# 

require 'rexml/document'
include REXML

return -1 unless ARGV.count >= 7

s_confirms =          ARGV[0]
app_conf_timeout =    ARGV[1]
unsol_allowed =       ARGV[2]
unsol_retry_period =  ARGV[3]
unsol_offline_delay = ARGV[4]
tx_frag_size =        ARGV[5]
self_address =        ARGV[6]

test_msg = ARGV[7] if ARGV.count >= 8
test_msg ||= ""

#puts "SCONFIRMS:          " + s_confirms
#puts "APPCONFTIMEOUT:     " + app_conf_timeout
#puts "UNSOLALLOWED:       " + unsol_allowed
#puts "UNSOLRETRYPER:      " + unsol_retry_period
#puts "UNSOLOFFLINEDELAY:  " + unsol_offline_delay
#puts "TXFRAGSIZE:         " + tx_frag_size
#puts "SELFADDRESS:        " + self_address
#puts "Test:               " + test_msg


file = File.open("C:\\code\\DNP\\TestSet\\tmw_template.xml")
doc = Document.new file

doc.elements.each("//LinkLayer") do |link|
  link.attributes["UseConfirmations"] = (s_confirms == "always") ? "true" : "false"
end

doc.elements.each("//AppLayer") do |app|
  app.attributes["ConfTimeoutMS"] = app_conf_timeout > unsol_retry_period ? app_conf_timeout : unsol_retry_period
  app.attributes["MaxFragmentSize"] = tx_frag_size
end

doc.elements.each("//UnsolDefaults") do |app|
  app.attributes["UnsolEnabled"] = unsol_allowed
  #app.attributes["DoClass1"] = unsol_allowed
  #app.attributes["DoClass2"] = unsol_allowed
  #app.attributes["DoClass3"] = unsol_allowed
  app.attributes["RetryMS"] = 0
end

filename = "C:/code/DNP/TestSet/temp_slave.xml"
outfile = File.new(filename, "w+")
doc.write outfile
outfile.close

puts filename

