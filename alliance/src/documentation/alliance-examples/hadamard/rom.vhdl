
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;
use IEEE.STD_LOGIC_arith.ALL;

entity rom is
      port ( c1  : in  std_logic_vector (2 downto 0);
             c2  : in  std_logic_vector (2 downto 0);
             hph : in  std_logic;
             lo  : in  std_logic_vector (2 downto 0);
             ope : out std_logic );
end rom;

architecture behavioral of rom is

   signal adr : std_logic_vector (5 downto 0);
   constant rom_data : std_logic_vector(0 to 63) := "0000000000001111001111000011001101100110011010010101101001010101";
   
   begin
     
     adr <= c2(2 downto 0)&c1(2 downto 0) when hph='0' else
            lo(2 downto 0)&c1(2 downto 0) ;
             
     ope <= rom_data(conv_integer(Adr));
end behavioral;
