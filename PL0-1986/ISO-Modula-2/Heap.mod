IMPLEMENTATION MODULE Heap;
  IMPORT SYSTEM, Storage;

  PROCEDURE ALLOCATE(VAR a: SYSTEM.ADDRESS; n: CARDINAL);
  BEGIN
    Storage.ALLOCATE(a, n)
  END ALLOCATE;

  PROCEDURE ResetHeap(a: SYSTEM.ADDRESS);
  BEGIN
  END ResetHeap;

END Heap.
