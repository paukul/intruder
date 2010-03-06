require 'intruder_ext'

module Intruder
  
  class Node
    def mod(mod_name)
      Mod.new(mod_name, self)
    end
  end

  class Mod

    private

    def method_missing(method, *args)
      rpc(method.to_s, *args)
    end
  end
  
end
