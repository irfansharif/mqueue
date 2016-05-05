require 'test_helper'

class MQueueAttributesTest < Minitest::Test
  def test_invalid_queue_name
    assert_raises RuntimeError do
      MQueue.new "invalid_name", flags: [:creat, :rdwr]
    end
  end

  def test_custom_queue_capacity
    small_queue = MQueue.new "/small_queue", capacity: 2, flags: [:creat, :rdwr]
    2.times { small_queue.send "fill queue with messages" }

    refute small_queue.timedsend "overflow", 0
    small_queue.delete
  end

  def test_custom_max_msgsize
    wide_queue = MQueue.new "/tight_queue", max_msgsize: 2 ** 8, flags: [:creat, :rdwr]
    assert_equal 2 ** 8, wide_queue.max_msgsize
    assert wide_queue.send 'c' * (2 ** 8)
    refute wide_queue.send 'c' * ((2 ** 8) + 1)
    wide_queue.delete
  end
end
