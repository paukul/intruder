-module(test).
-export([say/1]).

say(Any) ->
          {ok, congratulations, [a, {b, c}], Any}.