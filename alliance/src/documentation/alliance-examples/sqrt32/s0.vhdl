library IEEE;
use IEEE.std_logic_1164.all;

entity S0 is  
  port ( x  : in  std_logic;
         u  : in  std_logic;
         d  : out std_logic;
         bo : out std_logic);
end S0;

architecture circuits of S0 is
begin  -- circuits of S0
  bo   <= not x;
  d    <= not x when u='1' else x;  
end circuits;  -- of S0

