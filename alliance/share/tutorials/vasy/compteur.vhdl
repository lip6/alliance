
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;

entity compteur is
      port ( c1i   : in  std_logic;
             c2i   : in  std_logic;
             loi   : in  std_logic;
             c1    : out std_logic_vector (2 downto 0) ;
             c2    : out std_logic_vector (2 downto 0) ;
             lo    : out std_logic_vector (2 downto 0) ;
             reset : in  std_logic;
             ck    : in  std_logic );
end compteur;

architecture behavioral of compteur is

   signal rc1 :  integer range 0 to 7;
   signal rc2 :  integer range 0 to 7;
   signal rlo :  integer range 0 to 7;

begin

  process (ck)
  begin
    if (ck='0' and ck'event) then
      if (reset='1') then rc1<=0; rc2<=0; rlo<=0;
      else
        if c1i='1' then rc1 <= (rc1 + 1) mod 8;
                   else rc1 <= rc1;
        end if;
        if c2i='1' then rc2 <= (rc2 + 1) mod 8;
                   else rc2 <= rc2;
        end if;
        if loi='1' then rlo <= (rlo + 1) mod 8; 
                   else rlo <= rlo; 
        end if;  
      end if;
    end if; 
  end process;


  c1 <= (conv_std_logic_vector(rc1,c1'length));
  c2 <= (conv_std_logic_vector (rc2,c2'length));  
  lo <= (conv_std_logic_vector (rlo,lo'length));

end behavioral;

