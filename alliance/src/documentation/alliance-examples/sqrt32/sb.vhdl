library IEEE;
use IEEE.std_logic_1164.all;

entity Sb is 
  port ( x  : in  std_logic;
         y  : in  std_logic;
         b  : in  std_logic;
         bo : out std_logic);
end Sb;

architecture circuits of Sb is
  signal t011, t111, t010, t001 : std_logic;
begin  -- circuits of Sb
  t011 <= (not x) and y and b;
  t111 <= x and y and b;
  t010 <= (not x) and y and (not b);
  t001 <= (not x) and (not y) and b;
  bo   <= t011 or t111 or t010 or t001;
end circuits;  -- of Sb
