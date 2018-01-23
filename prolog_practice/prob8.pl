%prob8

% Attach facts for the configuration here:
p(x,1,1).
p(x,1,3).
p(o,3,1).
p(o,3,3).

%p(x,1,1).
%p(x,2,3).
%p(o,3,1).
%p(o,3,3).

p(b, R, C):- \+ p(x, R, C), \+ p(o, R, C). 
validRow(X) :- member(X,[1,2,3]).
validCol(Y) :- member(Y,[1,2,3]).

towin(Player,1,1):- p(b,1,1), p(Player,1,2), p(Player,1,3).
towin(Player,1,2):- p(Player,1,1), p(b,1,2), p(Player,1,3).
towin(Player,1,3):- p(Player,1,1), p(Player,1,2), p(b,1,3).

towin(Player,2,1):- p(b,2,1), p(Player,2,2), p(Player,2,3).
towin(Player,2,2):- p(Player,2,1), p(b,2,2), p(Player,2,3).
towin(Player,2,3):- p(Player,2,1), p(Player,2,2), p(b,2,3).

towin(Player,3,1):- p(b,3,1), p(Player,3,2), p(Player,3,3).
towin(Player,3,2):- p(Player,3,1), p(b,3,2), p(Player,3,3).
towin(Player,3,3):- p(Player,3,1), p(Player,3,2), p(b,3,3).

towin(Player,1,1):- p(b,1,1), p(Player,2,1), p(Player,3,1).
towin(Player,2,1):- p(Player,1,1), p(b,2,1), p(Player,3,1).
towin(Player,3,1):- p(Player,1,1), p(Player,2,1), p(b,3,1).

towin(Player,1,2):- p(b,1,2), p(Player,2,2), p(Player,3,2).
towin(Player,2,2):- p(Player,1,2), p(b,2,2), p(Player,3,2).
towin(Player,3,2):- p(Player,1,2), p(Player,2,2), p(b,3,2).

towin(Player,1,3):- p(b,1,3), p(Player,2,3), p(Player,3,3).
towin(Player,2,3):- p(Player,1,3), p(b,2,3), p(Player,3,3).
towin(Player,3,3):- p(Player,1,3), p(Player,2,3), p(b,3,3).

towin(Player,1,1):- p(b,1,1), p(Player,2,2), p(Player,3,3).
towin(Player,2,2):- p(Player,1,1), p(b,2,2), p(Player,3,3).
towin(Player,3,3):- p(Player,1,1), p(Player,2,2), p(b,3,3).

towin(Player,1,3):- p(b,1,3), p(Player,2,2), p(Player,3,1).
towin(Player,2,2):- p(Player,1,3), p(b,2,2), p(Player,3,1).
towin(Player,3,1):- p(Player,1,3), p(Player,2,2), p(b,3,1).

ttt_move(Player, R, C):-
    consider_win(Player, R, C),
	write('go for win'), nl.

ttt_move(Player, R, C):-
    \+ consider_win(Player, R, C), 
	consider_stopOther(Player, R, C),
	write('move to block opponent!'), nl.

consider_win(Player, R, C):-
    validRow(R),   
    validCol(C),
	towin(Player, R, C).

consider_stopOther(Player, R, C):-
    Player == x, CompPlayer = o,
    validRow(R),   
    validCol(C),
	towin(CompPlayer, R, C).

consider_stopOther(Player, R, C):-
    Player == o, CompPlayer = x,
    validRow(R),   
    validCol(C),
	towin(CompPlayer, R, C).
