library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;

entity Srb is

    port ( Shift : in Std_Logic ;
           LD : in Std_Logic ;
           CLK : in Std_Logic ;
           B : in Std_Logic_Vector(7 downto 0);

           OUTS : out Std_Logic_Vector(15 downto 0));

end Srb;


----------------------------------------------------------------------

architecture DataFlow OF Srb is
signal outsint : Std_Logic_Vector(15 downto 0);

begin
process (CLK)
begin
     if CLK'event and CLK='0' then
         if LD = '1' then outsint <= 
	    B(7)&B(7)&B(7)&B(7)&B(7)&B(7)&B(7)&B(7)&B ;
         elsif Shift= '1' then  
              outsint(15 downto 0) <= outsint(14 downto 0) & '0';
	 else outsint <= outsint;
         end if;
     end if;
end process;

OUTS <= outsint;

end DataFlow;


----------------------------------------------------------------------
