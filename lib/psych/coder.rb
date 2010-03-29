module Psych
  ###
  # If an object defines +encode_with+, then an instance of Psych::Coder will
  # be passed to the method when the object is being serialized.  The Coder
  # automatically assumes a Psych::Nodes::Mapping is being emitted.  Other
  # objects like Sequence and Scalar may be emitted if +seq=+ or +scalar=+ are
  # called, respectively.
  class Coder
    attr_accessor :tag, :style, :implicit
    attr_reader   :type, :map, :scalar, :seq

    def initialize tag
      @map        = {}
      @seq        = []
      @implicit   = false
      @type       = :map
      @tag        = tag
      @style      = Psych::Nodes::Mapping::BLOCK
      @scalar     = nil
    end

    # Emit a scalar with +value+ and +tag+
    def represent_scalar tag, value
      self.tag    = tag
      self.scalar = value
    end

    # Emit a sequence with +list+ and +tag+
    def represent_seq tag, list
      @tag = tag
      self.seq = list
    end

    # Emit a sequence with +map+ and +tag+
    def represent_map tag, map
      @tag = tag
      self.map = map
    end

    # Emit a scalar with +value+
    def scalar= value
      @type   = :scalar
      @scalar = value
    end

    # Emit a map with +value+
    def map= map
      @type = :map
      @map  = map
    end

    def []= k, v
      @type = :map
      @map[k] = v
    end

    def [] k
      @type = :map
      @map[k]
    end

    # Emit a sequence of +list+
    def seq= list
      @type = :seq
      @seq  = list
    end
  end
end
