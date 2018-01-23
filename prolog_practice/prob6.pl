% prob6
% Y is the returned answer, NewX is new X by updated equation, 0.0001 is threshold(acceptable error)

sin_zero(X,Y):-
    NewX is (X - sin(X)/cos(X)),
	-0.0001 < sin(NewX),
	sin(NewX) < 0.0001,
	Y is NewX.

sin_zero(X,Y):- 
    NewX is (X - sin(X)/cos(X)),
	\+ (-0.0001 < sin(NewX)),
	sin_zero(NewX,Y).

sin_zero(X,Y):- 
    NewX is (X - sin(X)/cos(X)),
	\+ (sin(NewX) < 0.0001),
	sin_zero(NewX,Y).
    
