
in       dividend  (6  downto 0) X;;;
in       divisor   (3  downto 0) X;;;
out      quotient  (3 downto 0) X;;;
out      remainder (3 downto 0) X;;;
in       vss B;;
in       vdd B;;

begin

-- Pattern description :
--         d  d   q  r  V V
--         i  i   u  e  V V
--         v  v   o  m  V V
--         i  i   t  a  V V
--         d  s   i  i  V V

< +10ns>: 12  3  ?* ?*  0 1;
< +10ns>: 12  3  ?* ?*  0 1;
< +10ns>: 17  3  ?* ?*  0 1;
< +10ns>: 17  3  ?* ?*  0 1;
                        
end;
