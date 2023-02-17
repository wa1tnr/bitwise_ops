hex
: binary ( -- ) 2 base ! ;
: octal ( -- ) 8 base ! ;

binary

: give_127 1 drop ;

: seeded dup dup dup ;

: twospace ( -- ) space space ;

decimal

: set8? dup abs $80 AND $80 = ;

: set7? dup abs $40 AND $40 = ;
;

binary

: prebin
  set8?
;

: test
  depth 1 < IF ." bad" EXIT THEN

  prebin
  dup IF ." MSB is set! " cr drop drop exit THEN
  drop drop
  ." MSB is clr! " cr
;

decimal

\ UNTESTED EDIT, END

