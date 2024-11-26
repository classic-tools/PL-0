**const** m = 7, n = 85;\
**var** x, y, z, q, r;\
**procedure** multiply;\
&ensp;&ensp;**var** a, b;\
**begin** a := x; b := y; z := 0;\
&ensp;&ensp;**while** b > 0 **do**\
&ensp;&ensp;**begin**\
&ensp;&ensp;&ensp;&ensp;**if** **odd** b **then** z := z + a;\
&ensp;&ensp;&ensp;&ensp;a := 2 * a; b := b / 2\
&ensp;&ensp;**end**\
**end**;\
**procedure** divide;\
&ensp;&ensp;**var** w;\
**begin**\
&ensp;&ensp;r := x; q := 0; w := y;\
&ensp;&ensp;**while** w &le; r **do** w := 2 * w;\
&ensp;&ensp;**while** w > y **do**\
&ensp;&ensp;**begin** q := 2 * q; w := w / 2;\
&ensp;&ensp;&ensp;&ensp;**if** w &le; r **then**\
&ensp;&ensp;&ensp;&ensp;**begin** r := r - w; q := q + 1\
&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;**end**\
**end**;\
**procedure** gcd;\
&ensp;&ensp;**var** f, g;\
**begin** f := x; g := y;\
&ensp;&ensp;**while** f &ne; g **do**\
&ensp;&ensp;**begin** **if** f < g **then** g := g - f;\
&ensp;&ensp;&ensp;&ensp;**if** g < f **then** f := f - g\
&ensp;&ensp;**end**;\
&ensp;&ensp;z := f\
**end**;\
**begin**\
&ensp;&ensp;x := m; y := n; **call** multiply;\
&ensp;&ensp;y := 25; y := 3; **call** divide;\
&ensp;&ensp;x := 84; y := 36; **call** gcd\
**end**.  
 
