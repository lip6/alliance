library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;


entity AddAccu is

    port ( CLK : in Std_Logic;
           CLR : in Std_Logic;
           LD  : in Std_Logic;
           A   : in Std_Logic_Vector(15 downto 0) ;
           RESULT : out Std_Logic_Vector(15 downto 0) );

end AddAccu;


architecture DataFlow OF AddAccu is
signal resultint : Std_Logic_Vector(15 downto 0) ;
begin
process (CLK)
begin
   if CLK'event and CLK='1' then
      if CLR = '1' then resultint  <= ( others => '0' );
      elsif LD = '1' then resultint <= resultint + A;
      end if;
 end if;
end process;
RESULT <= resultint;
end DataFlow;
