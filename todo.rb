`make clean; make`
require 'erl_node'

n = ErlNode.new('rabbot@localhost', File.read(File.expand_path('~/.erlang.cookie')))

p n
p n.nodename
