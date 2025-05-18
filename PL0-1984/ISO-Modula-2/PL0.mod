MODULE PL0;  (*NW WS 83/84*)
  FROM Terminal IMPORT Read;
  FROM FileSystem IMPORT Lookup, Response, Close;
  FROM TextWindows IMPORT Window,
       OpenTextWindow, Write, WriteLn, WriteString, CloseTextWindow;
  FROM PL0Scanner IMPORT InitScanner, source, CloseScanner;
  FROM PL0Parser IMPORT Parse, noerr, EndParser;
  FROM PL0Generator IMPORT InitGenerator, EndGenerator;
  FROM PL0Interpreter IMPORT Interpret, EndInterpreter;

  CONST NL = 27; (*max file name length*)

  VAR ch: CHAR;
      win: Window;
      FileName: ARRAY [0 .. NL] OF CHAR;

  PROCEDURE ReadName;
    VAR i: CARDINAL;
  BEGIN Read(ch); FileName := "";
    i := 0;
    WHILE (CAP(ch) >= "A") & (CAP(ch) <= "Z")
       OR (ch >= "0") & (ch <= "9")
       OR (ch = ".") DO
      IF i < NL THEN
        FileName[i] := ch; i := i + 1
      END;
      Read(ch)
    END;
    IF (3 < i) & (i < NL) & (FileName[i - 1] = ".") THEN
      FileName[i] := "p"; i := i + 1;
      FileName[i] := "l"; i := i + 1;
      FileName[i] := "0"; i := i + 1; WriteString(win, "pl0")
    END;
    FileName[i] := 0C;
    IF (ch = 12C) & (i = 0) THEN ch := 33C END 
  END ReadName;

BEGIN OpenTextWindow(win, 0, 0, 704, 66, "DIALOG");
  LOOP WriteString(win, "in> "); ReadName;
    IF ch = 33C THEN EXIT END;
    Lookup(source, FileName, FALSE);
    IF source.res = done THEN
      InitScanner; InitGenerator; Parse; Close(source);
      IF noerr THEN
        WriteString(win, " interpreting"); Interpret
      ELSE WriteString(win, " incorrect")
      END
    ELSE WriteString(win, " not found")
    END;
    WriteLn(win)
  END;
  CloseScanner; EndParser; EndGenerator; EndInterpreter;
  CloseTextWindow(win)
END PL0.
