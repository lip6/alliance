
in       P  (7  downto 0) X;;;
out      U  (3  downto 0) X;;;
in       vss B;;
in       vdd B;;

begin

-- Pattern description :
--         P   U  V V
--         P   U  V V
--         P   U  V V
--         P   U  V V
--         P   U  V V

< +50ns>:  04 ?*  0 1;
< +50ns>:  04 ?*  0 1;
< +50ns>:  10 ?*  0 1;
< +50ns>:  10 ?*  0 1;
< +50ns>:  20 ?*  0 1;
< +50ns>:  20 ?*  0 1;
< +50ns>:  ff ?*  0 1;
< +50ns>:  ff ?*  0 1;
                        
end;
