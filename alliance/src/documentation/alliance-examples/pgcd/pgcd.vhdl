library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity pgcd is
  port ( ck       : in    std_logic;
         a        : in    std_logic_vector (7 downto 0);
         b        : in    std_logic_vector (7 downto 0);
         empty    : in    std_logic;
         full     : in    std_logic;
         reset    : in    std_logic;
         res      : out   std_logic_vector (7 downto 0);
         s_read   : out   std_logic;
         s_write  : out   std_logic );
end pgcd;

architecture pgcd of pgcd is

   component sequenceur
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
   end component;

   component calcul
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
   end component;

   signal ra_eq_rb, ra_inf_rb, wres_ra, wres_rb, ra_sub_rb, int_s_read : std_logic;

begin

   s_read <= int_s_read;

   e_sequenceur : sequenceur 
     port map( ck        => ck
             , empty     => empty
             , full      => full
             , reset     => reset
             , ra_eq_rb  => ra_eq_rb
             , ra_inf_rb => ra_inf_rb
             , wres_ra   => wres_ra
             , wres_rb   => wres_rb
             , ra_sub_rb => ra_sub_rb
             , s_read    => int_s_read
             , s_write   => s_write );

   e_calcul : calcul
      port map( ck        => ck
              , a         => a
              , b         => b
              , res       => res
	      , ra_eq_rb  => ra_eq_rb
	      , ra_inf_rb => ra_inf_rb
              , wres_ra   => wres_ra
              , wres_rb   => wres_rb
              , ra_sub_rb => ra_sub_rb
	      , s_read    => int_s_read );

end pgcd;

