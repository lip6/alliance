library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;

-- port declaration

entity addaccu4 is 
port ( a : in std_logic_vector (3 downto 0);
       b : in std_logic_vector (3 downto 0);
     sel : in std_logic;
      ck : in std_logic;
     vdd : in std_logic;
     vss : in std_logic;
       s : inout std_logic_vector (3 downto 0)
     );
end addaccu4;

-- architecture body

architecture data_flow of addaccu4 is

signal mux_out : std_logic_vector (3 downto 0);
signal reg_out : std_logic_vector (3 downto 0);

begin 

  mux_out <= a when sel='0' else reg_out;
  s <= b + mux_out;

process ( ck )
begin
   if ( ck'event and ck ='1' ) then reg_out <= s;
   end if;
end process;

end data_flow;
   
