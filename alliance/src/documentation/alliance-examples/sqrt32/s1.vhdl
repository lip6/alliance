library IEEE;
use IEEE.std_logic_1164.all;

entity S1 is  -- subtractor multiplexor 
  port ( x  : in  std_logic;
         b  : in  std_logic;
         u  : in  std_logic;
         d  : out std_logic;
         bo : out std_logic);
end S1;

architecture circuits of S1 is
  signal t100, t001, td : std_logic;
begin  -- circuits of S1
  t001 <= (not x) and b;
  t100 <= x and (not b);
  bo   <= t001;
  td   <= t100 or t001;
  d    <= td when u='1' else x;  
end circuits;  -- of S1
