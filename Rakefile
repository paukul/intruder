require 'rubygems'
require 'rake'

RUBY = "ruby"

# http://blog.flavorjon.es/2009/06/easily-valgrind-gdb-your-ruby-c.html
def test_suite_cmdline
  require 'find'
  files = []
  Find.find("test") do |f|
    files << f if File.basename(f) =~ /.*test.*\.rb$/
  end
  cmdline = "#{RUBY} -w -I.:lib:ext:test -rtest/unit \
               -e '%w[#{files.join(' ')}].each {|f| require f}'"
end

task :compile do
  `make clean; make all`
end

namespace :test do
  # partial-loads-ok and undef-value-errors necessary to ignore
  # spurious (and eminently ignorable) warnings from the ruby
  # interpreter
  VALGRIND_BASIC_OPTS = "--num-callers=50 --error-limit=no --dsymutil=yes \
                         --partial-loads-ok=yes --undef-value-errors=no"

  desc "run test suite under valgrind with basic ruby options"
  task :valgrind => :compile do
    cmdline = "valgrind #{VALGRIND_BASIC_OPTS} #{test_suite_cmdline}"
    puts cmdline
    system cmdline
  end
end
