% prob1:

parent(bart, homer).
parent(bart, marge).
parent(lisa, homer).
parent(lisa, marge).
parent(maggie, homer).
parent(maggie, marge).
parent(homer, abraham).
parent(herb, abraham).
parent(tod, ned).
parent(rod, ned).
parent(marge, jackie).
parent(patty, jackie).
parent(selma, jackie).

female(maggie).
female(lisa).
female(marge).
female(patty).
female(selma).
female(jackie).

male(bart).
male(homer).
male(herb).
male(burns).
male(smithers).
male(tod).
male(rod).
male(ned).
male(abraham).

brother(X,Y):- male(Y), parent(X,Z), parent(Y,Z), \+(X==Y).
sister(X,Y):- female(Y), parent(X,Z), parent(Y,Z), \+(X==Y).
aunt(X,Y):- parent(X,Z), sister(Z,Y).
uncle(X,Y):- parent(X,Z), brother(Z,Y).
grandfather(X,Y):- male(Y), parent(X,Z), parent(Z,Y).
grandmother(X,Y):- female(Y), parent(X,Z), parent(Z,Y).
granddaughter(X,Y):- grandmother(Y,X).
ancestor(X,Y):- parent(X,Y).
ancestor(X,Z):- parent(X,Y), parent(Y,Z).

descendant(X,Y):- ancestor(Y,X).


unrelated(X,Y):- \+brother(X,Y), \+sister(X,Y), \+aunt(X,Y), \+uncle(X,Y), \+ancestor(X,Y), \+descendant(X,Y).




