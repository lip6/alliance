
-- ### -------------------------------------------------------------- ###
-- #									#
-- # file	: dlxm_ctl.vst.h					#
-- # date	: February 1995						#
-- # version	: v0.0							#
-- #									#
-- # origin	: this description has been developed by CAO-VLSI team	#
-- #		  at MASI laboratory, University Pierre et Marie Curie	#
-- #		  URA CNRS 818, Institut Blaise Pascal			#
-- #		  4 Place Jussieu 75252 Paris Cedex 05 - France		#
-- # E-mail      :      cao-vlsi@masi.ibp.fr                            #
-- #									#
-- # descr.	: structural model of the control instantiating		#
-- #		  the sequencer and the status				#
-- #		  2*16 output control signals for data and address bus	#
-- #									#
-- ### -------------------------------------------------------------- ###




-- Entity Declaration

Entity dlxm_ctl is
PORT(
  it : in BIT_VECTOR(3 DOWNTO 0);       	-- it
  scin : in BIT;        			-- scin
  scout : out BIT;      			-- scout
  test : in BIT;        			-- test
  reset : in BIT;       			-- external reset
  frz : in BIT; 				-- frz
  byte : out BIT_VECTOR(0 to 3);    		-- byte
  rw : out BIT; 				-- rw
  rw_ctl : out BIT_VECTOR(15 downto 0);         -- tristate data control
  frz_ctl : out BIT_VECTOR(15 downto 0);        -- tristate address control
  ck : in BIT;  				-- ck

-- Interface with data path

seq_ctladr : out BIT;
sts_zero : out BIT;
sts_sr_cpurst : inout BIT;    -- cpureset flag to datapath
sts_sr_it3 : out BIT;      -- sts_sr_it
sts_sr_it2 : out BIT;      -- sts_sr_it
sts_sr_it1 : out BIT;      -- sts_sr_it
sts_sr_it0 : out BIT;      -- sts_sr_it
sts_sr_ico : out BIT;      -- sts_sr_ico
sts_sr_iav : out BIT;      -- sts_sr_iav
sts_sr_dav : out BIT;      -- sts_sr_dav
sts_sr_ovr : out BIT;      -- sts_sr_ovr
sts_wrtpnt : out BIT_VECTOR(4 downto 0);
sts_redpnt : out BIT_VECTOR (4 downto 0);
sts_alu_test : out BIT;    -- sts_alu_test
dp_alu_sign : in BIT;     -- dp_alu_sign
dp_alu_nul : in BIT;      -- dp_alu_nul
dp_alu_c30 : in BIT;    -- dp_alu_c30_n
dp_alu_c31 : in BIT;    -- dp_alu_c31_n
dp_opy_sign : in BIT;     -- dp_opy_sign
dp_ir_sr : in BIT;        -- dp_ir_sr
dp_ir_iar : in BIT;       -- dp_ir_iar
dp_ir_tvr : in BIT;       -- dp_ir_tvr
dp_rd : in BIT_VECTOR(4 downto 0);   -- dp_rd
dp_rdrt : in BIT_VECTOR(4 downto 0); -- dp_rdrt
dp_rs : in BIT_VECTOR(4 downto 0); -- dp_rs
dp_codop : in BIT_VECTOR(5 downto 0);-- dp_codop
dp_opx_sign : in BIT;     -- dp_opx_sign
dp_sr_mode : in BIT;      -- dp_sr_mode
dp_sr_mask : in BIT;      -- dp_sr_mask
sts_ctlrw : out BIT_VECTOR(3 downto 2);     -- sts_ctlrw
sts_wenable : out BIT_VECTOR(5 downto 0);   -- sts_wenable
seq_ctlopy : out BIT_VECTOR(4 downto 0);      -- seq_ctlopy
seq_ctlopx : out BIT_VECTOR(5 downto 0);    -- seq_ctlopx
ad_n_31:in bit;
ad_n_1:in bit;
ad_n_0:in bit;
  -- Connecteur utilises en interne ET le datapath
seq_ctlrw: inout bit_vector(3 downto 0);
seq_ctlalu: inout bit_vector(4 downto 0);
seq_wenable: inout bit_vector(8 downto 0);

  vdd : in BIT; -- vdd
  vss : in BIT  -- vss

  );

END dlxm_ctl;

-- Architecture Declaration

