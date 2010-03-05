`make clean; make CPPFLAGS=-D_REENTRANT`
require 'invader'
# require 'rubygems'
# require 'ruby-debug'
# Debugger.start
# debugger
n = InvaderNode.new('codeslave', 'foo', File.read(File.expand_path('~/.erlang.cookie')))
# p n.cookie
puts "connceted!" if n.connect('rabbit@codeslave')
puts n.pid
#p n.node
