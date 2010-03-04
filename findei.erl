#!/usr/bin/env escript
%%% Author : KDr2
%%% Find the location of erl_interface.

main(_) ->
    io:format("~s~n", [code:lib_dir(erl_interface)]).
