library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Multi4 is


    port ( X  : in Std_Logic_Vector(3 downto 0) ;
           Y  : in Std_Logic_Vector(3 downto 0) ;
           R  : out Std_Logic_Vector(7 downto 0) );

end Multi4;

----------------------------------------------------------------------

architecture beh OF Multi4 is

  signal PP1 : Std_Logic_Vector(4 downto 0);
  signal PP2 : Std_Logic_Vector(4 downto 0);
  signal PP3 : Std_Logic_Vector(4 downto 0);
  signal PP4 : Std_Logic_Vector(4 downto 0);

  signal PP12 : Std_Logic_Vector(5 downto 0);
  signal PP34 : Std_Logic_Vector(5 downto 0);

begin

   PP1(0) <= Y(0) and X(0);
   PP1(1) <= Y(0) and X(1);
   PP1(2) <= Y(0) and X(2);
   PP1(3) <= Y(0) and X(3);
   PP1(4) <= '0';

   PP2(0) <= '0';
   PP2(1) <= Y(1) and X(0);
   PP2(2) <= Y(1) and X(1);
   PP2(3) <= Y(1) and X(2);
   PP2(4) <= Y(1) and X(3);

   PP3(0) <= Y(2) and X(0);
   PP3(1) <= Y(2) and X(1);
   PP3(2) <= Y(2) and X(2);
   PP3(3) <= Y(2) and X(3);
   PP3(4) <= '0';

   PP4(0) <= '0';
   PP4(1) <= Y(3) and X(0);
   PP4(2) <= Y(3) and X(1);
   PP4(3) <= Y(3) and X(2);
   PP4(4) <= Y(3) and X(3);

   PP12 <= PP1 + PP2;
   PP34 <= PP3 + PP4;

   R <= ("00" & PP12) + (PP34 & "00");

end beh;
