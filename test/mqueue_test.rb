require 'test_helper'

class MqueueTest < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil MQueue::VERSION
  end
end
