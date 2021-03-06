# GEM: mqueue ![](https://travis-ci.org/irfansharif/mqueue.svg?branch=master) [![Gem Version](https://badge.fury.io/rb/mqueue.svg)](https://badge.fury.io/rb/mqueue)

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

mq.size
# => 10

# Timed send takes an optional parameter indicating duration it would wait
# (in seconds) before returning false, defaults to 0
refute mq.timedsend "queue full, this will fail", 0

mq.full?
# => true

# This would block indefinitely until something is removed from the queue
# mq.send "the answer to life"

# Empties queue of all messages, includes pending messages
mq.flush

# Timed receive takes an optional parameter indicating duration it would wait
# (in seconds) before returning false, defaults to 0
refute mq.timedreceive "queue empty, this will fail too", 0

mq.empty?
# => true

# Notification as defined here is when a message is added to previously empty
# queue, will not get triggered if someone else is waiting on `receive` or
# messages added to non-empty queues. is triggered asynchronously, only one
# process will be able to add the notification hook to a particular queue (NOT IMPLEMENTED)
mq.on_notification do |msg|
  puts "#{msg} delivered through notification hook"
end

mq.send "notification"
# => "notification delivered through notification hook"

# Hook no longer triggered (NOT IMPLEMENTED)
mq.disable_notification
mq.send "ignored notification"

# Deletes the queue and any containing messages, if not done so it persists until system reboot
mq.delete
```

## Benchmarks
```ruby
require 'benchmark'
# => true
q = MQueue.new "/name_a", flags: [:creat, :rdwr], capacity: 100
# => #<MQueue:0x00000002af0620>
Benchmark.bm(7) do |b|
  b.report('simultaneous:') { 1000000.times { q.send "msg"; q.receive } }
end

#                   user     system      total        real
# simultaneous:  1.460000   0.980000   2.440000  (2.445581)

# => [#<Benchmark::Tms:0x0000000263c228
#     @cstime=0.0,
#     @cutime=0.0,
#     @label="simultaneous:",
#     @real=2.44558057,
#     @stime=0.98,
#     @total=2.44,
#     @utime=1.46>]
```

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

## Contributing

Bug reports and pull requests are welcome on GitHub at [irfansharif](https://github.com/irfansharif/mqueue).


## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).

