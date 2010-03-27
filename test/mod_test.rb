require File.expand_path('../../lib/intruder', __FILE__)

require 'rubygems'
require 'minitest/spec'

include Intruder
MiniTest::Unit.autorun

describe Intruder::Mod do
  it "trying an rpc call should raise an exception if the node is not yet connected" do
    node = Node.new('foo', 'somethingsomethingdarkside')
    mod = node.mod('some_mod')
    lambda{ mod.some_fun(Term.encode([])) }.must_raise(Error)
  end
end
