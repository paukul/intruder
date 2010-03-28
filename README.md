What is it?
===========

A ruby library with a C extension to interact with erlang processes.
There's not much more than the ability to connect a node to a erlang
process and then run some rpc calls.

Want to use it?
==========

install it from gemcutter with `gem install intruder --source
http://gemcutter.org`.

Still interested?
==========

Glad you asked...
First of all you need a node. A node is more or less your
representative in the erlang world. You need to give it a name and it
needs a cookie which basically manages your access rights.
So for example if you wan't to communicate with a RabbitMQ process,
you need to use the same magic cookie value as your RabbitMQ server
does. Usually the erlang cookie is stored in the home directory in a
hidden file called .erlang.cookie. Sometimes it can also be found in the
application directory of your erlang application. Whatever...

Let's try to get our feet wet and write a simple little echofunction
in erlang. Here's the content of the file echo.erl:

      -module(echo).
      -export([say/1]).

        say(Any) ->
          {ok, Any}.

Thats pretty basic stuff here. What we have here is a module called
"echo" with a function called "say" that just returns a tuple of ok plus whatever arguments it was
called with. Now open your erlang shell with `erl -sname funkyzeit`
in the same directory as your echo.erl file is located and type
`c(echo).` (including the dot) into the console. Now you have a node
named "funkyzeit" running which knows about a module called "echo".

But back to ruby. Open another console and fire up irb:

First we wan't to create the node (assuming that the .erlang.cookie
file is located at '~/.erlang.cookie').

    require 'rubygems'
    require 'intruder'
    node = Intruder::Node.new('my_node', File.read(File.expand_path('~/.erlang.cookie')))

Great! Now connect that thing to the other erlang node we fired up
earlier (replace <myhost> with your actual hostname.... skip the
.local if you are a mac user).

    node.connect('funkyzeit@myhost')

You should be connected to the other node now. Let's create the
arguments we gonna pass to the echo module now.

    args = Intruder::Term.encode([[:a, :b, :c]])

Intruder::Term.encode is able to create Intruder Terms like lists,
atoms, and "strings" if you pass them in as arrays, atoms or strings
(or combinations of them). If you need a tuple or a binary you need to
create them with `Intruder::Binary.new("mybinaryvalue")` or
`Intruder::Tuple.new([:a, :b])` for now. Maybe I'll come up with a
cleverer solution for that later... Lets call the "say" method on the
"echo" module now. Therefore we create a proxy object for the erlang
module and let some super simple method missing magic call the
function on that proxy.

    echo = node.mod('echo')
    response = echo.say(args)

The response should be a tuple containing the atom 'ok' and our list
of atoms we created with the Term.encode function earlier. Let's
confirm that:

    response
    => {ok, [a, b]}
    p response.class
    => Intruder::Tuple
    response[0].to_s
    => "ok"
    p response[1].class
    => Intruder::List
    p response[1][0]
    => a

I hope that explains enough to get the idea...

Issues
====

Well the obvious one is the lack of functionality. For my purpose,
being able to run rpc calls could already be enough, but there are way
more things to add that are expsed via the erl_interface / ei C
libraries.

Test coverage is... well, let's not talk about that.

Keeping nodes alive requires to constantly reply to ERL_TICK messages
that might be send by the remote node. To handle that I'm currently
using a "keep_alive" POSIX thread and mutexes and stuff. That's just
because I'm a bloody noob at those kind of things (frankly, they scare
me!). That might (hopefully) change soon.

.. insert more issues here ...

Run an example?
============

the test script makes an rpc call to a rabbitmq server and queries for
its status (Note: the example assumes that you are running it from
within a checked out and compiled version).

* checkout the source
* `ruby extconf.rb`
* start your rabbitmq server `sudo rabbitmq-server`
* `ruby examples/rabbitmq_status_rpc.rb`

the script should print something similar to this:
    --- rabbit call ---
    sending params: []
    rpc call to rabbit:status
    [{running_applications, [{rabbit, "RabbitMQ", "1.7.2"}, {mnesia, "MNESIA  CXC 138 12", "4.4.13"}, {os_mon, "CPO  CXC 138 46", "2.2.5"}, {sasl, "SASL  CXC 138 11", "2.1.9"}, {stdlib, "ERTS  CXC 138 10", "1.16.5"}, {kernel, "ERTS  CXC 138 10", "2.13.5"}]}, {nodes, ['rabbit@codeslave']}, {running_nodes, ['rabbit@codeslave']}]
    Intruder::List

    --- length of the response ---
    Members: 3

    --- print every member with its class ---
    {running_applications, [{rabbit, "RabbitMQ", "1.7.2"}, {mnesia, "MNESIA  CXC 138 12", "4.4.13"}, {os_mon, "CPO  CXC 138 46", "2.2.5"}, {sasl, "SASL  CXC 138 11", "2.1.9"}, {stdlib, "ERTS  CXC 138 10", "1.16.5"}, {kernel, "ERTS  CXC 138 10", "2.13.5"}]} class: Intruder::Tuple
    {nodes, ['rabbit@codeslave']} class: Intruder::Tuple
    {running_nodes, ['rabbit@codeslave']} class: Intruder::Tuple

    --- access the second element and print its size ---
    {running_nodes, ['rabbit@codeslave']} has size 2

    --- access the first element of the second member ---
    nodes

    --- which is a ---
    Intruder::Atom

Why that?
=========

because thats a fun project to learn c and I need it for humpty,
another project of mine (find it on github as well).

But your C code sucks big time!
====================

I know, I'm a C-n00b. Wan't to improve it? Contribute? Ah, Not
interested? ok then stfu :)

Erlix?
======

is great, I also took some stuff from them. Afaik, Erlix uses
erl_interface for most of its operations. erl_interface is limited to
one connection only, I needed something to open multiple connections
to multiple queues. If thats fine for you, Erlix is able to do a LOT
more than Intruder at the moment, make sure to check it out as well!
