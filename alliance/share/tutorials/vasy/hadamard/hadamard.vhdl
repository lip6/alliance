library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity hadamard is
  port ( ck     : in    std_logic;
         data   : in    std_logic_vector (7 downto 0);
         empty  : in    std_logic;
         full   : in    std_logic;
         reset  : in    std_logic;
         hadout : out   std_logic_vector (13 downto 0);
         s_read   : out   std_logic;
         s_write  : out   std_logic );
end hadamard;

architecture schematic of hadamard is

   signal ramout   : std_logic_vector(7 downto 0);
   signal ope      : std_logic;
   signal hph      : std_logic;
   signal vaccu    : std_logic;
   signal clraccu  : std_logic;
   signal vbreg    : std_logic;
   signal vhadout  : std_logic;
   signal c1       : std_logic_vector(2 downto 0);
   signal c2       : std_logic_vector(2 downto 0);
   signal c1c2     : std_logic_vector(5 downto 0);
   signal lo       : std_logic_vector(2 downto 0);
   signal loi      : std_logic;
   signal c2i      : std_logic;
   signal c1i      : std_logic;
   signal readout  : std_logic;
   signal nreadout : std_logic;
   signal nck      : std_logic;

   component calcul
      port( ramout  : in  std_logic_vector ( 7 downto 0);
            hadout  : out std_logic_vector (13 downto 0);
            ope     : in  std_logic;
            hph     : in  std_logic;
            vaccu   : in  std_logic;
            clraccu : in  std_logic;
            vbreg   : in  std_logic;
            vhadout : in  std_logic;
            c2i     : in  std_logic;
            reset   : in  std_logic;
            ck      : in  std_logic );
   end component;  

   component sequenceur
      port( c1        : in  std_logic_vector (2 downto 0);
            c2        : in  std_logic_vector (2 downto 0);
            lo        : in  std_logic_vector (2 downto 0);
            ck        : in  std_logic;
            empty     : in  std_logic;
            full      : in  std_logic;
            reset     : in  std_logic;
            c1i       : out std_logic;
            c2i       : out std_logic;
            loi       : out std_logic;
            clraccu   : out std_logic;
            hph       : out std_logic;
            s_read    : out std_logic;
            vaccu     : out std_logic;
            vbreg     : out std_logic;
            vhadout   : out std_logic;
            s_write   : out std_logic );
   end component;

   component compteur
      port( c1i     : in  std_logic;
            c2i     : in  std_logic;
            loi     : in  std_logic;
            c1      : out std_logic_vector (2 downto 0);
            c2      : out std_logic_vector (2 downto 0);
            lo      : out std_logic_vector (2 downto 0) ; 
            reset   : in  std_logic;
            ck      : in  std_logic );
   end component;

   component rom
      port( c1      : in  std_logic_vector (2 downto 0);
            c2      : in  std_logic_vector (2 downto 0);
            hph     : in  std_logic;
            lo      : in  std_logic_vector (2 downto 0);
            ope     : out std_logic );
   end component;  
                   
   component ram  
     port ( A        : in std_logic_vector(5 downto 0);
            CEB, WEB : in std_logic;
            INN      : in  std_logic_vector(7 downto 0);
            OUTT     : out std_logic_vector(7 downto 0) );
   end component;
   
begin

   c1c2     <= c1&c2;
   s_read   <= readout;
   nreadout <= not(readout);
   nck      <= not(ck);

   e_ram : ram        
     port map( A    => c1c2
             , CEB  => nck
             , WEB  => nreadout
             , INN  => data
             , OUTT => ramout );

   e_calcul : calcul
     port map( ramout  => ramout
             , hadout  => hadout
             , ope     => ope
             , hph     => hph
             , vaccu   => vaccu
             , clraccu => clraccu
             , vbreg   => vbreg
             , vhadout => vhadout
             , c2i     => c2i
             , reset   => reset
             , ck      => ck );

   e_sequenceur : sequenceur 
     port map( c1      => c1
             , c2      => c2
             , lo      => lo
             , ck      => ck
             , empty   => empty
             , full    => full
             , reset   => reset
             , c1i     => c1i
             , c2i     => c2i
             , loi     => loi
             , clraccu => clraccu
             , hph     => hph
             , s_read  => readout
             , vaccu   => vaccu
             , vbreg   => vbreg
             , vhadout => vhadout
             , s_write => s_write );

   e_rom : rom
     port map( c1      => c1
             , c2      => c2 
             , lo      => lo 
             , hph     => hph
             , ope     => ope );
                      
   e_compteur : compteur
     port map( c1i     => c1i 
             , c2i     => c2i 
             , loi     => loi 
             , c1      => c1
             , c2      => c2
             , lo      => lo
             , reset   => reset
             , ck      => ck  );

end schematic;

