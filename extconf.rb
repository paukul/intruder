require 'mkmf'

extension_name = 'erl_node'

dir_config(extension_name)

findei=File.join(File.dirname(__FILE__),"findei.erl")
File.chmod(0755,findei)

ei_dir=`#{findei}`.chomp
include_dir = ei_dir + '/include/'
lib_dir = ei_dir + '/lib/'

$LDFLAGS << "-L#{lib_dir} -lei -lerl_interface"

if find_header('ei.h', include_dir)
  create_makefile(extension_name)
else
  puts "error: erl_interface not found!"
end
