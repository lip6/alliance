library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Multi8 is


    port ( CK : in std_logic;
           X  : in Std_Logic_Vector(15 downto 0) ;
           Y  : in Std_Logic_Vector(15 downto 0) ;
           R  : out Std_Logic_Vector(31 downto 0) );

end Multi8;

----------------------------------------------------------------------

architecture beh OF Multi8 is

  signal PP1  : Std_Logic_Vector(16 downto 0);
  signal PP2  : Std_Logic_Vector(16 downto 0);
  signal PP3  : Std_Logic_Vector(16 downto 0);
  signal PP4  : Std_Logic_Vector(16 downto 0);
  signal PP5  : Std_Logic_Vector(16 downto 0);
  signal PP6  : Std_Logic_Vector(16 downto 0);
  signal PP7  : Std_Logic_Vector(16 downto 0);
  signal PP8  : Std_Logic_Vector(16 downto 0);
  signal PP9  : Std_Logic_Vector(16 downto 0);
  signal PP10 : Std_Logic_Vector(16 downto 0);
  signal PP11 : Std_Logic_Vector(16 downto 0);
  signal PP12 : Std_Logic_Vector(16 downto 0);
  signal PP13 : Std_Logic_Vector(16 downto 0);
  signal PP14 : Std_Logic_Vector(16 downto 0);
  signal PP15 : Std_Logic_Vector(16 downto 0);
  signal PP16 : Std_Logic_Vector(16 downto 0);

  signal PP1_2   : Std_Logic_Vector(17 downto 0);
  signal PP3_4   : Std_Logic_Vector(17 downto 0);
  signal PP5_6   : Std_Logic_Vector(17 downto 0);
  signal PP7_8   : Std_Logic_Vector(17 downto 0);
  signal PP9_10  : Std_Logic_Vector(17 downto 0);
  signal PP11_12 : Std_Logic_Vector(17 downto 0);
  signal PP13_14 : Std_Logic_Vector(17 downto 0);
  signal PP15_16 : Std_Logic_Vector(17 downto 0);

  signal R1, R2, R3, R4   : Std_Logic_Vector(19 downto 0);

  signal R12, R34         : Std_Logic_Vector(23 downto 0);

  signal R_int, R_out : Std_Logic_Vector(31 downto 0);

