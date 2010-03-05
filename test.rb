`make clean; make CPPFLAGS=-D_REENTRANT`
require 'erl_node'
# require 'rubygems'
# require 'ruby-debug'
# Debugger.start
# debugger
n = ErlNode.new('nb-pfriederich', 'foo', File.read(File.expand_path('~/.erlang.cookie')))
p n.cookie
n.connect('rabbit@nb-pfriederich')

#p n.node
