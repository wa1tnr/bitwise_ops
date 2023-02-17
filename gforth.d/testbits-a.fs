hex
: binary ( -- ) 2 base ! ;
( \ decimal done )
( \ hex done )
: octal ( -- ) 8 base ! ; ( \ 8 is legal hex for decimal 8 as well )

binary

: give_127 1 drop ; \ rescinded

: seeded ( n -- n n n n )
  dup ( \ base 8, octal )
  dup ( \ base 10, decimal )
  dup ( \ base 16, hexadecimal )
;

: twospace ( -- ) space space ;

decimal

: set7? ( n -- BOOL )
  abs $80 AND $80 = ;

: set6? ( n -- BOOL )
  abs $40 AND $40 = ;

: set5? ( n -- BOOL )
  abs $20 AND $20 = ;

binary

: prebin ( n -- ? ) \ no SED given
  dup set7?  dup IF ." MSB is set! " cr drop drop drop exit THEN
  drop ." MSB is clr! " cr

  dup set6?  dup IF ." bit 6 is set! " cr drop drop exit THEN
  drop ." bit 6 is clr! " cr
  
  dup set5?  dup IF ." bit 5 is set! " cr drop drop exit THEN
  drop ." bit 5 is clr! " cr
  cr ." control reaches end of prebin.  exiting. check the stack. " cr
;

: test ( n -- dont care)
  depth 1 < IF ." bad" EXIT THEN
  prebin
;

: messages ( -- ) \ no SED
  cr
  ."   binary octal decimal hex" cr
  twospace   ." 0b"   binary  .  \ eol
  twospace   ."  Q"    octal  .
  twospace   ."  D"  decimal  .
  twospace   ." 0x"      hex  .
  decimal
;

decimal \ interactive base very first live keystroke in interpreter

\ END

