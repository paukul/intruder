`make clean; make all`
require File.expand_path('../../lib/intruder', __FILE__)
hostname = `hostname`.chomp

n = Intruder::Node.new('fooz', File.read(File.expand_path('~/.erlang.cookie')))

puts "--- rabbit call ---\n"
n.connect("rabbit@#{hostname}")
m = n.mod("rabbit_amqqueue")

param = Intruder::Binary.new("\\")

p param
p m.info_all(Intruder::Term.encode([param]))
