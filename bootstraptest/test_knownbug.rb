#
# This test file concludes tests which point out known bugs.
# So all tests will cause failure.
#

assert_finish 1, %q{
  r, w = IO.pipe
  t1 = Thread.new { r.sysread(1) }
  t2 = Thread.new { r.sysread(1) }
  sleep 0.1
  w.write "a"
  sleep 0.1
  w.write "a"
}, '[ruby-dev:31866]'

assert_normal_exit %q{
  Marshal.load(Marshal.dump({"k"=>"v"}), lambda {|v| })
}

assert_not_match /method_missing/, %q{
  STDERR.reopen(STDOUT)
  variable_or_mehtod_not_exist
}

assert_normal_exit %q{
  ary = (1..10).to_a
  ary.permutation(2) {|x|
    if x == [1,2]
      ObjectSpace.each_object(String) {|s|
        s.clear if s.length == 40 || s.length == 80
      }
    end
  }
}, '[ruby-dev:31982]'

