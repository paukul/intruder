require 'mkmf'

extension_name = 'erl_node'

dir_config(extension_name)

findei=File.join(File.dirname(__FILE__),"findei.erl")
File.chmod(0755,findei)

ei_dir=`#{findei}`.chomp + '/include/'

# $LIBPATH.unshift ei_dir
if find_header('erl_interface.h', ei_dir)
  create_makefile(extension_name)
else
  puts "error: erl_interface not found!"
end
