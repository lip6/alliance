library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;

entity hadamard_tb is
end hadamard_tb;

architecture test_bench of hadamard_tb is

component hadamard
  port ( ck     : in    std_logic;
         data   : in    std_logic_vector (7 downto 0);
         empty  : in    std_logic;
         full   : in    std_logic;
         reset  : in    std_logic;
         hadout : out   std_logic_vector (13 downto 0);
         s_read   : out   std_logic;
         s_write  : out   std_logic );
end component;

  signal i_ck     : std_logic;
  signal i_data   : std_logic_vector (7 downto 0);
  signal i_empty  : std_logic;
  signal i_full   : std_logic;
  signal i_reset  : std_logic;
  signal i_hadout : std_logic_vector (13 downto 0);
  signal i_read   : std_logic;
  signal i_write  : std_logic;

begin

  InsHadamard : hadamard 

   port map( i_ck, i_data, i_empty, i_full, i_reset, 
             i_hadout, i_read, i_write );


  Stimulus:
    process
--use IEEE.STD_LOGIC_TEXTIO.ALL;
  use STD.TEXTIO.ALL;
      FILE infile: TEXT is IN "hadamard.stim";
        FILE outfile: TEXT is OUT "hadamard_res.stim";

        VARIABLE L1, L2: LINE;

  variable v_char   : character;
  variable v_ck     : std_logic;
  variable v_data   : std_logic_vector (7 downto 0);
  variable v_empty  : std_logic;
  variable v_full   : std_logic;
  variable v_reset  : std_logic;
  variable v_hadout : std_logic_vector (13 downto 0);
  variable v_read   : std_logic;
  variable v_write  : std_logic;

    begin
      While not (endfile(infile)) loop
        readline (infile,L1);

        read( L1, v_char);

        if ( v_char = '#' )
        then writeline(outfile,L1);
             next;
        end if;
        
        read( L1, v_ck);
        read( L1, v_data);
        read( L1, v_empty);
        read( L1, v_full);
        read( L1, v_reset);

        i_ck <= v_ck;
        i_data <= v_data;
        i_empty <= v_empty;
        i_full <= v_full;
        i_reset <= v_reset;

         wait for 100 ns;

        write( L2, i_ck);
        write( L2, i_data);
        write( L2, i_empty);
        write( L2, i_full);
        write( L2, i_reset);
        write( L2, i_hadout);
        write( L2, i_read);
        write( L2, i_write);

        writeline (outfile,L2);
           
      end loop;

      ASSERT FALSE REPORT "End of simulation !!!"
      SEVERITY ERROR;

      wait;  -- to terminate simulation
    end process;

end test_bench;

configuration CFG_test_bench of hadamard_tb is
use WORK.all;

  for test_bench
     FOR InsHadamard : Hadamard
       use entity WORK.Hadamard(schematic);
     end for;
  end for;

end CFG_test_bench;
