library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Multi8 is


    port ( X  : in Std_Logic_Vector(7 downto 0) ;
           Y  : in Std_Logic_Vector(7 downto 0) ;
           R  : out Std_Logic_Vector(15 downto 0) );

end Multi8;

----------------------------------------------------------------------

architecture beh OF Multi8 is

  signal PP1 : Std_Logic_Vector(8 downto 0);
  signal PP2 : Std_Logic_Vector(8 downto 0);
  signal PP3 : Std_Logic_Vector(8 downto 0);
  signal PP4 : Std_Logic_Vector(8 downto 0);
  signal PP5 : Std_Logic_Vector(8 downto 0);
  signal PP6 : Std_Logic_Vector(8 downto 0);
  signal PP7 : Std_Logic_Vector(8 downto 0);
  signal PP8 : Std_Logic_Vector(8 downto 0);

  signal PP12 : Std_Logic_Vector(9 downto 0);
  signal PP34 : Std_Logic_Vector(9 downto 0);
  signal PP56 : Std_Logic_Vector(9 downto 0);
  signal PP78 : Std_Logic_Vector(9 downto 0);

  signal R1, R2   : Std_Logic_Vector(11 downto 0);

begin

   PP1(0) <= Y(0) and X(0);
   PP1(1) <= Y(0) and X(1);
   PP1(2) <= Y(0) and X(2);
   PP1(3) <= Y(0) and X(3);
   PP1(4) <= Y(0) and X(4);
   PP1(5) <= Y(0) and X(5);
   PP1(6) <= Y(0) and X(6);
   PP1(7) <= Y(0) and X(7);
   PP1(8) <= '0';

   PP2(0) <= '0';
   PP2(1) <= Y(1) and X(0);
   PP2(2) <= Y(1) and X(1);
   PP2(3) <= Y(1) and X(2);
   PP2(4) <= Y(1) and X(3);
   PP2(5) <= Y(1) and X(4);
   PP2(6) <= Y(1) and X(5);
   PP2(7) <= Y(1) and X(6);
   PP2(8) <= Y(1) and X(7);

   PP3(0) <= Y(2) and X(0);
   PP3(1) <= Y(2) and X(1);
   PP3(2) <= Y(2) and X(2);
   PP3(3) <= Y(2) and X(3);
   PP3(4) <= Y(2) and X(4);
   PP3(5) <= Y(2) and X(5);
   PP3(6) <= Y(2) and X(6);
   PP3(7) <= Y(2) and X(7);
   PP3(8) <= '0';

   PP4(0) <= '0';
   PP4(1) <= Y(3) and X(0);
   PP4(2) <= Y(3) and X(1);
   PP4(3) <= Y(3) and X(2);
   PP4(4) <= Y(3) and X(3);
   PP4(5) <= Y(3) and X(4);
   PP4(6) <= Y(3) and X(5);
   PP4(7) <= Y(3) and X(6);
   PP4(8) <= Y(3) and X(7);

   PP5(0) <= Y(4) and X(0);
   PP5(1) <= Y(4) and X(1);
   PP5(2) <= Y(4) and X(2);
   PP5(3) <= Y(4) and X(3);
   PP5(4) <= Y(4) and X(4);
   PP5(5) <= Y(4) and X(5);
   PP5(6) <= Y(4) and X(6);
   PP5(7) <= Y(4) and X(7);
   PP5(8) <= '0';

   PP6(0) <= '0';
   PP6(1) <= Y(5) and X(0);
   PP6(2) <= Y(5) and X(1);
   PP6(3) <= Y(5) and X(2);
   PP6(4) <= Y(5) and X(3);
   PP6(5) <= Y(5) and X(4);
   PP6(6) <= Y(5) and X(5);
   PP6(7) <= Y(5) and X(6);
   PP6(8) <= Y(5) and X(7);

   PP7(0) <= Y(6) and X(0);
   PP7(1) <= Y(6) and X(1);
   PP7(2) <= Y(6) and X(2);
   PP7(3) <= Y(6) and X(3);
   PP7(4) <= Y(6) and X(4);
   PP7(5) <= Y(6) and X(5);
   PP7(6) <= Y(6) and X(6);
   PP7(7) <= Y(6) and X(7);
   PP7(8) <= '0';

   PP8(0) <= '0';
   PP8(1) <= Y(7) and X(0);
   PP8(2) <= Y(7) and X(1);
   PP8(3) <= Y(7) and X(2);
   PP8(4) <= Y(7) and X(3);
   PP8(5) <= Y(7) and X(4);
   PP8(6) <= Y(7) and X(5);
   PP8(7) <= Y(7) and X(6);
   PP8(8) <= Y(7) and X(7);

   PP12 <= PP1 + PP2;
   PP34 <= PP3 + PP4;
   PP56 <= PP5 + PP6;
   PP78 <= PP7 + PP8;

   R1 <= ("00" & PP12) + (PP34 & "00");
   R2 <= ("00" & PP56) + (PP78 & "00");
   R <= ("0000" & R1)   + (R2 & "0000");

end beh;
