IMPLEMENTATION MODULE Terminal;
  IMPORT IOChan, StdChans, TextIO;

  VAR stdin: IOChan.ChanId;

  PROCEDURE Read(VAR ch: CHAR);
  BEGIN
    TextIO.ReadChar(stdin, ch);
    IF ch = 12C THEN TextIO.SkipLine(stdin) END
  END Read;

  PROCEDURE BusyRead(VAR ch: CHAR);
  BEGIN
    ch := 0C
  END BusyRead;

BEGIN
  stdin := StdChans.StdInChan()
END Terminal.