ARCHITECTURE structural_view OF dlxm_ctl IS
  COMPONENT dlxm_seq
    port (
    ck : in BIT;	-- ck
    frz : in BIT;	-- frz
    rqs : in BIT;	-- rqs
    reset : in BIT;	-- reset flag from status
    resnul : in BIT;	-- resnul
    ir_opcod : in BIT_VECTOR(5 DOWNTO 0);	-- ir_opcod
    ir_tvr : in BIT;	-- ir_tvr
    ir_iar : in BIT;	-- ir_iar
    ir_sr : in BIT;	-- ir_sr
    vdd : in BIT;	-- vdd
    vss : in BIT;	-- vss
    scin : in BIT;	-- scin
    test : in BIT;	-- test
    ovr_en : out BIT;	-- ovr_en
    ico : out BIT;      -- ico
    priv : out BIT;	-- priv 
    iformt : out BIT;	-- iformt 
    riformt : out BIT;	-- iformt 
    ctlopx : out BIT_VECTOR(5 DOWNTO 0);	-- ctlopx
    ctlopy : out BIT_VECTOR(4 DOWNTO 0);	-- ctlopy
    ctlalu : out BIT_VECTOR(4 DOWNTO 0);	-- ctlalu
    wenable : out BIT_VECTOR(8 DOWNTO 0);	-- wenable
    ctlrw : out BIT_VECTOR(3 DOWNTO 0);	-- ctlrw
    ctladr : out BIT;	-- ctladr
    scout : out BIT	-- scout
    );
  END COMPONENT;

  COMPONENT dlxm_sts
    port (
    ck : in BIT;	-- ck
    frz : in BIT;	-- frz
    reset : in BIT;	-- external reset
    test : in BIT;	-- test
    ctlalu : in BIT_VECTOR(4 DOWNTO 0);	-- ctlalu
    opx_sign : in BIT;	-- opx_sign
    opy_sign : in BIT;	-- opy_sign
    alu_sign : in BIT;	-- alu_sign
    alu_nul : in BIT;	-- alu_nul
    alu_c31 : in BIT;	-- alu_c31_n
    alu_c30 : in BIT;	-- alu_c30_n
    ovr_en : in BIT;	-- ovr_en
    ico : in BIT;   -- illegal codop
    priv : in BIT;  -- privil 
    iformt : in BIT;  -- format i 
    riformt : in BIT;  -- format i 
    rs : in BIT_VECTOR(4 DOWNTO 0);	-- rs
    rd : in BIT_VECTOR(4 DOWNTO 0);	-- rd
    rdrt : in BIT_VECTOR(4 DOWNTO 0);	-- rdrt
    mxrs_rdrt : in BIT;	-- mxrs_rdrt
    wenable_in : in BIT_VECTOR(8 DOWNTO 0);	-- wenable_in
    ctlrw_in : in BIT_VECTOR(3 DOWNTO 0);	-- ctlrw_in
    sr_mode : in BIT;	-- sr_mode
    sr_mask : in BIT;	-- sr_mask
    adr0 : in BIT;	-- adr0
    adr1 : in BIT;	-- adr1
    adr31 : in BIT;	-- adr31
    intrqs : in BIT_VECTOR(3 DOWNTO 0);	-- intrqs
    scin : in BIT;	-- scin
    alu_test : out BIT;	-- alu_test
    redpnt : out BIT_VECTOR(4 DOWNTO 0);	-- redpnt
    wrtpnt : out BIT_VECTOR(4 DOWNTO 0);	-- wrtpnt
    wenable_out : out BIT_VECTOR(5 DOWNTO 0);	-- wenable_out
    ctlrw_out : out BIT_VECTOR(3 DOWNTO 2);	-- ctlrw_out
    sr_ovr : out BIT;	-- sr_ovr
    sr_dav : out BIT;	-- sr_dav
    sr_iav : out BIT;	-- sr_iav
    sr_ico : out BIT;	-- sr_ico
    sr_it0 : out BIT;	-- sr_it0
    sr_it1 : out BIT;	-- sr_it1
    sr_it2 : out BIT;	-- sr_it2
    sr_it3 : out BIT;	-- sr_it3
    sr_cpurst : out BIT;	-- cpureset flag to datapath
    sts_reset : out BIT;        -- reset flag to sequencer
    rqs : out BIT;	-- rqs
    rw : out BIT;	-- rw
    rw_ctl : out BIT_VECTOR(15 DOWNTO 0);	-- data control
    frz_ctl : out BIT_VECTOR(15 DOWNTO 0);	-- address control
    byte : out BIT_VECTOR(0 to 3);	-- byte
    scout : out BIT;	-- scout
    zero : out BIT;	-- zero
    vdd : in BIT;	-- vdd
    vss : in BIT	-- vss
    );
  END COMPONENT;

  COMPONENT dlxm_boost
  port(i : in bit;
       o : out bit_vector(15 downto 0);
       vss,vdd:in bit);
  END COMPONENT;

  SIGNAL sts_rqs : BIT;	-- sts_rqs
  SIGNAL seq_scout : BIT;	-- seq_scout
  SIGNAL seq_ico : BIT;	-- seq_ico_en
  SIGNAL seq_priv : BIT;
  SIGNAL seq_iformt : BIT;
  SIGNAL seq_riformt : BIT;
  SIGNAL seq_ovr_en : BIT;	-- seq_ovr_en
  SIGNAL sts_reset_seq : BIT ;			-- reset flag from status to seq

