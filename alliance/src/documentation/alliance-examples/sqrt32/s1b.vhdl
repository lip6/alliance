library IEEE;
use IEEE.std_logic_1164.all;

entity S1b is  
  port ( x  : in  std_logic;
         b  : in  std_logic;
         bo : out std_logic);
end S1b;

architecture circuits of S1b is
begin  -- circuits of S1b
  bo <= (not x) and b;
end circuits;  -- of S1b
