-- Date of netlist generation:  Dec-6-95
----------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Multi8 is


    port ( A : in Std_Logic_Vector(7 downto 0) ;
           B : in Std_Logic_Vector(7 downto 0) ;
           CLK : in Std_Logic ;
           RST : in Std_Logic ;
           STB : in Std_Logic ;

           RESULT : out Std_Logic_Vector(15 downto 0) ;
           DONE : out Std_Logic );

end Multi8;

----------------------------------------------------------------------

architecture Structural OF Multi8 is

    component Controller
        port ( STB : in Std_Logic;
               CLK : in Std_Logic;
               LSB : in Std_Logic;
               DONE : in Std_Logic;
               RST : in Std_Logic;

               INIT : out Std_Logic;
               SHIFT : out Std_Logic;
               ADD : out Std_Logic );
    end component;

    component Srb
        port ( SHIFT : in Std_Logic;
               LD : in Std_Logic;
               CLK : in Std_Logic;
               B : in Std_Logic_Vector(7 downto 0) ;

               OUTS : out Std_Logic_Vector(15 downto 0)  );
    end component;

    component AddAccu
        port ( CLR : in Std_Logic;
               LD : in Std_Logic;
               OUTS : in Std_Logic_Vector(15 downto 0) ;
               CLK : in Std_Logic;

               RESULT : out Std_Logic_Vector(15 downto 0)  );
    end component;

    component Sra
        port ( SHIFT : in Std_Logic;
               LD : in Std_Logic;
               CLK : in Std_Logic;
               RST : in Std_Logic;
               A : in Std_Logic_Vector(7 downto 0) ;

               DONE : out Std_Logic;
               LSB : out Std_Logic );
    end component;


    signal DONE_local: Std_Logic;
    signal U1_LSB : Std_Logic;
    signal U3_INIT : Std_Logic;
    signal U3_SHIFT : Std_Logic;
    signal U3_ADD : Std_Logic;
    signal OUT_INT : Std_Logic_vector(15 downto 0);

begin


    U3: Controller
        port map ( STB => STB,
                   CLK => CLK,
                   LSB => U1_LSB,
                   DONE => DONE_local,
                   RST => RST,
                   INIT => U3_INIT,
                   SHIFT => U3_SHIFT,
                   ADD => U3_ADD );

    U2: Srb
        port map ( SHIFT => U3_SHIFT,
                   LD => U3_INIT,
                   CLK => CLK,
                   B => B,
                   OUTS => OUT_INT );

    U5: AddAccu
        port map ( CLR => U3_INIT,
                   LD => U3_ADD,
                   OUTS => OUT_INT,
                   CLK => CLK,
                   RESULT => RESULT );

    U1: Sra
        port map ( SHIFT => U3_SHIFT,
                   LD => U3_INIT,
                   RST => RST,
                   CLK => CLK,
                   A => A,
                   DONE => DONE_local,
                   LSB => U1_LSB );

    DONE <= DONE_local;

end Structural;

configuration CFG_Multi8 of Multi8 is
   for Structural

   end for;
end CFG_Multi8;


----------------------------------------------------------------------
