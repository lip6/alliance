wave CLK
wave RST
wave STB
wave DONE
wave -dec A
wave -dec B
wave -dec RESULT

force clk 1 50 , 0 100 -repeat 100
force RST 1
run
force RST 0
force STB 1
force A   10#123
force B   10#12
run
