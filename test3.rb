`make clean; make all`
require File.expand_path('../lib/intruder', __FILE__)

require 'rubygems'
require 'minitest/spec'

include Intruder
MiniTest::Unit.autorun

describe Intruder::Term do
  describe ".convert" do
    it "should create atoms for symbols" do
      assert_instance_of Atom, Term.convert(:a)
    end

    it "should create empty lists for empty arrays" do
      list = Term.convert([])
      assert_instance_of List, list
      assert_equal 0, list.size
    end

    it "should raise an exception for datatypes that can't be converted" do
      assert_raises Intruder::Error do
        Term.convert({:a => 1})
      end
    end
  end
end
