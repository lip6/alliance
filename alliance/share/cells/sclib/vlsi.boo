# default boot file for cmos  
# vlsi versus 7  for compose tools

technology idps_symb 1.0

define library_search_path *,cmos
cell_library sclib 	.
cell_library prim_symb /labo/etc/vlsi/idps/prim_symb
cell_library tch_comp  /labo/etc/vlsi/idps/tch_comp
search_path "sclib -> prim_symb -> tch_comp -> cmosch000x"

define compose_set_up "snap grid size=1.0,snap to items=off"
define compose_display "grid size=1,cell AB=on,cell BB=off,global guts=all,view=layout,connector node name=on"
define merge_polygons_completely true
define plotter_1 hp,h7585,/tmp,E
define plotter_2 PS,PS,/tmp/,A4
