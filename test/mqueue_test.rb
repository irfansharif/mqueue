require 'test_helper'

class MqueueTest < Minitest::Test
  def setup
    @mq = MQueue.new "/queue_name"
  end

  def teardown
    @mq.delete
  end

  def test_that_it_has_a_version_number
    refute_nil MQueue::VERSION
  end

  def test_that_it_initializes_with_default_attributes
    assert_equal @mq.size, 0
    assert_equal @mq.capacity, 10
  end
end
