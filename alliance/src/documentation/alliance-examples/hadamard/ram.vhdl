---------------------------------------------------------
----------------    Bank  description    ----------------
---------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;
--use IEEE.STD_LOGIC_signed.ALL;

ENTITY ram IS
  port ( A        : in std_logic_vector(5 downto 0);
         CEB, WEB : in std_logic;
         INN      : in  std_logic_vector(7 downto 0);
         OUTT     : out std_logic_vector(7 downto 0)
  );
END ram;

ARCHITECTURE dataflow_view OF ram IS

  SUBTYPE TYPE_WORD IS std_logic_vector(7 downto 0);
  TYPE TYPE_RAM IS ARRAY(63 DOWNTO 0) OF TYPE_WORD;
  SIGNAL memory : TYPE_RAM;

BEGIN

  OUTT <= memory( CONV_INTEGER( A ) );

  RAM_0 : PROCESS( CEB )
  BEGIN
   IF (CEB='1' AND CEB'EVENT )
   THEN IF (WEB='0')
        THEN memory( CONV_INTEGER( A ) ) <= INN;
        END IF;
   END IF;

  END PROCESS RAM_0;

END dataflow_view;
