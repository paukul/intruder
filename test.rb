`make clean; make all CPPFLAGS=-D_REENTRANT`
require 'intruder'
# require 'rubygems'
# require 'ruby-debug'
# Debugger.start
# debugger
n = IntruderNode.new('codeslave', 'foo', File.read(File.expand_path('~/.erlang.cookie')))

# p n.cookie
p n.status
n.connect('rabbit@codeslave')
p n.status

puts "Pid: " + n.pid.to_s
#p n.node
