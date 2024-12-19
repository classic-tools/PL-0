IMPLEMENTATION MODULE TextWindows;
  IMPORT SYSTEM, Strings, IOChan, StdChans, TextIO, WholeIO, SeqFile;

  CONST NL = 27;

  TYPE Name = ARRAY [0 .. NL] OF CHAR;
       TextWindow = RECORD
         channel: IOChan.ChanId;
         stdChannel, open: BOOLEAN;
         name: Name;
       END;

  VAR stdin, stdout: IOChan.ChanId;
      windowId: CARDINAL;
      windows: ARRAY [0 .. 4] OF TextWindow;

  PROCEDURE OpenTextWindow(VAR win: Window; x, y, w, h: CARDINAL; name: ARRAY OF CHAR);

    VAR oRes: SeqFile.OpenResults;
        window: TextWindow;

  BEGIN
    win := 5; 
    Done := windowId <= 4;
    IF Done THEN
      Strings.Assign(name, window.name);
      window.stdChannel := Strings.Equal(window.name, "RESULT") OR Strings.Equal(window.name, "DIALOG");
      IF window.stdChannel THEN
        window.channel := stdout;
        window.open := TRUE;
        windows[windowId] := window;
        win := windowId;
        windowId := windowId + 1
      ELSE
        Done := Strings.Length(window.name) < NL - 4;
        IF Done THEN
          Strings.Append(".txt", window.name);
          SeqFile.OpenWrite(window.channel, window.name, SeqFile.write + SeqFile.text, oRes);
          Done := (oRes = SeqFile.opened) & SeqFile.IsSeqFile(window.channel); 
          IF Done THEN
            window.open := TRUE;
            windows[windowId] := window;
            win := windowId;
            windowId := windowId + 1
          ELSE
            SeqFile.Close(window.channel)
          END
        END     
      END
    END
  END OpenTextWindow;

  PROCEDURE ReadInt(win: Window; VAR n: INTEGER);
  BEGIN
    Done := (win <= 4) & windows[win].open & windows[win].stdChannel;
    IF Done THEN
      WholeIO.ReadInt(stdin, n);
      TextIO.SkipLine(stdin)
    END
  END ReadInt;

  PROCEDURE Write(win: Window; ch: CHAR);
  BEGIN
    Done := (win <= 4) & windows[win].open;
    IF Done THEN
      IF (ch = 14C) & windows[win].stdChannel THEN
        TextIO.WriteLn(windows[win].channel)
      ELSIF ch # 14C THEN
        TextIO.WriteChar(windows[win].channel, ch)
      END
    END
  END Write;

  PROCEDURE WriteString(win: Window; s: ARRAY OF CHAR);
  BEGIN
    Done := (win <= 4) & windows[win].open;
    IF Done THEN
      TextIO.WriteString(windows[win].channel, s)
    END
  END WriteString;
  
  PROCEDURE WriteCard(win: Window; n, w: CARDINAL);
  BEGIN
    Done := (win <= 4) & windows[win].open;
    IF Done THEN
      WholeIO.WriteCard(windows[win].channel, n, w)
    END
  END WriteCard;

  PROCEDURE WriteInt(win: Window; n: INTEGER; w: CARDINAL);
  BEGIN
    Done := (win <= 4) & windows[win].open;
    IF Done THEN
      WholeIO.WriteInt(windows[win].channel, n, w)
    END
  END WriteInt;

  PROCEDURE WriteLn(win: Window);
  BEGIN
    Done := (win <= 4) & windows[win].open;
    IF Done THEN
      TextIO.WriteLn(windows[win].channel)
    END
  END WriteLn;

  PROCEDURE Invert(win: Window; on: BOOLEAN);
  BEGIN
    Done := (win <= 4) & windows[win].open & windows[win].stdChannel;
    IF Done & on THEN
      TextIO.WriteChar(windows[win].channel, ' ')
    END
  END Invert;

  PROCEDURE CloseTextWindow(VAR win: Window);
  BEGIN
    Done := (win <= 4) & windows[win].open;
    IF Done THEN
      IF NOT windows[win].stdChannel THEN
        SeqFile.Close(windows[win].channel)
      END;
      windows[win].open := FALSE
    END
  END CloseTextWindow;

  VAR i: CARDINAL;

BEGIN
  stdin := StdChans.StdInChan();
  stdout := StdChans.StdOutChan();
  windowId := 0;
  FOR i := 0 TO 4 DO
    windows[i].open := FALSE
  END
END TextWindows.
