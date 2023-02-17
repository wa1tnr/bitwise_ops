hex
: binary ( -- ) 2 base ! ;
: octal ( -- ) 8 base ! ;

binary

decimal

: set7? ( n -- BOOL )
  abs $80 AND $80 = ;

: set6? ( n -- BOOL )
  abs $40 AND $40 = ;

: set5? ( n -- BOOL )
  abs $20 AND $20 = ;

: set4? ( n -- BOOL )
  abs $10 AND $10 = ;

: set3? ( n -- BOOL )
  abs $08 AND $08 = ;

: set2? ( n -- BOOL )
  abs $04 AND $04 = ;

: set1? ( n -- BOOL )
  abs $02 AND $02 = ;

: set0? ( n -- BOOL )
  abs $01 AND $01 = ;

binary

: prebin
  dup set7?  dup IF ." bit 7 is set!   MSB is set! " cr drop drop exit THEN
  drop ." bit 7 is clr!   MSB is clr! " cr

  dup set6?  dup IF ." bit 6 is set! " cr drop drop exit THEN
  drop ." bit 6 is clr! " cr
  
  dup set5?  dup IF ." bit 5 is set! " cr drop drop exit THEN
  drop ." bit 5 is clr! " cr

  dup set4?  dup IF ." bit 4 is set! " cr drop drop exit THEN
  drop ." bit 4 is clr! " cr

  dup set3?  dup IF ." bit 3 is set! " cr drop drop exit THEN
  drop ." bit 3 is clr! " cr

  dup set2?  dup IF ." bit 2 is set! " cr drop drop exit THEN
  drop ." bit 2 is clr! " cr

  dup set1?  dup IF ." bit 1 is set! " cr drop drop exit THEN
  drop ." bit 1 is clr! " cr

  dup set0?  dup IF ." bit 0 is set! " cr drop drop exit THEN
  drop ." bit 0 is clr! " cr
  cr
  ." is there underflow yet or does this line print prior " cr
  drop
  cr ." control reaches end of prebin.  exiting. check the stack. " cr
;

decimal

: test
  depth 1 < IF ." bad" EXIT THEN
  dup   0 < IF ." bad" EXIT THEN
  dup 255 > IF ." bad" EXIT THEN
  cr
  prebin
;


."  Just type an int 0 to 255 and type test after it and press ENTER" cr cr
."  check the stack after. " cr
\ END

