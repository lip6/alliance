library IEEE;
use IEEE.std_logic_1164.all;

entity divcas4 is   -- 8 bit dividend, 4 bit divisor
  port (
    dividend  : in  std_logic_vector(6 downto 0);
    divisor   : in  std_logic_vector(3 downto 0);
    quotient  : out std_logic_vector(3 downto 0);
    remainder : out std_logic_vector(3 downto 0));
end divcas4;

use IEEE.std_logic_textio.all;
use STD.textio.all;

architecture circuits of divcas4 is

  component cas 
  port (
    divisor       : in  std_logic;
    T             : in  std_logic;
    remainder_in  : in  std_logic;
    cin           : in  std_logic;
    remainder_out : out std_logic;
    cout          : out std_logic);
  end component;

  signal T : std_logic_vector(3 downto 0);
  signal c36, c35, c34, c33, c25, c24, c23, c22 : std_logic;
  signal c14, c13, c12, c11, c03, c02, c01, c00 : std_logic;
  signal r36, r35, r34, r33, r25, r24, r23, r22 : std_logic;
  signal r14, r13, r12, r11, r03, r02, r01, r00 : std_logic;
begin
  T(3) <= '1';
  cas36: cas port map(
         divisor(3), T(3), dividend(6), c35,  r36, c36); 
  cas35: cas port map(
         divisor(2), T(3), dividend(5), c34,  r35, c35); 
  cas34: cas port map(
         divisor(1), T(3), dividend(4), c33,  r34, c34); 
  cas33: cas port map(
         divisor(0), T(3), dividend(3), T(3), r33, c33);
  T(2) <= not r36;
  
  cas25: cas port map(
         divisor(3), T(2), r35        , c24,  r25, c25); 
  cas24: cas port map(
         divisor(2), T(2), r34        , c23,  r24, c24); 
  cas23: cas port map(
         divisor(1), T(2), r33        , c22,  r23, c23); 
  cas22: cas port map(
         divisor(0), T(2), dividend(2), T(2), r22, c22);
  T(1) <= not r25;
  
  cas14: cas port map(
         divisor(3), T(1), r24        , c13,  r14, c14); 
  cas13: cas port map(
         divisor(2), T(1), r23        , c12,  r13, c13); 
  cas12: cas port map(
         divisor(1), T(1), r22        , c11,  r12, c12); 
  cas11: cas port map(
         divisor(0), T(1), dividend(1), T(1), r11, c11);
  T(0) <= not r14;
  
  cas03: cas port map( divisor(3), T(0), r13        , c02,  r03, c03); 
  cas02: cas port map( divisor(2), T(0), r12        , c01,  r02, c02); 
  cas01: cas port map( divisor(1), T(0), r11        , c00,  r01, c01); 
  cas00: cas port map( divisor(0), T(0), dividend(0), T(0), r00, c00);

  quotient(3)  <= T(2);
  quotient(2)  <= T(1);
  quotient(1)  <= T(0);
  quotient(0)  <= not r03;
  remainder(3) <= r03;
  remainder(2) <= r02;
  remainder(1) <= r01;
  remainder(0) <= r00;

end circuits;
