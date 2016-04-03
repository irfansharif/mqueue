require "mqueue/mqueue"

class MQueue
  def flush
    values = []
    values << self.timedreceive(0) while self.size > 0
    values
  end
end
