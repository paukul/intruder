`make clean; make all`
require File.expand_path('../lib/intruder', __FILE__)
hostname = `hostname`.chomp

n = Intruder::Node.new('fooz', File.read(File.expand_path('~/.erlang.cookie')))

puts "--- rabbit call ---\n"
n.connect("rabbit@#{hostname}")
m = n.mod('rabbit')
ret = m.status(Intruder::Term.encode([]))
puts ret.to_s
puts ret.class

puts "\n--- length of the response ---\n"
puts "Members: #{ret.size}"
puts "\n--- print every member with its class ---\n"
ret.each do |member|
  puts member.to_s + " class: " + member.class.to_s
end
puts "\n--- access the second element and print its size ---\n"
puts "#{ret[2].to_s} has size #{ret[2].length}"
puts "\n--- access the first element of the second member ---\n"
puts ret[1][0].to_s
puts "\n--- which is a ---\n"
puts ret[1][0].class

