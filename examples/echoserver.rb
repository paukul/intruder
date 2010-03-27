#
# Run the test.erl script before:
#  erl test.erl -sname snaps
#  c(test).
#
require File.expand_path('../../lib/intruder', __FILE__)
include Intruder

n = Node.new('fazy', File.read(File.expand_path('~/.erlang.cookie')))
#hostname = `hostname`.chomp
hostname = 'nb-pfriederich'
n.connect("snaps@#{hostname}")

test = n.mod('test')
p test.say(Intruder::Term.encode([[:a, :b, :c]]))
p test.say(Intruder::Term.encode([Intruder::Binary.new("uauaua")]))
