library IEEE;
use IEEE.std_logic_1164.all;

entity S0b is  
  port ( x  : in  std_logic;
         bo : out std_logic);
end S0b;

architecture circuits of S0b is
begin  -- circuits of S0b
  bo <= not x;
end circuits;  -- of S0b
