# GEM: mqueue

Lightweight wrapper built around [POSIX message queues](http://man7.org/linux/man-pages/man7/mq_overview.7.html). Abstracted functionality when designing a module for Ruby IPC with concurrent processes, reference used was [The Linux Programming Interface](http://man7.org/tlpi/download/TLPI-52-POSIX_Message_Queues.pdf).

Highlighted features:

* POSIX messages have associated priorities, messages are always strictly queued (and received) in priority order
* POSIX message queues have associated sets of attributes that can be set when the queue is created or opened
* Allows a process to register for message notification
from a queue. After registering, the process is notified of the availability of a message
by delivery of a signal or by the invocation of a function in a separate thread
* Ridiculously fast, everything is delegated to the kernel
*

**NOTE:** At the time of writing there already existed a [wrapper](https://github.com/Sirupsen/posix-mqueue) around POSIX message queues but didn't offer some of the notification subscription functionality I needed for IPC, while I could've opted to add that into the original (you'll notice a lot of overlap), I wanted to experiment with C extensions in ruby for some other libraries I'm going to be working on in the future, messaging queues was a subject I was sufficiently versed it to start with.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'mqueue'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install mqueue

## Usage

```ruby
require 'mqueue'

# Queue names must start with '/', shown below are the default settings
provided # by the Kernel (except for queue_name), check below for options
available for flags and how to change # default settings outside ruby
mq = MQueue.new("/queue_name", capacity: 10, max_msgsize: 8192, flags: [:creat, :rdwr])
mq.send "message"

mq.receive
# => "message"

fork { mq.send "another one" }

# Blocks on empty queues, in this case until forked process adds message onto queue
mq.recieve
# => "another one"

10.times do
  mq.send "42"
end

queue.size
# => 10

# Timed send takes an optional parameter indicating duration it would wait
# (in seconds) before throwing a MQueue::QueueFull error, defaults to 0
assert_raises MQueue::QueueFull do
  mq.timedsend "queue full, this will fail", 0
end

queue.full?
# => true

# This would block indefinitely until something is removed from the queue
# queue.send "the answer to life"

# Empties queue of all messages, includes pending messages
queue.flush

# Timed receive takes an optional parameter indicating duration it would wait
# (in seconds) before throwing a MQueue::QueueEmpty error, defaults to 0
assert_raises MQueue::QueueEmpty do
  queue.timedreceive "queue empty, this will fail too", 0
end

queue.empty?
# => true

# Notification as defined here is when a message is added to previously empty
# queue, will not get triggered if someone else is waiting on `receive` or
# messages added to non-empty queues. is triggered asynchronously, only one
# process will be able to add the notification hook to a particular queue
queue.on_notification do |msg|
  puts "#{msg} delivered through notification hook"
end

queue.send "notification"
# => "notification delivered through notification hook"

# Hook no longer triggered
queue.disable_notification
queue.send "ignored notification"

# Deletes the queue and any containing messages, if not done so it persists until system reboot
queue.delete
```

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

## Contributing

Bug reports and pull requests are welcome on GitHub at [irfansharif](https://github.com/irfansharif/mqueue).


## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).

