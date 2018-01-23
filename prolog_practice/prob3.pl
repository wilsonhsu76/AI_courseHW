%prob3
%remdups([1,3,4,2,4,3,6,8,6,5,4,2,3,4,9],X)
%ResList: Residue List
remdups([],[]).
remdups([H|T],ResList):- member(H, T), remdups(T,ResList).
remdups([H|T],[H|T1]):- \+member(H,T), remdups(T,T1).
%[H|T1] makes X with H elements in the head. 

