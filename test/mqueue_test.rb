require 'test_helper'

class MqueueTest < Minitest::Test
  def setup
    @queue_name = "/test_queue_name"
    @mq = MQueue.new @queue_name, flags: [:creat, :rdwr]
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

  def test_single_message_transmission
    @mq.send "buongiorno"
    received = @mq.receive
    assert_equal received, "buongiorno"
  end

  # def test_multiple_messages_transmission
  #   @mq.send "buongiorno"
  #   @mq.send "mondo"
  #   assert_equal @mq.receive, "buongiorno"
  #   assert_equal @mq.receive, "mondo"
  # end

  def test_cross_process_transmission
    fork do 
      another_queue = MQueue.new(@queue_name, flags: [:creat, :rdwr])
      another_queue.send "namaste"
      another_queue.delete
    end
    assert_equal @mq.receive, "namaste"
  end

  def test_timedsend_returns_false_when_full
    10.times { @mq.send "filling up the queue" }
    refute @mq.timedsend "over capacity message", 0
  end

  # def test_timedreceive_returns_false_when_empty
  #   refute @mq.timedreceive, 0
  # end

  # def test_invalid_queue_name
  #   refute MQueue.new "invalid_name", flags: [:creat, :rdwr]
  # end

  def test_queue_attributes
    @mq.send "hello"
    @mq.send "is it me you're looking for?"
    @mq.send "i can see it in your eyes"

    assert_equal @mq.size, 3
  end

  def test_custom_queue_capacity
    small_queue = MQueue.new "/small-queue", capacity: 2, flags: [:creat, :rdwr]
    2.times { small_queue.send "fill queue with messages" }

    refute small_queue.timedsend "overflow", 0
    small_queue.delete
  end

  # def test_custom_max_msgsize
  #   refute @mq.send 'c' * 4097

  #   wide_queue = MQueue.new "/tight-queue", max_msgsize: 2 ** 13, flags: [:creat, :rdwr]
  #   assert_equal 2 ** 13, wide_queue.size
  #   wide_queue.send 'c' * (2 ** 13)
  #   wide_queue.delete
  # end
end
