require "mqueue/mqueue"

module MQueueExtensions
  def flush
    values = []
    values << timedreceive(0) while size > 0
    values
  end

  def full?
    size == capacity
  end

  def empty?
    size == 0
  end

  def delete
    flush
    super
  end
end

class MQueue
  prepend MQueueExtensions
end
