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
      assert_instance_of Atom, list[0]
    end

    it "should create lists with character lists for arrays with strings" do
      list = Term.encode(["asdf"])
      assert_instance_of List, list[0]
    end

    it "should raise an exception for datatypes that can't be encodeed" do
      assert_raises Intruder::Error do
        Term.encode({:a => 1})
      end
    end
  end

#  describe ".to_hash" do
#    it "should decode lists of tuples with two members using the members as key / value pairs" do
#      Intruder::
#    end
#  end
end

describe Intruder::Tuple do
  describe ".new" do
    it "should create a tuple from the contents of an array" do
      Intruder::Tuple.new([:a]).must_be_instance_of(Intruder::Tuple)
    end
  end

  describe "[]" do # thats some aweful duplication
    before do
      bin = Binary.new("foo")
      @tuple = Tuple.new([:a, :b, :c, bin])
    end

    it "should return the element at the given position" do
      @tuple[0].to_s.must_equal "a"
    end

    it "should return nil for numbers greater than the tuple size" do
      @tuple[@tuple.size].must_be_nil
    end

    it "should return nil for numbers smaller than 0" do
      @tuple[-1].must_be_nil
    end

    it "should return binarys" do
      @tuple[3].must_be_instance_of(Binary)
      @tuple[3].to_s.must_equal("foo")
    end
  end
end

describe Intruder::List do
  describe "[]" do
    before do
      @list = Term.encode([:a, :b, :c])
    end

    it "should return the element at the given position" do
      @list[0].to_s.must_equal "a"
    end

    it "should return nil for numbers greater than the list size" do
      @list[@list.size].must_be_nil
    end

    it "should return nil for numbers smaller than 0" do
      @list[-1].must_be_nil
    end
  end
end

describe Intruder::Binary do
  describe "new" do
    before do
      @bin = Intruder::Binary.new("foo")
    end

    it "should create a new binary from a string" do
      @bin.must_be_instance_of(Intruder::Binary)
    end

    it "should encode the binary to a human readable string if possible" do
      @bin.to_s.must_equal("foo")
    end
  end
end
