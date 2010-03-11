`make clean; make all`
require File.expand_path('../intruder', __FILE__)
hostname = `hostname`.chomp

n = Intruder::Node.new('fooz', File.read(File.expand_path('~/.erlang.cookie')))

puts "rabbit call"
n.connect("rabbit@#{hostname}")
m = n.mod('rabbit')
puts m.status('').to_s

# puts
# puts "test call"
# n.connect('foo@codeslave')
# m = n.mod('test')
# m.say('')


# puts "Pid: " + n.pid.to_s

