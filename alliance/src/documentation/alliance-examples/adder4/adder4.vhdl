library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;


entity Adder4 is

    port ( A   : in Std_Logic_Vector(3 downto 0) ;
           B   : in Std_Logic_Vector(3 downto 0) ;
           RESULT : out Std_Logic_Vector(3 downto 0) );

end Adder4;


architecture DataFlow OF Adder4 is
begin

   RESULT <= std_logic_vector( unsigned(A) + unsigned(B) );

end DataFlow;
