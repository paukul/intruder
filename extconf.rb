require 'mkmf'

extension_name = 'intruder_ext'
src_dir = File.expand_path('../src', __FILE__)
findei = File.expand_path("../findei.erl", __FILE__)

dir_config(extension_name)
File.chmod(0755, findei)

ei_dir = `#{findei}`.chomp
include_dir = ei_dir + '/include/'
lib_dir = ei_dir + '/lib/'

$LDFLAGS << " -L#{lib_dir} -lei -lerl_interface"
$CPPFLAGS << ' -D_REENTRANT -g'

if find_header('ei.h', include_dir)
  create_makefile(extension_name, src_dir)
else
  puts "error: erl_interface not found!"
end
