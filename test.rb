`make clean; make all`
require File.expand_path('../lib/intruder', __FILE__)
hostname = `hostname`.chomp

n = Intruder::Node.new('fooz', File.read(File.expand_path('~/.erlang.cookie')))

puts "rabbit call"
n.connect("rabbit@#{hostname}")
m = n.mod('rabbit')
ret = m.status('')
puts ret.to_s
puts ret.class

puts "-----------"
ret.each do |member|
  puts member.to_s + " class: " + member.class.to_s
end
puts "-----------"
puts ret[2].to_s



# puts
# puts "test call"
# n.connect('foo@codeslave')
# m = n.mod('test')
# m.say('')


# puts "Pid: " + n.pid.to_s

