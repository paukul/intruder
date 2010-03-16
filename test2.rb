`make clean; make`
require 'lib/intruder'
include Intruder

n = Node.new('fazy', File.read(File.expand_path('~/.erlang.cookie')))
n.connect('snaps@codeslave')

test = n.mod('test')
#p test.say('[a]')
p test.say('[<<"/">>]')
