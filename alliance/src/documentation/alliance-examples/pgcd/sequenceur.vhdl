
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sequenceur is
      port ( ck        : in  std_logic;
             empty     : in  std_logic;
             full      : in  std_logic;
             reset     : in  std_logic;
	     ra_eq_rb  : in  std_logic;
	     ra_inf_rb : in  std_logic;
             wres_ra   : out std_logic;
             wres_rb   : out std_logic;
             ra_sub_rb : out std_logic;
             s_write   : out std_logic;
	     s_read    : out std_logic  );
end sequenceur;


architecture behavioral of sequenceur is
	type t_etat is (E1, E2, E3, E4);
	signal ec, ef : t_etat;
	

begin
  
	process(ck)
	begin
		if(rising_edge(ck)) then
			if(reset = '1') then ec <= E1;
			else ec <= ef;
			end if;
		end if;
	end process;

	
	process(ec, ra_eq_rb, ra_inf_rb, empty, full)
	begin
             wres_ra   <= '0';
             wres_rb   <= '0';
             ra_sub_rb <= '0';
             s_write   <= '0';
	     s_read    <= '0';

		case(ec) is
			when E1 =>
				if(empty = '1')	then ef <= E1;
				else ef <= E2;
				end if;
				s_read <= '1';
			when E2 =>
				if( ra_eq_rb = '1') then ef <= E4;
				else ef <= E3;
				end if;
			when E3 =>
				wres_rb   <= ra_inf_rb;
				wres_ra   <= not(ra_inf_rb);
				ra_sub_rb <= not(ra_inf_rb);
				ef <= E2;
			when E4 =>
				if(full = '1') then ef <= E4;
				else ef <= E1;
				end if;
				s_write <= '1';
		end case;
	end process;

end behavioral;

