library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;

entity Sra is

    port ( Shift : in Std_Logic ;
           LD : in Std_Logic ;
	   RST : in Std_Logic;
           CLK : in Std_Logic ;
           A : in Std_Logic_Vector(7 downto 0);

           Done : out Std_Logic ;
           LSB : out Std_Logic  );

end Sra;


----------------------------------------------------------------------

architecture DataFlow OF Sra is

signal outsint : Std_Logic_Vector(7 downto 0);
begin
process (CLK)
begin
     if CLK'event and CLK='0' then
         if RST ='1' then outsint <= (others => '0');
	 elsif LD = '1' then outsint <= A ;
         elsif Shift= '1' then
            outsint(7 downto 0) <= '0' & outsint(7 downto 1);
	else outsint <= outsint;
         end if;
     end if;
end process;

process( outsint )
  variable RESULT : Std_Logic;
begin
   RESULT := '0';
   for j in outsint'range loop
       RESULT := outsint(j) or RESULT;
       exit when RESULT = '1';
   end loop;
   RESULT := not(RESULT);

   Done <= RESULT;
end process;
LSB <= outsint(0);

end DataFlow;


----------------------------------------------------------------------
