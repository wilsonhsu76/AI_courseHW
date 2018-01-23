% prob4
% homework hint divisible. Also used INTexpr1//INTexpr2 INT divide

divisible(N,X) :- M is N mod X, M == 0.
factor(N, ResList):- factor(N, ResList, 2).
factor(1,[],_).
factor(N, [Test|T], Test):-
    N > 0,
	Test >= 2,
	Test =< N,
	divisible(N,Test),
	Quotient is N//Test,
	factor(Quotient, T, Test).
factor(N, ResList, Test):-
    N > 0,
	Test >= 2,
	Test =< N,
	\+ divisible(N,Test),
	TestNext is Test+1,
	factor(N, ResList, TestNext).