library IEEE;
use IEEE.std_logic_1164.all;

entity Sn is  -- subtractor multiplexor 
  port ( x  : in  std_logic;
         b  : in  std_logic;
         bo : out std_logic);
end Sn;

architecture circuits of Sn is
begin  -- circuits of Sn
  bo <= (not x) nand b;  -- complemented
end circuits;  -- of Sn
