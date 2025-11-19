IMPLEMENTATION MODULE FileSystem;
  IMPORT SeqFile, TextIO, IOConsts, IOResult;

  PROCEDURE Lookup(VAR f: File; name: ARRAY OF CHAR; new: BOOLEAN);
    VAR oRes: SeqFile.OpenResults;
  BEGIN
    IF new THEN
      f.res := error
    ELSE
      SeqFile.OpenRead(f.chan, name, SeqFile.read + SeqFile.old + SeqFile.text, oRes);
      IF oRes = SeqFile.opened THEN
        IF SeqFile.IsSeqFile(f.chan) THEN
          f.res := done
        ELSE
          SeqFile.Close(f.chan);
          f.res := nonSequential
        END
      ELSIF oRes = SeqFile.noSuchFile THEN
        f.res := noSuchFile 
      ELSE
        f.res := error
      END
    END
  END Lookup;

  PROCEDURE ReadChar(VAR f: File; VAR ch: CHAR);
    VAR result: IOConsts.ReadResults;
  BEGIN
    TextIO.ReadChar(f.chan, ch);
    result := IOResult.ReadResult(f.chan);
    IF result = IOConsts.endOfInput THEN
      ch := 0C
    ELSIF result = IOConsts.endOfLine THEN
      TextIO.SkipLine(f.chan);
      ch := 12C
    END
  END ReadChar;

  PROCEDURE Close(VAR f: File);
  BEGIN
    SeqFile.Close(f.chan)
  END Close;

END FileSystem.
