library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;

-- AMD2901'sig_s behavioral description 
-- Connector'sig_s declaration
--
ENTITY amd2901 IS
PORT (
       ck        : IN STD_LOGIC;
       i         : IN STD_LOGIC_VECTOR (8 DOWNTO 0);
       a         : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
       b         : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
       d         : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
       r0_in     : IN STD_LOGIC;
       r0_out    : OUT STD_LOGIC;
       r3_in     : IN STD_LOGIC;
       r3_out    : OUT STD_LOGIC;
       q0_in     : IN STD_LOGIC;
       q0_out    : OUT STD_LOGIC;
       q3_in     : IN STD_LOGIC;
       q3_out    : OUT STD_LOGIC;
       ovr       : OUT STD_LOGIC;
       zero      : OUT STD_LOGIC;
       signe     : OUT STD_LOGIC;
       np        : OUT STD_LOGIC;
       ng        : OUT STD_LOGIC;
       cin       : IN STD_LOGIC;
       cout      : OUT STD_LOGIC;
       y         : OUT STD_LOGIC_VECTOR (3 DOWNTO 0)
     );
END amd2901;
--
ARCHITECTURE data_flow OF amd2901 IS
--
-- Internal registers
--
--
SIGNAL accu      : STD_LOGIC_VECTOR (3 DOWNTO 0);

--
-- Internal signals
--
--
SIGNAL sig_r         : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL sig_s         : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL alu_out   : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL sumrs     : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL difrs     : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL difsr     : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL c_sumrs   : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL c_difrs   : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL c_difsr   : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL waccu     : STD_LOGIC;
sIGNAL shift_n   : STD_LOGIC;
SIGNAL shift_l   : STD_LOGIC;
SIGNAL shift_r   : STD_LOGIC;
SIGNAL sh_acc    : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL sh_ram    : STD_LOGIC_VECTOR (3 DOWNTO 0);
SIGNAL wram      : STD_LOGIC;
SIGNAL accu_in   : STD_LOGIC_VECTOR ( 3 DOWNTO 0 ) ;
SIGNAL ra        : STD_LOGIC_VECTOR ( 3 DOWNTO 0 ) ;
SIGNAL rb        : STD_LOGIC_VECTOR ( 3 DOWNTO 0 ) ;

  SUBTYPE TYPE_WORD IS std_logic_vector(3 downto 0);
  TYPE TYPE_RAM IS ARRAY(15 DOWNTO 0) OF TYPE_WORD;
  SIGNAL ram : TYPE_RAM;

