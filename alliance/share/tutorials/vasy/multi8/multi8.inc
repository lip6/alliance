trace /MULTI8/U3/STATE

cd MULTI8
trace A
trace B
trace CLK
trace RST
trace STB
trace RESULT
trace DONE

assign ("00000111") A
assign ("00000011") B
assign ('1') RST

assign ('0') CLK
run 50
assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('0') RST
assign ('1') STB

run 50
assign ('1') CLK
run 50
assign ('0') CLK
run 50

run 50
assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50

assign ('1') CLK
run 50
assign ('0') CLK
run 50
