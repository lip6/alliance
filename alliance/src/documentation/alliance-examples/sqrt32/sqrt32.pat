
in       P  (31  downto 0) X;;;
out      U  (15  downto 0) X;;;
in       vss B;;
in       vdd B;;

begin

-- Pattern description :
--         P         U     V V
--         P         U     V V
--         P         U     V V
--         P         U     V V
--         P         U     V V

< +50ns>:  00000004 ?****  0 1;
< +50ns>:  00000004 ?****  0 1;
< +50ns>:  00000010 ?****  0 1;
< +50ns>:  00000010 ?****  0 1;
< +50ns>:  00000120 ?****  0 1;
< +50ns>:  00000120 ?****  0 1;
< +50ns>:  7fffffff ?****  0 1;
< +50ns>:  7fffffff ?****  0 1;
                        
end;