begin

   PP1(0)  <= Y(0) and X(0);
   PP1(1)  <= Y(0) and X(1);
   PP1(2)  <= Y(0) and X(2);
   PP1(3)  <= Y(0) and X(3);
   PP1(4)  <= Y(0) and X(4);
   PP1(5)  <= Y(0) and X(5);
   PP1(6)  <= Y(0) and X(6);
   PP1(7)  <= Y(0) and X(7);
   PP1(8)  <= Y(0) and X(8);
   PP1(9)  <= Y(0) and X(9);
   PP1(10) <= Y(0) and X(10);
   PP1(11) <= Y(0) and X(11);
   PP1(12) <= Y(0) and X(12);
   PP1(13) <= Y(0) and X(13);
   PP1(14) <= Y(0) and X(14);
   PP1(15) <= Y(0) and X(15);
   PP1(16) <= '0';

   PP2(0)  <= '0';
   PP2(1)  <= Y(1) and X(0);
   PP2(2)  <= Y(1) and X(1);
   PP2(3)  <= Y(1) and X(2);
   PP2(4)  <= Y(1) and X(3);
   PP2(5)  <= Y(1) and X(4);
   PP2(6)  <= Y(1) and X(5);
   PP2(7)  <= Y(1) and X(6);
   PP2(8)  <= Y(1) and X(7);
   PP2(9)  <= Y(1) and X(8);
   PP2(10) <= Y(1) and X(9);
   PP2(11) <= Y(1) and X(10);
   PP2(12) <= Y(1) and X(11);
   PP2(13) <= Y(1) and X(12);
   PP2(14) <= Y(1) and X(13);
   PP2(15) <= Y(1) and X(14);
   PP2(16) <= Y(1) and X(15);

   PP3(0)  <= Y(2) and X(0);
   PP3(1)  <= Y(2) and X(1);
   PP3(2)  <= Y(2) and X(2);
   PP3(3)  <= Y(2) and X(3);
   PP3(4)  <= Y(2) and X(4);
   PP3(5)  <= Y(2) and X(5);
   PP3(6)  <= Y(2) and X(6);
   PP3(7)  <= Y(2) and X(7);
   PP3(8)  <= Y(2) and X(8);
   PP3(9)  <= Y(2) and X(9);
   PP3(10) <= Y(2) and X(10);
   PP3(11) <= Y(2) and X(11);
   PP3(12) <= Y(2) and X(12);
   PP3(13) <= Y(2) and X(13);
   PP3(14) <= Y(2) and X(14);
   PP3(15) <= Y(2) and X(15);
   PP3(16) <= '0';

   PP4(0)  <= '0';
   PP4(1)  <= Y(3) and X(0);
   PP4(2)  <= Y(3) and X(1);
   PP4(3)  <= Y(3) and X(2);
   PP4(4)  <= Y(3) and X(3);
   PP4(5)  <= Y(3) and X(4);
   PP4(6)  <= Y(3) and X(5);
   PP4(7)  <= Y(3) and X(6);
   PP4(8)  <= Y(3) and X(7);
   PP4(9)  <= Y(3) and X(8);
   PP4(10) <= Y(3) and X(9);
   PP4(11) <= Y(3) and X(10);
   PP4(12) <= Y(3) and X(11);
   PP4(13) <= Y(3) and X(12);
   PP4(14) <= Y(3) and X(13);
   PP4(15) <= Y(3) and X(14);
   PP4(16) <= Y(3) and X(15);

   PP5(0)  <= Y(4) and X(0);
   PP5(1)  <= Y(4) and X(1);
   PP5(2)  <= Y(4) and X(2);
   PP5(3)  <= Y(4) and X(3);
   PP5(4)  <= Y(4) and X(4);
   PP5(5)  <= Y(4) and X(5);
   PP5(6)  <= Y(4) and X(6);
   PP5(7)  <= Y(4) and X(7);
   PP5(8)  <= Y(4) and X(8);
   PP5(9)  <= Y(4) and X(9);
   PP5(10) <= Y(4) and X(10);
   PP5(11) <= Y(4) and X(11);
   PP5(12) <= Y(4) and X(12);
   PP5(13) <= Y(4) and X(13);
   PP5(14) <= Y(4) and X(14);
   PP5(15) <= Y(4) and X(15);
   PP5(16) <= '0';

   PP6(0)  <= '0';
   PP6(1)  <= Y(5) and X(0);
   PP6(2)  <= Y(5) and X(1);
   PP6(3)  <= Y(5) and X(2);
   PP6(4)  <= Y(5) and X(3);
   PP6(5)  <= Y(5) and X(4);
   PP6(6)  <= Y(5) and X(5);
   PP6(7)  <= Y(5) and X(6);
   PP6(8)  <= Y(5) and X(7);
   PP6(9)  <= Y(5) and X(8);
   PP6(10) <= Y(5) and X(9);
   PP6(11) <= Y(5) and X(10);
   PP6(12) <= Y(5) and X(11);
   PP6(13) <= Y(5) and X(12);
   PP6(14) <= Y(5) and X(13);
   PP6(15) <= Y(5) and X(14);
   PP6(16) <= Y(5) and X(15);

   PP7(0)  <= Y(6) and X(0);
   PP7(1)  <= Y(6) and X(1);
   PP7(2)  <= Y(6) and X(2);
   PP7(3)  <= Y(6) and X(3);
   PP7(4)  <= Y(6) and X(4);
   PP7(5)  <= Y(6) and X(5);
   PP7(6)  <= Y(6) and X(6);
   PP7(7)  <= Y(6) and X(7);
   PP7(8)  <= Y(6) and X(8);
   PP7(9)  <= Y(6) and X(9);
   PP7(10) <= Y(6) and X(10);
   PP7(11) <= Y(6) and X(11);
   PP7(12) <= Y(6) and X(12);
   PP7(13) <= Y(6) and X(13);
   PP7(14) <= Y(6) and X(14);
   PP7(15) <= Y(6) and X(15);
   PP7(16) <= '0';

   PP8(0)  <= '0';
   PP8(1)  <= Y(7) and X(0);
   PP8(2)  <= Y(7) and X(1);
   PP8(3)  <= Y(7) and X(2);
   PP8(4)  <= Y(7) and X(3);
   PP8(5)  <= Y(7) and X(4);
   PP8(6)  <= Y(7) and X(5);
   PP8(7)  <= Y(7) and X(6);
   PP8(8)  <= Y(7) and X(7);
   PP8(9)  <= Y(7) and X(8);
   PP8(10) <= Y(7) and X(9);
   PP8(11) <= Y(7) and X(10);
   PP8(12) <= Y(7) and X(11);
   PP8(13) <= Y(7) and X(12);
   PP8(14) <= Y(7) and X(13);
   PP8(15) <= Y(7) and X(14);
   PP8(16) <= Y(7) and X(15);

   PP9(0)  <= Y(8) and X(0);
   PP9(1)  <= Y(8) and X(1);
   PP9(2)  <= Y(8) and X(2);
   PP9(3)  <= Y(8) and X(3);
   PP9(4)  <= Y(8) and X(4);
   PP9(5)  <= Y(8) and X(5);
   PP9(6)  <= Y(8) and X(6);
   PP9(7)  <= Y(8) and X(7);
   PP9(8)  <= Y(8) and X(8);
   PP9(9)  <= Y(8) and X(9);
   PP9(10) <= Y(8) and X(10);
   PP9(11) <= Y(8) and X(11);
   PP9(12) <= Y(8) and X(12);
   PP9(13) <= Y(8) and X(13);
   PP9(14) <= Y(8) and X(14);
   PP9(15) <= Y(8) and X(15);
   PP9(16) <= '0';

   PP10(0)  <= '0';
   PP10(1)  <= Y(9) and X(0);
   PP10(2)  <= Y(9) and X(1);
   PP10(3)  <= Y(9) and X(2);
   PP10(4)  <= Y(9) and X(3);
   PP10(5)  <= Y(9) and X(4);
   PP10(6)  <= Y(9) and X(5);
   PP10(7)  <= Y(9) and X(6);
   PP10(8)  <= Y(9) and X(7);
   PP10(9)  <= Y(9) and X(8);
   PP10(10) <= Y(9) and X(9);
   PP10(11) <= Y(9) and X(10);
   PP10(12) <= Y(9) and X(11);
   PP10(13) <= Y(9) and X(12);
   PP10(14) <= Y(9) and X(13);
   PP10(15) <= Y(9) and X(14);
   PP10(16) <= Y(9) and X(15);

   PP11(0)  <= Y(10) and X(0);
   PP11(1)  <= Y(10) and X(1);
   PP11(2)  <= Y(10) and X(2);
   PP11(3)  <= Y(10) and X(3);
   PP11(4)  <= Y(10) and X(4);
   PP11(5)  <= Y(10) and X(5);
   PP11(6)  <= Y(10) and X(6);
   PP11(7)  <= Y(10) and X(7);
   PP11(8)  <= Y(10) and X(8);
   PP11(9)  <= Y(10) and X(9);
   PP11(10) <= Y(10) and X(10);
   PP11(11) <= Y(10) and X(11);
   PP11(12) <= Y(10) and X(12);
   PP11(13) <= Y(10) and X(13);
   PP11(14) <= Y(10) and X(14);
   PP11(15) <= Y(10) and X(15);
   PP11(16) <= '0';

   PP12(0)  <= '0';
   PP12(1)  <= Y(11) and X(0);
   PP12(2)  <= Y(11) and X(1);
   PP12(3)  <= Y(11) and X(2);
   PP12(4)  <= Y(11) and X(3);
   PP12(5)  <= Y(11) and X(4);
   PP12(6)  <= Y(11) and X(5);
   PP12(7)  <= Y(11) and X(6);
   PP12(8)  <= Y(11) and X(7);
   PP12(9)  <= Y(11) and X(8);
   PP12(10) <= Y(11) and X(9);
   PP12(11) <= Y(11) and X(10);
   PP12(12) <= Y(11) and X(11);
   PP12(13) <= Y(11) and X(12);
   PP12(14) <= Y(11) and X(13);
   PP12(15) <= Y(11) and X(14);
   PP12(16) <= Y(11) and X(15);

   PP13(0)  <= Y(12) and X(0);
   PP13(1)  <= Y(12) and X(1);
   PP13(2)  <= Y(12) and X(2);
   PP13(3)  <= Y(12) and X(3);
   PP13(4)  <= Y(12) and X(4);
   PP13(5)  <= Y(12) and X(5);
   PP13(6)  <= Y(12) and X(6);
   PP13(7)  <= Y(12) and X(7);
   PP13(8)  <= Y(12) and X(8);
   PP13(9)  <= Y(12) and X(9);
   PP13(10) <= Y(12) and X(10);
   PP13(11) <= Y(12) and X(11);
   PP13(12) <= Y(12) and X(12);
   PP13(13) <= Y(12) and X(13);
   PP13(14) <= Y(12) and X(14);
   PP13(15) <= Y(12) and X(15);
   PP13(16) <= '0';

   PP14(0)  <= '0';
   PP14(1)  <= Y(13) and X(0);
   PP14(2)  <= Y(13) and X(1);
   PP14(3)  <= Y(13) and X(2);
   PP14(4)  <= Y(13) and X(3);
   PP14(5)  <= Y(13) and X(4);
   PP14(6)  <= Y(13) and X(5);
   PP14(7)  <= Y(13) and X(6);
   PP14(8)  <= Y(13) and X(7);
   PP14(9)  <= Y(13) and X(8);
   PP14(10) <= Y(13) and X(9);
   PP14(11) <= Y(13) and X(10);
   PP14(12) <= Y(13) and X(11);
   PP14(13) <= Y(13) and X(12);
   PP14(14) <= Y(13) and X(13);
   PP14(15) <= Y(13) and X(14);
   PP14(16) <= Y(13) and X(15);

   PP15(0)  <= Y(14) and X(0);
   PP15(1)  <= Y(14) and X(1);
   PP15(2)  <= Y(14) and X(2);
   PP15(3)  <= Y(14) and X(3);
   PP15(4)  <= Y(14) and X(4);
   PP15(5)  <= Y(14) and X(5);
   PP15(6)  <= Y(14) and X(6);
   PP15(7)  <= Y(14) and X(7);
   PP15(8)  <= Y(14) and X(8);
   PP15(9)  <= Y(14) and X(9);
   PP15(10) <= Y(14) and X(10);
   PP15(11) <= Y(14) and X(11);
   PP15(12) <= Y(14) and X(12);
   PP15(13) <= Y(14) and X(13);
   PP15(14) <= Y(14) and X(14);
   PP15(15) <= Y(14) and X(15);
   PP15(16) <= '0';

   PP16(0)  <= '0';
   PP16(1)  <= Y(15) and X(0);
   PP16(2)  <= Y(15) and X(1);
   PP16(3)  <= Y(15) and X(2);
   PP16(4)  <= Y(15) and X(3);
   PP16(5)  <= Y(15) and X(4);
   PP16(6)  <= Y(15) and X(5);
   PP16(7)  <= Y(15) and X(6);
   PP16(8)  <= Y(15) and X(7);
   PP16(9)  <= Y(15) and X(8);
   PP16(10) <= Y(15) and X(9);
   PP16(11) <= Y(15) and X(10);
   PP16(12) <= Y(15) and X(11);
   PP16(13) <= Y(15) and X(12);
   PP16(14) <= Y(15) and X(13);
   PP16(15) <= Y(15) and X(14);
   PP16(16) <= Y(15) and X(15);

   PP1_2   <= PP1  + PP2;
   PP3_4   <= PP3  + PP4;
   PP5_6   <= PP5  + PP6;
   PP7_8   <= PP7  + PP8;
   PP9_10  <= PP9  + PP10;
   PP11_12 <= PP11 + PP12;
   PP13_14 <= PP13 + PP14;
   PP15_16 <= PP15 + PP16;

   R1 <= ("00" & PP1_2) + (PP3_4 & "00");
   R2 <= ("00" & PP5_6) + (PP7_8 & "00");

   R3 <= ("00" & PP9_10 ) + (PP11_12 & "00");
   R4 <= ("00" & PP13_14) + (PP15_16 & "00");

   R12 <= ("0000" & R1)  + (R2 & "0000");
   R34 <= ("0000" & R3)  + (R4 & "0000");

   R_int <= ("00000000" & R12) + (R34 & "00000000");

   process( ck )
   begin
     if ( ck='1' and ck'event)
     then R_out <= not(R_int);
     end if;
   end process;

   R <= not(R_out);

end beh;
