**A little WIP again**

Want to try?
===========

the test script makes an rpc call to a rabbitmq server and queries for its status.

* checkout the source
* `./configure`
* start your rabbitmq server `sudo rabbitmq-server`
* `ruby test.rb`

the script should print something similar to this:
    rabbit call
    sending params: []
    rpc call to rabbit:status
    rpc response: [{running_applications, [{rabbit, "RabbitMQ", "1.7.2"}, {mnesia, "MNESIA  CXC 138 12", "4.4.13"}, {os_mon, "CPO  CXC 138 46", "2.2.5"}, {sasl, "SASL  CXC 138 11", "2.1.9"}, {stdlib, "ERTS  CXC 138 10", "1.16.5"}, {kernel, "ERTS  CXC 138 10", "2.13.5"}]}, {nodes, [rabbit@codeslave]}, {running_nodes, [rabbit@codeslave]}]

Why that?
=========

because thats a fun project to learn c :)

What works already?
===================

not much

Erlix?
======

is great, took some stuff from there, as I said, thats a fun project for me to learn more about c and c ruby extensions
