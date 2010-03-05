`make clean; make all CPPFLAGS=-D_REENTRANT`
require 'intruder'

n = Intruder::Node.new('codeslave', 'foo', File.read(File.expand_path('~/.erlang.cookie')))

p n.status
n.connect('rabbit@codeslave')
p n.status

puts "Pid: " + n.pid.to_s

