
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sequenceur is
      port ( c1       : in  std_logic_vector (2 downto 0);
             c2       : in  std_logic_vector (2 downto 0);
             lo       : in  std_logic_vector (2 downto 0);
             ck       : in  std_logic;
             empty    : in  std_logic;
             full     : in  std_logic;
             reset    : in  std_logic;
             c1i      : out std_logic;
             c2i      : out std_logic;
             clraccu  : out std_logic;
             hph      : out std_logic;
             loi      : out std_logic;
             s_read   : out std_logic;
             vaccu    : out std_logic;
             vbreg    : out std_logic;
             vhadout  : out std_logic;
             s_write  : out std_logic );
end sequenceur;


architecture behavioral of sequenceur is

  type etat_type is (E0,E1,E2,E3,E4,E5,E6,E7,E8,E9,E10);
  signal next_state,current_state : etat_type;

begin
  
process (current_state,reset,empty,full,c1,c2,lo)
 begin
   if (reset='1') then
   
     next_state <= E0;

     s_read  <= '0';
     s_write <= '0';
     clraccu <= '1';
     hph     <= '0';
     vhadout <= '0';
     vaccu   <= '0';
     vbreg   <= '0';
     c1i     <= '0';
     c2i     <= '0';
     loi     <= '0';

   else
      case current_state is

        when E0 =>

          next_state <= E1;

          s_read     <= '0';
          s_write    <= '0';
          clraccu    <= '1';
          hph        <= '0';
          vhadout    <= '0';
          vaccu      <= '0';
          vbreg      <= '0';
          c1i        <= '0';
          c2i        <= '0';
          loi        <= '0';

        when E1 =>

          if (empty = '1') then next_state <= E1;
                           else next_state <= E2;
          end if; 

          s_read  <= '1';
          s_write <= '0';
          clraccu <= '0';
          hph     <= '0';
          vhadout <= '0';
          vaccu   <= '0';
          vbreg   <= '0';
          c1i     <= '0';
          c2i     <= '0';
          loi     <= '0';
          
        when E2 =>

          if (c2 = "111") then next_state <= E3;
                          else next_state <= E1;
          end if;

          s_read  <= '0';
          s_write <= '0';
          clraccu <= '0';
          hph     <= '0';
          vhadout <= '0';
          vaccu   <= '0';
          vbreg   <= '0';
          c1i     <= '0';
          c2i     <= '1';
          loi     <= '0';

        when E3 =>

          if (c1 = "111") then next_state <= E4;
                          else next_state <= E1;
          end if;

          s_read  <= '0';
          s_write <= '0';
          clraccu <= '0';
          hph     <= '0';
          vhadout <= '0';
          vaccu   <= '0';
          vbreg   <= '0';
          c1i     <= '1';
          c2i     <= '0';
          loi     <= '0';

        when E4 =>

          if (c1 = "110") then next_state <= E5;
                          else next_state <= E4;
          end if;

          s_read  <= '0';
          s_write <= '0';
          clraccu <= '0';
          hph     <= '1';
          vhadout <= '0';
          vaccu   <= '1';
          vbreg   <= '0';
          c1i     <= '1';
          c2i     <= '0';
          loi     <= '0';

        when E5 =>

          if ( c2 = "111" ) then next_state <= E6;
                            else next_state <= E4;
          end if;

          s_read    <= '0';
          s_write   <= '0';
          clraccu <= '1';
          hph     <= '1';
          vhadout <= '0';
          vaccu   <= '0';
          vbreg   <= '1';
          c1i     <= '1';
          c2i     <= '1';
          loi     <= '0';

        when E6 =>

          if (c2 = "111") then next_state <= E7;
                          else next_state <= E6;
          end if;

          s_read  <= '0';
          s_write <= '0';
          clraccu <= '0';
          hph     <= '0';
          vhadout <= '0';
          vaccu   <= '1';
          vbreg   <= '0';
          c1i     <= '0';
          c2i     <= '1';
          loi     <= '0';

        when E7 =>

          next_state <= E8;

          s_read  <= '0';
          s_write <= '0';
          clraccu <= '0';
          hph     <= '0';
          vhadout <= '1';
          vaccu   <= '0';
          vbreg   <= '0';
          c1i     <= '0';
          c2i     <= '0';
          loi     <= '0';

        when E8 =>

          if (full = '1') then next_state <= E8;
                          else next_state <= E9;
          end if;

          s_read  <= '0';
          s_write <= '1';
          clraccu <= '1';
          hph     <= '0';
          vhadout <= '0';
          vaccu   <= '0';
          vbreg   <= '0';
          c1i     <= '0';
          c2i     <= '0';
          loi     <= '0';

        when E9 =>

          if (c1 = "111") then next_state <= E10;
          else                 next_state <= E6;
          end if;

          s_read  <= '0';
          s_write <= '0';
          clraccu <= '1';
          hph     <= '0';
          vhadout <= '0';
          vaccu   <= '0';
          vbreg   <= '0';
          c1i     <= '1';
          c2i     <= '0';
          loi     <= '0';

        when E10 =>

          if(lo = "111") then next_state <= E0;
                         else next_state <= E4;
          end if;
 

          s_read  <= '0';
          s_write <= '0';
          clraccu <= '1';
          hph     <= '0';
          vhadout <= '0';
          vaccu   <= '0';
          vbreg   <= '0';
          c1i     <= '0';
          c2i     <= '0';
          loi     <= '1';

      end case;
   end if;
end process;

process(ck)
  begin
    if (ck='0' and ck'event) then
      current_state <= next_state;
    end if;
end process;

end behavioral;

