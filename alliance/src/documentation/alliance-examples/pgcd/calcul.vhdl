library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;

entity calcul is 
      port ( ck        : in std_logic;
             a         : in std_logic_vector(7 downto 0);
             b         : in std_logic_vector(7 downto 0);
             res       : out std_logic_vector(7 downto 0);
	     ra_eq_rb  : out  std_logic;
	     ra_inf_rb : out  std_logic;
             wres_ra   : in std_logic;
             wres_rb   : in std_logic;
             ra_sub_rb : in std_logic;
	     s_read    : in std_logic  );
end calcul;



architecture behavioral of calcul is

	signal RA      : std_logic_vector(7 downto 0);
	signal RB      : std_logic_vector(7 downto 0);
	signal RES_int : std_logic_vector(7 downto 0);

begin

  process( ck )
  begin
    if ( rising_edge( ck ) )
    then if ( s_read = '1' )     then RA <= A;
	 elsif ( wres_ra = '1' ) then RA <= RES_int;
	 end if;
    end if;
  end process;

  process( ck )
  begin
    if ( rising_edge( ck ) )
    then if ( s_read = '1' )     then RB <= B;
	 elsif ( wres_rb = '1' ) then RB <= RES_int;
	 end if;
    end if;
  end process;

  process( RA, RB, ra_sub_rb )
  begin
    if ( ra_sub_rb = '1' )
    then RES_int <= std_logic_vector(unsigned(RA) - unsigned(RB));
    else RES_int <= std_logic_vector(unsigned(RB) - unsigned(RA));
    end if;
  end process;

  RES <= RA;

  process( RA, RB )
  begin
     if ( unsigned(RA) < unsigned( RB ) )
     then ra_inf_rb <= '1';
     else ra_inf_rb <= '0';
     end if;
  end process;

  process( RA, RB )
  begin
     if ( RA = RB )
     then ra_eq_rb <= '1';
     else ra_eq_rb <= '0';
     end if;
  end process;

end behavioral;
