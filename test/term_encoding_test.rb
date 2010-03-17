`make clean; make all`
require File.expand_path('../../lib/intruder', __FILE__)

require 'rubygems'
require 'minitest/spec'

include Intruder
MiniTest::Unit.autorun

describe Intruder::Term do
  describe ".encode" do
    it "should create atoms for symbols" do
      assert_instance_of Atom, Term.encode(:a)
    end

    it "should create empty lists for empty arrays" do
      list = Term.encode([])
      assert_instance_of List, list
      assert_equal 0, list.size
    end

    it "should create lists with atom members for arrays with symbols" do
      list = Term.encode([:a])
      assert_instance_of List, list
      assert_instance_of Atom, list[0]
    end

    it "should raise an exception for datatypes that can't be encodeed" do
      assert_raises Intruder::Error do
        Term.encode({:a => 1})
      end
    end
  end
end
