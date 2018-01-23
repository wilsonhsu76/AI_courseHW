% prob7
% query: sol_SendMoreMoney(S,E,N,D,M,O,R,Y).

validDigit(X) :- member(X,[0,1,2,3,4,5,6,7,8,9]).
validHead(X) :- member(X,[1,2,3,4,5,6,7,8,9]).

checkNoDup([]).
checkNoDup([H|T]):- \+member(H,T), checkNoDup(T).

% debug and test for simple case...
% sol_SendMoreMoney(D, E, Y) :-
%    validHead(E),
%	validDigit(D),
%	validDigit(Y),
%	checkNoDup([E,D,Y]),
%	% write('debug: '), write(E),write(', '),write(D), write(', '),write(Y),nl,
%    R1 is ((D + E) mod 10), R1 == Y,
%	% write('debug2: '), write(E),write(', '),write(D), write(', '),write(Y), write(', '),write(R),nl,
%	C1 is ((D + E)//10),
%	% write('debug3: '), write(E),write(', '),write(D), write(', '),write(Y), write(', '),write(C1),nl,
%	E == C1.

% In this question, M should be 1
sol_SendMoreMoney(S,E,N,D,M,O,R,Y) :-
    M is 1,
    validHead(S),
    validDigit(E),
    validDigit(N),
    validDigit(D),
    validDigit(O),
    validDigit(R),
    validDigit(Y),
    checkNoDup([S,M,E,N,D,O,R,Y]),
    R1 is ((D + E) mod 10), R1==Y,
    C1 is ((D + E)//10),
    R2 is ((N + R + C1) mod 10), R2==E,
    C2 is ((N + R + C1)//10),
    R3 is ((C2 + E + O) mod 10), R3==N,
    C3 is ((C2 + E + O)//10),
    R4 is ((C3 + S + M) mod 10), R4==O,
	C4 is ((C3 + S + M) // 10),  C4==M.
	