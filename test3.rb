`make clean; make all`
require File.expand_path('../lib/intruder', __FILE__)

require 'rubygems'
require 'minitest/spec'

MiniTest::Unit.autorun

describe Intruder::Term do
  it "needs to work for atoms" do
    Intruder::Term.convert(:a).must_be_instance_of(Intruder::Atom)
  end
end
