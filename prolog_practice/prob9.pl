%prob9

visited(1,1).
visited(2,1).
visited(1,2).
stench(2,1).
breeze(1,2).

%visited(1,1).
%visited(2,1).
%visited(1,2).
%stench(2,1).
%breeze(1,2).
%visited(2,2).

%visited(4,1).
%visited(4,2).
%visited(4,3).
%visited(4,4).
%stench(4,2).
%breeze(4,3).
%breeze(4,4).

validCol(X) :- member(X,[1,2,3,4]).
validRow(Y) :- member(Y,[1,2,3,4]).

%To represent adjacent block
adj(1,2).
adj(2,1).
adj(2,3).
adj(3,2).
adj(3,4).
adj(4,3).
adjacent( [X1, Y1], [X2, Y2] ) :- X1 = X2, adj( Y1, Y2 ).
adjacent( [X1, Y1], [X2, Y2] ) :- Y1 = Y2, adj( X1, X2 ).

possible_go(X,Y):-
	\+ visited(X,Y),
	adjacent([X,Y],[X_t,Y_t]),
	visited(X_t,Y_t).

candidate(X,Y):-
    validCol(X),
	validRow(Y),
	possible_go(X,Y).

safe(X,Y):- noWumpus(X,Y), noPit(X,Y).

noWumpus(X,Y):- 
    adjacent([X,Y],[X_t,Y_t]),
	visited(X_t,Y_t),
	\+ stench(X_t,Y_t).

noPit(X,Y):-
    adjacent([X,Y],[X_t,Y_t]),
	visited(X_t,Y_t),
	\+ breeze(X_t,Y_t).

move(X,Y):- candidate(X,Y), safe(X,Y).

ww_move(X,Y):- candidate(X,Y), safe(X,Y).