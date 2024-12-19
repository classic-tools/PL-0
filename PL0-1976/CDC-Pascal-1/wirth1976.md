CONST M = 7, N = 85;\
VAR X, Y, Z, Q, R;\
PROCEDURE MULTIPLY;\
&ensp;&ensp;VAR A, B;\
BEGIN A := X; B := Y; Z := 0;\
&ensp;&ensp;WHILE B > 0 DO\
&ensp;&ensp;BEGIN\
&ensp;&ensp;&ensp;&ensp;IF ODD B THEN Z := Z + A;\
&ensp;&ensp;&ensp;&ensp;A := 2 * A; B := B / 2\
&ensp;&ensp;END\
END;\
PROCEDURE DIVIDE;\
&ensp;&ensp;VAR W;\
BEGIN\
&ensp;&ensp;R := X; Q := 0; W := Y;\
&ensp;&ensp;WHILE W &le; R DO W := 2 * W;\
&ensp;&ensp;WHILE W > Y DO\
&ensp;&ensp;BEGIN Q := 2 * Q; W := W / 2;\
&ensp;&ensp;&ensp;&ensp;IF W &le; R THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN R := R - W; Q := Q + 1\
&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;END\
END;\
PROCEDURE GCD;\
&ensp;&ensp;VAR F, G;\
BEGIN F := X; G := Y;\
&ensp;&ensp;WHILE F &ne; G DO\
&ensp;&ensp;BEGIN IF F < G THEN G := G - F;\
&ensp;&ensp;&ensp;&ensp;IF G < F THEN F := F - G\
&ensp;&ensp;END;\
&ensp;&ensp;Z := F\
END;\
BEGIN\
&ensp;&ensp;X := M; Y := N; CALL MULTIPLY;\
&ensp;&ensp;Y := 25; Y := 3; CALL DIVIDE;\
&ensp;&ensp;X := 84; Y := 36; CALL GCD\
END.  
