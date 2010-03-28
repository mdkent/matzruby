require 'test/psych/helper'

module Psych
  module Visitors
    class TestYAMLTree < TestCase
      def setup
        super
        @v = Visitors::YAMLTree.new
      end

      def test_object_has_no_class
        yaml = Psych.dump(Object.new)
        assert(Psych.dump(Object.new) !~ /Object/, yaml)
      end

      def test_struct_const
        foo = Struct.new("Foo", :bar)
        assert_cycle foo.new('bar')
      end

      A = Struct.new(:foo)

      def test_struct
        assert_cycle A.new('bar')
      end

      def test_struct_anon
        s = Struct.new(:foo).new('bar')
        obj =  Psych.load(Psych.dump(s))
        assert_equal s.foo, obj.foo
      end

      def test_exception
        ex = Exception.new 'foo'
        loaded = Psych.load(Psych.dump(ex))

        assert_equal ex.message, loaded.message
        assert_equal ex.class, loaded.class
      end

      def test_regexp
        assert_cycle(/foo/)
        assert_cycle(/foo/i)
        assert_cycle(/foo/mx)
      end

      def test_time
        assert_cycle Time.now
      end

      def test_date
        date = Date.strptime('2002-12-14', '%Y-%m-%d')
        assert_cycle date
      end

      def test_rational
        assert_cycle Rational(1,2)
      end

      def test_complex
        assert_cycle Complex(1,2)
      end

      def test_scalar
        assert_cycle 'foo'
        assert_cycle ':foo'
        assert_cycle ''
        assert_cycle ':'
      end

      def test_boolean
        assert_cycle true
        assert_cycle 'true'
        assert_cycle false
        assert_cycle 'false'
      end

      def test_range_inclusive
        assert_cycle 1..2
      end

      def test_range_exclusive
        assert_cycle 1...2
      end

      def test_anon_class
        assert_raises(TypeError) do
          @v.accept Class.new
        end

        assert_raises(TypeError) do
          Psych.dump(Class.new)
        end
      end

      def test_hash
        assert_cycle('a' => 'b')
      end

      def test_list
        assert_cycle(%w{ a b })
        assert_cycle([1, 2.2])
      end

      def test_symbol
        assert_cycle :foo
      end

      def test_int
        assert_cycle 1
        assert_cycle(-1)
        assert_cycle '1'
        assert_cycle '-1'
      end

      def test_float
        assert_cycle 1.2
        assert_cycle '1.2'

        assert Psych.load(Psych.dump(0.0 / 0.0)).nan?
        assert_equal 1, Psych.load(Psych.dump(1 / 0.0)).infinite?
        assert_equal(-1, Psych.load(Psych.dump(-1 / 0.0)).infinite?)
      end

      # http://yaml.org/type/null.html
      def test_nil
        assert_cycle nil
        assert_equal nil, Psych.load('null')
        assert_equal nil, Psych.load('Null')
        assert_equal nil, Psych.load('NULL')
        assert_equal nil, Psych.load('~')
        assert_equal({'foo' => nil}, Psych.load('foo: '))

        assert_cycle 'null'
        assert_cycle 'nUll'
        assert_cycle '~'
      end
    end
  end
end
