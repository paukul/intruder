`make clean; make all CPPFLAGS=-D_REENTRANT`
require 'intruder'
# require 'rubygems'
# require 'ruby-debug'
# Debugger.start
# debugger
n = IntruderNode.new('codeslave', 'foo', File.read(File.expand_path('~/.erlang.cookie')))

# p n.cookie
puts "connceted!" if n.connect('rabbit@codeslave')

puts n.pid
#p n.node
