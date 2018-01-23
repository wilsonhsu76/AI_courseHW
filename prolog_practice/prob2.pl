% prob2
% request_p2(X), which is a predicate, is designed for query to find all surgeons who live in Texas and make over 100000/yr

% database
occupation(joe, oral_surgeon).
occupation(sam, patent_lawyer).
occupation(bill, trail_lawyer).
occupation(cindy, investment_laywer).
occupation(joan, civil_lawyer).
occupation(len, plastic_surgeon).
occupation(lance, heart_surgeon).
occupation(frank, brain_surgeon).
occupation(charlie, plastic_surgeon).
occupation(lisa, oral_surgeon).

address(joe, houston).
address(sam, pittsburgh).
address(bill,dallas).
address(cindy,omaha).
address(joan,chicago).
address(len,college_station).
address(lance, los_angeles).
address(frank, dallas).
address(charlie,houston).
address(lisa,san_antonio).

salary(joe,50000).
salary(sam,150000).
salary(bill,200000).
salary(cindy,140000).
salary(joan,80000).
salary(len,70000).
salary(lance,650000).
salary(frank,85000).
salary(charlie,120000).
salary(lisa, 190000).

% additional data city-state, type of surgeons
atTexas(X):- address(X, Y),(Y == houston).
atTexas(X):- address(X, Y),(Y == dallas).
atTexas(X):- address(X, Y),(Y == college_station).
atTexas(X):- address(X, Y),(Y == san_antonio).

surgeons(X):- occupation(X,Y),(Y == oral_surgeon).
surgeons(X):- occupation(X,Y),(Y == plastic_surgeon).
surgeons(X):- occupation(X,Y),(Y == heart_surgeon).
surgeons(X):- occupation(X,Y),(Y == brain_surgeon).

% a query for p2.
request_p2(X):- atTexas(X), surgeons(X), salary(X,Y), Y>100000 .

