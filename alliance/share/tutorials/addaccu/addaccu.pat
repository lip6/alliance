in vdde ;
in vsse ;
in vdd;
in vss;
in a(3 to 0);
in b(3 to 0);
in sel;;
in ck;;;
out s(3 to 0);
begin

# a=0, b=3, sel=direct input, we expect value 3 on s
pat_1 : 1 0 1 0  0000  0011  0 0  ?0011 ;
# a=1, b=3, sel=direct input, we expect value 4 on s
pat_2 : 1 0 1 0  0001  0011  0 0  ?0100 ;
# a=7, b=3, sel=direct input, we expect value bin 1010 on s
pat_3 : 1 0 1 0  0111  0011  0 0  ?1010 ;
# a=7, b=3, sel=direct input, we set ck to 1, we still expect bin 1010 on s 
pat_4 : 1 0 1 0  0111  0011  0 1  ?1010 ;
# when ck falls to 0, the computed value (7+3=bin 1010) is stored in accu
pat_5 : 1 0 1 0  0111  0011  0 0  ?1010 ;
# if we choose sel=accu output, we expect value bin 1010 + 4= bin 1110 on s 
# even if a=1111
pat_6 : 1 0 1 0  1111  0100  1 0  ?1110 ;
# back to normal mode, 5+4=9
pat_7 : 1 0 1 0  0101  0100  0 0  **** ;
end;
