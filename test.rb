`make clean; make all CPPFLAGS=-D_REENTRANT`
require 'intruder'

n = Intruder::Node.new('foo', File.read(File.expand_path('~/.erlang.cookie')))

p n.status
n.connect('rabbit@codeslave')
# p n.status

m = n.mod('rabbit')
# p m
m.status('yet')
# m.status(['/'])

puts "Pid: " + n.pid.to_s