BEGIN

  seq : dlxm_seq
    PORT MAP (
    scout => seq_scout,
    ctladr => seq_ctladr,
    ctlrw => seq_ctlrw,
    wenable => seq_wenable,
    ctlalu => seq_ctlalu,
    ctlopy => seq_ctlopy,
    ctlopx => seq_ctlopx,
    ico => seq_ico,
    priv => seq_priv,
    iformt => seq_iformt,
    riformt => seq_riformt,
    ovr_en => seq_ovr_en,
    test => test,
    scin => scin,
    vss => vss,
    vdd => vdd,
    ir_sr => dp_ir_sr,
    ir_iar => dp_ir_iar,
    ir_tvr => dp_ir_tvr,
    ir_opcod => dp_codop,
    resnul => dp_alu_nul,
    reset => sts_reset_seq,			-- reset flag from status
    rqs => sts_rqs,
    frz => frz,
    ck => ck);
  
  sts : dlxm_sts
    PORT MAP (
    vss => vss,
    vdd => vdd,
    zero => sts_zero,
    scout => scout,
    byte => byte(0)& byte(1)& byte(2)& byte(3),
    frz_ctl => frz_ctl,			-- adress ctl, with buffer
    rw_ctl => rw_ctl,			-- data ctl, with buffer
    rw => rw,
    rqs => sts_rqs,
    sts_reset => sts_reset_seq,			-- reset flag to sequencer
    sr_cpurst => sts_sr_cpurst,			-- reset flag to dpt
    sr_it3 => sts_sr_it3,
    sr_it2 => sts_sr_it2,
    sr_it1 => sts_sr_it1,
    sr_it0 => sts_sr_it0,
    sr_ico => sts_sr_ico,
    sr_iav => sts_sr_iav,
    sr_dav => sts_sr_dav,
    sr_ovr => sts_sr_ovr,
    ctlrw_out => sts_ctlrw,
    wenable_out => sts_wenable,
    wrtpnt => sts_wrtpnt,
    redpnt => sts_redpnt,
    alu_test => sts_alu_test,
    scin => seq_scout,
    intrqs => it(3)& it(2)& it(1)& it(0),
    adr31 => ad_n_31,
    adr1 => ad_n_1,
    adr0 => ad_n_0,
    sr_mask => dp_sr_mask,
    sr_mode => dp_sr_mode,
    ctlrw_in => seq_ctlrw,
    wenable_in => seq_wenable,
    mxrs_rdrt => seq_ctlopx(0),
    rdrt => dp_rdrt,
    rd => dp_rd,
    rs => dp_rs,
    ico => seq_ico,
    priv => seq_priv,
    iformt => seq_iformt,
    riformt => seq_riformt,
    ovr_en => seq_ovr_en,
    alu_c30 => dp_alu_c30,
    alu_c31 => dp_alu_c31,
    alu_nul => dp_alu_nul,
    alu_sign => dp_alu_sign,
    opy_sign => dp_opy_sign,
    opx_sign => dp_opx_sign,
    ctlalu => seq_ctlalu,
    test => test,
    reset => reset,			-- external reset
    frz => frz,
    ck => ck);

end structural_view;