BEGIN

  ra <= ram( CONV_INTEGER( a ) );
  rb <= ram( CONV_INTEGER( b ) );

  PROCESS( ck )
  BEGIN
   IF (ck='1' AND ck'EVENT )
   THEN IF (wram='1')
        THEN ram( CONV_INTEGER( A ) ) <= sh_ram;
        END IF;
   END IF;
  END PROCESS;

--
-- ALU-INPUT MULTIPLEXER 
--

WITH i(2 DOWNTO 0) SELECT
     sig_s <= ra       WHEN "100",
          ra       WHEN "101",
          rb       WHEN "001",
          rb       WHEN "011",
          NOT accu WHEN "000",
          NOT accu WHEN "010",
          NOT accu WHEN "110",
          "0000"   WHEN others;
--
--
WITH i(2 DOWNTO 0) SELECT
     sig_r <= ra       WHEN "000",
          ra       WHEN "001",
          "0000"   WHEN "010",
          "0000"   WHEN "011",
          "0000"   WHEN "100",
          d        WHEN others;
--
--  OUTPUT
--
WITH i(8 DOWNTO 6) SELECT
     y <= ra      WHEN "010",
          alu_out when others;
--
-- Arithmetic and Logic Unit 
--
--  sig_r+sig_s
--

c_sumrs <= (sig_r AND sig_s) OR (sig_r(3 DOWNTO 0) AND (c_sumrs(2 DOWNTO 0) & cin)) OR 
                  (sig_s(3 DOWNTO 0) AND (c_sumrs(2 DOWNTO 0) & cin));

sumrs <=  sig_s XOR (c_sumrs(2 DOWNTO 0) & cin) XOR sig_r;

--
--  sig_r-sig_s
--
difrs(3 DOWNTO 0)   <=  sig_r(3 DOWNTO 0) XOR 
                        NOT sig_s(3 DOWNTO 0) XOR 
                       (c_difrs(2 DOWNTO 0) & cin);
c_difrs(3 DOWNTO 0) <= (sig_r(3 DOWNTO 0) AND NOT sig_s(3 DOWNTO 0)) OR 
                       (sig_r(3 DOWNTO 0) AND (c_difrs(2 DOWNTO 0) & cin)) OR
                       (NOT sig_s(3 DOWNTO 0) AND (c_difrs(2 DOWNTO 0) & cin));
--
--  sig_s-sig_r
--
difsr(3 DOWNTO 0)   <=  NOT sig_r(3 DOWNTO 0) XOR 
                        sig_s(3 DOWNTO 0) XOR 
                       (c_difsr(2 DOWNTO 0) & cin);
c_difsr(3 DOWNTO 0) <= (NOT sig_r(3 DOWNTO 0) AND sig_s(3 DOWNTO 0)) OR 
                       (NOT sig_r(3 DOWNTO 0) AND (c_difsr(2 DOWNTO 0) & cin)) OR
                       (sig_s(3 DOWNTO 0) AND (c_difsr(2 DOWNTO 0) & cin));
--
-- P &G FLAGS
--
WITH i(5 DOWNTO 3) SELECT
ng <= NOT ((sig_r(3) AND sig_s(3)) OR ((sig_r(3) OR sig_s(3)) AND (sig_r(2) AND sig_s(2))) OR ((sig_r(3) OR sig_s(3)) AND (sig_r(2) OR sig_s(2)) AND (sig_r(1) AND sig_s(1))) OR ((sig_r(3) OR sig_s(3)) AND (sig_r(2) OR sig_s(2)) AND (sig_r(1) OR sig_s(1)) AND (sig_r(0) AND sig_s(0)))) WHEN "000",
      NOT ((NOT sig_r(3) AND sig_s(3)) OR ((NOT sig_r(3) OR sig_s(3)) AND (NOT sig_r(2) AND sig_s(2))) OR ((NOT sig_r(3) OR sig_s(3)) AND (NOT sig_r(2) OR sig_s(2)) AND (NOT sig_r(1) AND sig_s(1))) OR ((NOT sig_r(3) OR sig_s(3)) AND (NOT sig_r(2) OR sig_s(2)) AND (NOT sig_r(1) OR sig_s(1)) AND (NOT sig_r(0) AND sig_s(0)))) WHEN "001",
      NOT ((sig_r(3) AND NOT sig_s(3)) OR ((sig_r(3) OR NOT sig_s(3)) AND (sig_r(2) AND NOT sig_s(2))) OR ((sig_r(3) OR NOT sig_s(3)) AND (sig_r(2) OR NOT sig_s(2)) AND (sig_r(1) AND NOT sig_s(1))) OR ((sig_r(3) OR NOT sig_s(3)) AND (sig_r(2) OR NOT sig_s(2)) AND (sig_r(1) OR NOT sig_s(1)) AND (sig_r(0) AND NOT sig_s(0))))WHEN "010",
      "1" WHEN OTHERS;
--
--
WITH i(5 DOWNTO 3) SELECT
np <= NOT ((sig_r(3) OR sig_s(3)) AND (sig_r(2) OR sig_s(2)) AND (sig_r(1) OR sig_s(1)) AND (sig_r(0) OR sig_s(0))) WHEN "000",
      NOT ((NOT sig_r(3) OR sig_s(3)) AND (NOT sig_r(2) OR sig_s(2)) AND (NOT sig_r(1) OR sig_s(1)) AND (NOT sig_r(0) OR sig_s(0))) WHEN "001",
      NOT ((sig_r(3) OR NOT sig_s(3)) AND (sig_r(2) OR NOT sig_s(2)) AND (sig_r(1) OR NOT sig_s(1)) AND (sig_r(0) OR NOT sig_s(0))) WHEN "010",
      "1" WHEN OTHERS;
--
--
signe <= alu_out(3);
zero <= NOT (alu_out(3) OR alu_out(2) OR alu_out(1) OR alu_out(0));
--
--
WITH i(5 DOWNTO 3) SELECT
ovr <= c_sumrs(3) XOR c_sumrs(2) WHEN "000",
       c_difsr(3) XOR c_difsr(2) WHEN "001",
       c_difrs(3) XOR c_difrs(2) WHEN "010",
       '0'                       WHEN OTHERS;

WITH i(5 DOWNTO 3) SELECT
alu_out <= sumrs         WHEN "000",
           difsr         WHEN "001",
           difrs         WHEN "010",
           sig_r OR sig_s        WHEN "011",
           sig_r AND sig_s       WHEN "100",
           NOT(sig_r) AND sig_s  WHEN "101",
           sig_r XOR sig_s       WHEN "110",
           NOT(sig_r XOR sig_s)  WHEN others;
--
WITH i(5 DOWNTO 3) SELECT
cout <= c_sumrs(3) WHEN "000",
        c_difsr(3) WHEN "001",
        c_difrs(3) WHEN "010",
        "0"        WHEN OTHERS;
--
--
--
--
-- CONTROL SIGNALS
--
--

wram <= (i(8) OR i(7));
waccu <= NOT (i(6)) AND ((NOT i(7)) OR i(8));
shift_r <= i(8) AND (NOT i(7));  
shift_l <= i(8) AND i(7);  
shift_n <= NOT i(8);  

--
--
-- SHIFTER ACCU
--
WITH i(8 DOWNTO 6) SELECT
     sh_acc(3) <= alu_out(3)        WHEN "000",
                  NOT accu(2)       WHEN "110",
                  NOT accu(2)       WHEN "111",
                  q3_in             WHEN "100",
                  q3_in             WHEN "101",
                  "0"               WHEN OTHERS;
--
WITH i(8 DOWNTO 6) SELECT
     sh_acc(2) <= alu_out(2)        WHEN "000",
                  NOT accu(1)       WHEN "110",
                  NOT accu(1)       WHEN "111",
                  NOT accu(3)       WHEN "100",
                  NOT accu(3)       WHEN "101",
                  "0"               WHEN OTHERS;
--
WITH i(8 DOWNTO 6) SELECT
     sh_acc(1) <= alu_out(1)        WHEN "000",
                  NOT accu(0)       WHEN "110",
                  NOT accu(0)       WHEN "111",
                  NOT accu(2)       WHEN "100",
                  NOT accu(2)       WHEN "101",
                  "0"               WHEN OTHERS;
--
WITH i(8 DOWNTO 6) SELECT
     sh_acc(0) <= alu_out(0)        WHEN "000",
                  q0_in             WHEN "110",
                  q0_in             WHEN "111",
                  NOT accu(1)       WHEN "100",
                  NOT accu(1)       WHEN "101",
                  "0"               WHEN OTHERS;
--
     q3_out <=  NOT accu(3);
     q0_out <=  NOT accu(0);
--
-- WRITING ACCU
--
accu_in(3 DOWNTO 0) <= sh_acc(3 DOWNTO 0);

process ( ck )
begin
  if ( ck='1' and ck'event )
  then if ( waccu='1') then accu <= accu_in;
       end if;
  end if;
end process;

--
-- SHIFTER - RAM 
--
WITH shift_n & shift_l & shift_r SELECT
       sh_ram(3) <= alu_out(3) WHEN "100",
                    alu_out(2) WHEN "010",
                    r3_in      WHEN "001",
                    "0"        WHEN OTHERS;
--
WITH shift_n & shift_l & shift_r SELECT
       sh_ram(2) <= alu_out(2) WHEN "100",
                    alu_out(1) WHEN "010",
                    alu_out(3) WHEN "001",
                    "0"        WHEN OTHERS;
--
WITH shift_n & shift_l & shift_r SELECT
       sh_ram(1) <= alu_out(1) WHEN "100",
                    alu_out(0) WHEN "010",
                    alu_out(2) WHEN "001",
                    "0"        WHEN OTHERS;
--
WITH shift_n & shift_l & shift_r SELECT
       sh_ram(0) <= alu_out(0) WHEN "100",
                    r0_in      WHEN "010",
                    alu_out(1) WHEN "001",
                    "0"        WHEN OTHERS;

r3_out <= alu_out(3);
r0_out <= alu_out(0);

END data_flow;  
