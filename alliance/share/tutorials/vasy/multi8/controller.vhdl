library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity Controller is
  port (STB, CLK, LSB, Done, RST:	in	STD_LOGIC;
	Init, Shift, Add:	out	STD_LOGIC);
end Controller;

---------------------------------------------------

architecture FSM of Controller is

  type States is (EndS,InitS, CheckS, AddS, ShiftS);
  signal State: States ;
begin

  -- Drive control outputs based upon State

  Init  <= '1' when State = InitS  else '0';
  Add   <= '1' when State = AddS   else '0';
  Shift <= '1' when State = ShiftS else '0';

  -- Determine Next State from control inputs

  StateMachine:

    process (CLK)
    begin
	--compass stateMachine adj State
      if CLK'Event and CLK = '0' then
	if RST = '1' then State <= EndS;
	else
	case State is
	  when InitS =>
	    State <= CheckS;
	  when CheckS =>
	    if LSB = '1' then
	      State <= AddS;
	    elsif Done = '0' then
	      State <= ShiftS;
	    else
	      State <= EndS;
	    end if;
	  when AddS =>
	    State <= ShiftS;
	  when ShiftS =>
	    State <= CheckS;
	  when EndS =>
	    if STB = '1' then
	      State <= InitS;
	    end if;
	end case;
	end if;
      end if;
    end process;

end FSM;

---------------------------------------------------
