library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity calcul is 
port (
   ramout  : in  std_logic_vector( 7 downto 0);
   hadout  : out std_logic_vector(13 downto 0); 

   ope     : in  std_logic;
   hph     : in  std_logic;
   vaccu   : in  std_logic;
   clraccu : in  std_logic;
   vbreg   : in  std_logic;
   vhadout : in  std_logic;
   c2i     : in  std_logic;

   reset   : in  std_logic;
   ck      : in  std_logic);

end calcul;



architecture behavioral of calcul is

   type pile is array (0 to 7) of std_logic_vector (13 downto 0);
    

  signal op      : std_logic_vector(13 downto 0);
  signal mux     : std_logic_vector(13 downto 0);
  signal alu     : std_logic_vector(13 downto 0);
  signal re      : std_logic_vector(13 downto 0);
  signal rhadout : std_logic_vector(13 downto 0);

  signal accu    : std_logic_vector(13 downto 0);
  signal breg    : pile ;
 
begin

-- le multiplexeur d'abord --
-- *********************** --

   mux <= breg (7)  when hph='0' else "000000"&ramout; 
  
-- calcul alu --
-- ********** --

  op(13 downto 0) <= "11111111111111" when ope='1' else
                     "00000000000000";
  
  re(0) <= (accu(0) xor ope) and mux(0);
  re(13 downto 1) <= (mux(12 downto 0) and re(12 downto 0) ) 
                  or (    ( accu(12 downto 0) xor op(12 downto 0)) 
                      and ( mux(12 downto 0)   or re(12 downto 0)) ); 
   
  alu <= accu xor mux xor (re(13 downto 1) & '0');


-- accumulateur --
-- ************ --

  process (ck)
  begin
    if (ck='1' and ck'event ) then
      if (clraccu='1') then accu <= (others =>'0');
      elsif (vaccu='1') 
         then accu <= alu;
        else accu <= accu;
      end if;
    end if;
  end process;

-- registre de sortie --
-- ****************** --

  process (ck)
  begin
    if (ck='1' and ck'event ) then
      if (reset='1') then rhadout <= (others =>'0');
      elsif (vhadout='1') 
         then rhadout <= accu;
        else rhadout <= rhadout;
      end if;
    end if;
  end process;
  hadout <= rhadout;

-- banc de regsitres a decalage --
-- **************************** --

  process (ck)
  begin
    if (ck='1' and ck'event)
    then 
      if (c2i='1')
      then
        if (vbreg='1')
        then breg(0) <= alu;
	else breg(0) <= breg(7);
	end if;
        breg (1 to 7) <= breg (0 to 6);
      end if;
    end if;
  end process;

end behavioral;
