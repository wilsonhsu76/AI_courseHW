% prob5
% if List X includes exactly C1 ones, check return true.
% bitvec to generate possible combinations of bit vector.
% code: problem request.

bitvec(0,[]).

bitvec(N,[H|T]):- 
    N > 0,
	H is 0,
	NewN is N-1,
	bitvec(NewN, T).

bitvec(N,[H|T]):- 
    N > 0,
	H is 1,
	NewN is N-1,
	bitvec(NewN, T).

code(N,C1,X):-
    bitvec(N,X),
	check(X,C1).

check([],0).
check([H|L],Counts_1) :- H==1, DeCounts_1 is Counts_1 - 1, check(L,DeCounts_1).
check([H|L],Counts_1) :- H==0, check(L,Counts_1).




	