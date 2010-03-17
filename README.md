**A little WIP again**

What is it?
===========

A ruby library with a C extension to interact with erlang processes.
At the moment it's my fun project to get my feet wet with C. Stay tuned :)

Want to try?
============

the test script makes an rpc call to a rabbitmq server and queries for its status.

* checkout the source
* `ruby extconf.rb`
* start your rabbitmq server `sudo rabbitmq-server`
* `ruby test.rb`

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

because thats a fun project to learn c and I need it for another project of mine (namely: paukul/humpty#4)

What works already?
===================

description missing... yet :)

Erlix?
======

is great, I also took some stuff from them. Afaik, Erlix uses erl_interface for most of its operations. erl_interface is limited to one connection only, I needed something to open multiple connections to multiple queues. Erlix is able to do a LOT more than Intruder at the moment, make sure to check it out as well!
