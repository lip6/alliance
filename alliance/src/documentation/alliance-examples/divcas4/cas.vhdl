library IEEE;
use IEEE.std_logic_1164.all;

entity cas is  -- Controlled Add/Subtract cell
  port (
    divisor       : in  std_logic;
    T             : in  std_logic;
    remainder_in  : in  std_logic;
    cin           : in  std_logic;
    remainder_out : out std_logic;
    cout          : out std_logic);
end cas;

architecture circuits of cas is
  signal tt : std_logic;
begin
  tt            <= T xor divisor;
  remainder_out <= tt xor remainder_in xor cin;
  cout          <= (tt and remainder_in) or (tt and cin) or
                   (remainder_in and cin);
end circuits;
