require 'test_helper'

class MQueueTest < Minitest::Test
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
    assert_equal @mq.receive, "buongiorno"
  end

  def test_multiple_messages_transmission
    @mq.send "buongiorno"
    @mq.send "mondo"
    assert_equal @mq.receive, "buongiorno"
    assert_equal @mq.receive, "mondo"
  end

  def test_cross_process_transmission
    fork do
      another_queue = MQueue.new(@queue_name, flags: [:creat, :rdwr])
      another_queue.send "namaste"
    end
    assert_equal @mq.receive, "namaste"
  end

  def test_timedsend_returns_false_when_full
    10.times { @mq.send "filling up the queue" }
    refute @mq.timedsend "over capacity message", 0
  end

  def test_timedreceive_returns_false_when_empty
    refute @mq.timedreceive 0
  end

  def test_queue_attributes
    @mq.send "hello"
    @mq.send "is it me you're looking for?"
    @mq.send "i can see it in your eyes"

    assert_equal @mq.size, 3
  end
end
