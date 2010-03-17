`make clean; make`
#
# Run the test.erl script before:
#  erl test.erl -sname snaps
#  c(test).
#
require File.expand_path('../../lib/intruder', __FILE__)
include Intruder

n = Node.new('fazy', File.read(File.expand_path('~/.erlang.cookie')))
n.connect("snaps@#{`hostname`.chomp}")

test = n.mod('test')
p test.say(Intruder::Term.encode([:a]))
