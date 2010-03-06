require 'intruder_ext'

module Intruder
  
  class Node
    def mod(mod_name)
      Mod.new(mod_name, self)
    end
  end

end
