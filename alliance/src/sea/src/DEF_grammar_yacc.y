%{
/*
 * $Author: jpc $
 * $Id: DEF_grammar_yacc.y,v 1.1 2002/04/25 16:16:20 jpc Exp $
 * $Date: 2002/04/25 16:16:20 $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "util_Defs.h"
#include "DEF_actions.h"


  /* Flex substitutions. */
# define        yylex              DEF_grammarlex
# define        yyin               DEF_grammarin
# define        yylineno           DEF_grammarlineno


#define YYDEBUG             1
#define YYERROR_VERBOSE         1

extern int      yylineno;
extern char    *yytext;

extern void     yyerror(char *str);
extern int      yylex(void);

%}

%union {
    double       n;
    char        *s;
    struct pt   *pt;
};

%token    <n>    T_Number
%token    <s>    T_Ident
%token    <s>    T_String
%token    <s>    T_History

%type     <n>    orient
%type     <n>    direction
%type     <pt>   pt
%type     <s>    track_start
%type     <s>    track_layer_statement
%type     <s>    track_layer
%type     <s>    track_layers

%token        T_ALIGN T_ARRAY T_ASSERTIONS T_BEGINEXT T_BOTTOMLEFT
%token        T_BUSBITCHARS T_BY T_CANNOTOCCUPY T_CANPLACE T_CAPACITANCE
%token        T_COMMONSCANPINS T_COMPONENT T_COMPONENTPIN T_COMPS
%token        T_COMP_GEN T_CONSTRAINTS T_COVER T_DEFAULTCAP T_DESIGN
%token        T_DIE_AREA T_DIFF T_DIRECTION T_DISTANCE T_DIVIDERCHAR T_DO
%token        T_DRIVECELL T_E T_EEQMASTER T_END T_EQUAL T_ESTCAP T_FALL
%token        T_FALLMAX T_FALLMIN T_FE T_FIXED T_FLOATING T_FLOORPLAN T_FN
%token        T_FOREIGN T_FPC T_FROMCLOCKPIN T_FROMCOMPPIN T_FROMIOPIN
%token        T_FROMPIN T_FS T_FW T_GCELLGRID T_GROUP T_GROUPS T_HISTORY
%token        T_HOLDFALL T_HOLDRISE T_HORIZONTAL T_INTEGER T_IOTIMINGS
%token        T_LAYER T_MACRO T_MAX T_MAXDIST T_MAXHALFPERIMETER T_MAXX
%token        T_MAXY T_MICRONS T_MIN T_MINPINS T_MUSTJOIN T_N
%token        T_NAMEMAPSTRING T_NAMESCASESENSITIVE T_NET T_NETS T_NEW
%token        T_NONDEFAULTRULE T_NOSHIELD T_ORDERED T_ORIGINAL T_PARALLEL
%token        T_PARTITIONS T_PATH T_PATTERN T_PATTERNNAME T_PIN
%token        T_PINPROPERTIES T_PINS T_PLACED T_PROPERTY
%token        T_PROPERTYDEFINITIONS T_RANGE T_REAL T_RECT T_REENTRANTPATHS
%token        T_REGION T_REGIONS T_RISE T_RISEMAX T_RISEMIN T_ROUTED T_ROW
%token        T_ROWS T_S T_SCANCHAINS T_SETUPFALL T_SETUPRISE T_SHAPE
%token        T_SHIELD T_SHIELDNET T_SITE T_SLEWRATE T_SNET T_SNETS T_SOFT
%token        T_SOURCE T_SPACING T_SPECIAL T_START T_STEP T_STOP T_STYLE
%token        T_SUBNET T_SUM T_SYNTHESIZED T_TAPER T_TAPERRULE T_TECH
%token        T_THRUPIN T_TIMINGDISABLES T_TOCLOCKPIN T_TOCOMPPIN T_TOIOPIN
%token        T_TOPIN T_TOPRIGHT T_TRACKS T_TURNOFF T_UNITS T_UNPLACED T_USE
%token        T_VARIABLE T_VERSION T_VERTICAL T_VIAS T_VOLTAGE T_VPIN T_W
%token        T_WEIGHT T_WIDTH T_WIRECAP T_WIREDLOGIC T_XTALK
%token        T_INPUT T_OUTPUT T_INOUT T_FEEDTHRU

%%

def_file
    : version_stmt case_sens_stmt rules
    | version_stmt case_sens_stmt rules end_design
    ;

version_stmt
    : /* empty */
    | T_VERSION T_Number ';'
    ;

case_sens_stmt
    : /* empty */
    | T_NAMESCASESENSITIVE T_Ident ';'
    ;

rules
    : /* empty */
    | rules rule
    | error
    ;

rule
    : design_section
    | via_section
    | extension_section
    | comps_section
    | nets_section
    | snets_section
    | groups_section
    | scanchains_section
    | constraint_section
    | assertions_section
    | iotiming_section
    | regions_section
    | floorplan_contraints_section
    | timingdisables_section
    | partitions_section
    | pin_props_section
    ;

design_section
    : design_name
    | tech_name
    | history
    | units
    | divider_char
    | bus_bit_chars
    | site
    | canplace
    | cannotoccupy
    | die_area
    | pin_cap_rule
    | pin_rule
    | tracks_rule
    | gcellgrid
    | array_name
    | floorplan_name
    | row_rule
    | prop_def_section
    ;

design_name
    : T_DESIGN T_Ident ';'                  { def_design_name($2); }
    ;

end_design
    : T_END T_DESIGN                        { def_end_design(); }
    ;

tech_name
    : T_TECH T_Ident ';'                    { def_tech_name($2); }
    ;

array_name
    : T_ARRAY T_Ident ';'
    ;

floorplan_name
    : T_FLOORPLAN T_Ident ';'
    ;

history
    : T_HISTORY T_History ';'
    ;

prop_def_section
    : T_PROPERTYDEFINITIONS property_defs T_END T_PROPERTYDEFINITIONS
    ;

property_defs
    : /* empty */
    | property_defs property_def
    ;

property_def
    : T_DESIGN T_Ident property_type_and_val ';'
    | T_NET T_Ident property_type_and_val ';'
    | T_SNET T_Ident property_type_and_val ';'
    | T_REGION T_Ident property_type_and_val ';'
    | T_GROUP T_Ident property_type_and_val ';'
    | T_COMPONENT T_Ident property_type_and_val ';'
    | T_ROW T_Ident property_type_and_val ';'
    | T_PIN T_Ident property_type_and_val ';'
    | T_COMPONENTPIN T_Ident property_type_and_val ';'
    ;

property_type_and_val
    : T_INTEGER opt_range opt_num_val                {}
    | T_REAL opt_range opt_num_val                    {}
    | T_Ident                            {}
    | T_Ident T_String                        {}
    | T_NAMEMAPSTRING T_Ident                    {}
    ;

opt_num_val
    : /* empty */                            {}
    | T_Number                            {}
    ;

units
    : T_UNITS T_DISTANCE T_MICRONS T_Number ';' { def_units ($4); }
    ;

divider_char
    : T_DIVIDERCHAR T_String ';'
    ;

bus_bit_chars
    : T_BUSBITCHARS T_String ';'
    ;

site
    : T_SITE T_Ident T_Number T_Number orient T_DO T_Number T_BY T_Number T_STEP T_Number T_Number ';'
    ;

canplace
    : T_CANPLACE T_Ident T_Number T_Number orient T_DO T_Number T_BY T_Number T_STEP T_Number T_Number ';'
    ;

cannotoccupy
    : T_CANNOTOCCUPY T_Ident T_Number T_Number orient T_DO T_Number T_BY T_Number T_STEP T_Number T_Number ';'
    ;

orient
    : T_N   {  $$ = DEF_N; }
    | T_E   {  $$ = DEF_E; }
    | T_S   {  $$ = DEF_S; }
    | T_W   {  $$ = DEF_W; }
    | T_FN  {  $$ = DEF_FN; }
    | T_FE  {  $$ = DEF_FE; }
    | T_FS  {  $$ = DEF_FS; }
    | T_FW  {  $$ = DEF_FW; }
    ;

direction
    : T_INPUT    { $$ = DEF_INPUT; }
    | T_OUTPUT   { $$ = DEF_OUTPUT; }
    | T_INOUT    { $$ = DEF_INOUT; }
    | T_FEEDTHRU { $$ = DEF_FEEDTHRU; }
    ;

die_area
    : T_DIE_AREA pt pt ';'   { def_die_area ($2, $3); }
    ;

pin_cap_rule
    : start_def_cap pin_caps end_def_cap
    ;

start_def_cap
    : T_DEFAULTCAP T_Number
    ;

pin_caps
    : /* empty */
    | pin_caps pin_cap
    ;

pin_cap
    : T_MINPINS T_Number T_WIRECAP T_Number ';'
    ;

end_def_cap
    : T_END T_DEFAULTCAP
    ;

pin_rule
    : start_pins pins end_pins
    ;

start_pins
    : T_PINS T_Number ';'    { def_start_pins($2); }
    ;

pins
    : /* empty */
    | pins pin
    ;

pin
    : pin_start pin_options ';'    { def_pin_options (); }
    ;

pin_start
    : '-' T_Ident '+' T_NET T_Ident    { def_pin_start($2, $5); }
    ;

pin_options
    : /* empty */
    | pin_options pin_option
    ;

pin_option
    : '+' T_SPECIAL
    | extension_stmt
    | '+' T_DIRECTION direction     { def_pin_option_direction ($3); }
    | '+' T_USE T_Ident
    | '+' T_LAYER T_Ident pt pt     { def_pin_option_layer ($3, $4, $5); }
    | placement_status pt orient    { def_pin_option_place ($2, $3); }
    ;

end_pins
    : T_END T_PINS    { def_end_pins(); }
    ;

row_rule
    : T_ROW T_Ident T_Ident T_Number T_Number orient T_DO T_Number T_BY T_Number T_STEP T_Number T_Number row_options ';'
      { def_row_rule ($2, $3, $4, $5, $6, $8, $10, $12, $13); }
    ;

row_options
    : /* empty */
    | row_options row_option
    ;

row_option
    : '+' T_PROPERTY row_prop_list
    ;

row_prop_list
    : /* empty */
    | row_prop_list row_prop
    ;

row_prop
    : T_Ident property_val                        {}
    ;

tracks_rule
    : track_start T_Number T_DO T_Number T_STEP T_Number track_layer_statement ';'
      { def_track_rule ($1, $2, $4, $6, $7); }
    ;

track_start
    : T_TRACKS T_Ident    { $$ = $2; }
    ;

track_layer_statement
    : /* empty */          { $$ = ""; }
    | T_LAYER track_layers { $$ = $2; }
    ;

track_layers
    : /* empty */               { $$ = ""; }
    | track_layer track_layers  { $$ = def_track_layers ($1, $2); }
    ;

track_layer
    : T_Ident  { $$ = $1; }
    ;

gcellgrid
    : T_GCELLGRID T_Ident T_Number T_DO T_Number T_STEP T_Number ';'
    ;

extension_section
    : T_BEGINEXT
    ;

extension_stmt
    : '+' T_BEGINEXT
    ;

via_section
    : via via_declarations via_end
    ;

via
    : T_VIAS T_Number ';'  { def_start_vias ($2); }
    ;

via_declarations
    : /* empty */
    | via_declarations via_declaration
    ;

via_declaration
    : via_start layer_stmts ';' { def_via_end (); }
    ;

via_start
    : '-' T_Ident  { def_via_start ($2); }
    ;

layer_stmts
    : /* empty */
    | layer_stmts layer_stmt
    ;

layer_stmt
    : '+' T_RECT T_Ident pt pt   { def_via_stmt_rect ($3, $4, $5); }
    | '+' T_PATTERNNAME T_Ident  { def_via_stmt_pattern ($3); }
    | extension_stmt
    ;

pt
    : '(' T_Number T_Number ')'  { $$ = def_alloc_pt (   $2   ,    $3   ); }
    | '('   '*'    T_Number ')'  { $$ = def_alloc_pt (HUGE_VAL,    $3   ); }
    | '(' T_Number   '*'    ')'  { $$ = def_alloc_pt  (  $2   , HUGE_VAL); }
    | '('   '*'      '*'    ')'  { $$ = def_alloc_pt (HUGE_VAL, HUGE_VAL); }
    ;

via_end
    : T_END T_VIAS  { def_end_vias (); }
    ;

regions_section
    : regions_start regions_stmts T_END T_REGIONS
    ;

regions_start
    : T_REGIONS T_Number ';'
    ;

regions_stmts
    : /* empty */
    | regions_stmts regions_stmt
    ;

regions_stmt
    : '-' T_Ident rect_list region_options ';'
    ;

rect_list
    : pt pt           { }
    | rect_list pt pt { }
    ;

region_options
    : /* empty */
    | region_options region_option
    ;

region_option
    : '+' T_PROPERTY region_prop_list
    ;

region_prop_list
    : /* empty */
    | region_prop_list region_prop
    ;

region_prop
    : T_Ident property_val                        {}
    ;

comps_section
    : start_comps comps_rule end_comps        
    ;

start_comps
    : T_COMPS T_Number ';'                        { def_start_comps($2); }
    ;

comps_rule
    : /* empty */
    | comps_rule comp
    ;

comp
    : comp_start comp_net_list comp_options ';'
    ;

comp_start
    : '-' T_Ident T_Ident                        { def_comp_start($2, $3); }
    ;

comp_net_list
    : /* empty */                            {}
    | comp_net_list '*'                        { def_comp_net_list("*"); }
    | comp_net_list T_Ident                        { def_comp_net_list($2); }
    ;

comp_options
    : /* empty */
    | comp_options comp_option
    ;

comp_option
    : comp_generate
    | comp_source
    | comp_type
    | weight
    | comp_foreign
    | comp_region
    | comp_eeq
    | comp_property
    | comp_extension_stmt
    ;

comp_extension_stmt
    : extension_stmt
    ;

comp_eeq
    : '+' T_EEQMASTER T_Ident
    ;

comp_generate
    : '+' T_COMP_GEN T_Ident opt_pattern
    ;

opt_pattern
    : /* empty */                            {}
    | T_Ident                            {}
    ;

comp_source
    : '+' T_SOURCE T_Ident
    ;

comp_region
    : comp_region_start comp_pnt_list
    | comp_region_start T_Ident
    ;

comp_pnt_list
    : pt pt               { }
    | comp_pnt_list pt pt { }
    ;

comp_property
    : '+' T_PROPERTY comp_prop_list
    ;

comp_prop_list
    : /* empty */
    | comp_prop_list comp_prop
    ;

comp_prop
    : T_Ident property_val                        {}
    ;

comp_region_start
    : '+' T_REGION
    ;

comp_foreign
    : '+' T_FOREIGN T_Ident opt_paren T_Ident
    ;

opt_paren
    : pt                                {}
    | T_Number T_Number                        {}
    ;

comp_type
    : placement_status pt orient { def_comp_type ($2, $3); }
    ;

placement_status
    : '+' T_FIXED
    | '+' T_COVER
    | '+' T_PLACED
    | '+' T_UNPLACED
    ;

weight
    : '+' T_WEIGHT T_Number
    ;

end_comps
    : T_END T_COMPS                            { def_end_comps(); }
    ;

nets_section
    : start_nets net_rules end_nets
    ;

start_nets
    : T_NETS T_Number ';'                        { def_start_nets($2); }
    ;

net_rules
    : /* empty */
    | net_rules one_net
    ;

one_net
    : net_start net_connections net_options ';'
    ;

net_start
    : '-' T_Ident                            { def_net_start($2); }
    ;

net_connections
    : /* empty */
    | net_connections net_connection
    ;

net_connection
    : '(' net_conn_start net_conn_opt ')'
    | T_MUSTJOIN '('  net_conn_start net_conn_opt ')'
    ;

net_conn_start
    : T_Ident T_Ident                        { def_net_conn_start($1, $2); }
    | T_PIN T_Ident                            { def_net_conn_start("PIN", $2); }
    ;

net_conn_opt
    : /* empty */
    | extension_stmt
    | '+' T_SYNTHESIZED                        { def_net_conn_opt_synthesized(); }
    ;

net_options
    : /* empty */
    | net_options net_option
    ;

net_option
    : '+' net_type paths
    | '+' T_SOURCE T_Ident
    | '+' T_ORIGINAL T_Ident
    | '+' T_PATTERN T_Ident
    | '+' T_WEIGHT T_Number
    | '+' T_XTALK T_Number
    | '+' T_ESTCAP T_Number
    | '+' T_USE T_Ident
    | '+' T_STYLE T_Ident
    | '+' T_NONDEFAULTRULE T_Ident
    | vpin_stmt
    | '+' T_SHIELDNET T_Ident
    | '+' T_NOSHIELD paths
    | '+' T_SUBNET T_Ident comp_names subnet_options
    | '+' T_PROPERTY net_prop_list
    | extension_stmt
    ;

net_prop_list
    : /* empty */
    | net_prop_list net_prop
    ;

net_prop
    : T_Ident property_val                        {}
    ;

vpin_stmt
    : vpin_begin vpin_layer_opt pt pt vpin_options
    ;

vpin_begin
    : '+' T_VPIN T_Ident
    ;

vpin_layer_opt
    : /* empty */
    | T_LAYER T_Ident
    ;

property_val
    : T_Number                            {}
    | T_String                            {}
    ;

vpin_options
    : /* empty */
    | vpin_status pt orient
    ;

vpin_status
    : T_PLACED
    | T_FIXED
    | T_COVER
    ;

net_type
    : T_FIXED
    | T_COVER
    | T_ROUTED
    ;

paths
    : path
    | paths new_path
    ;

new_path
    : T_NEW path
    ;

path
    : path_start opt_width opt_shape opt_taper path_pt path_item_list
    ;

path_start
    : T_Ident  { def_path_start ($1); }
    ;

path_item_list
    : /* empty */
    | path_item_list path_item
    ;

path_item
    : T_Ident                             { def_path_item_via ($1); }
    | path_pt
    ;

path_pt
    : '(' T_Number T_Number ')'           { def_path_item_pt (def_alloc_pt (   $2   ,    $3   )); }
    | '(' '*' T_Number ')'                { def_path_item_pt (def_alloc_pt (HUGE_VAL,    $3   )); }
    | '(' T_Number '*' ')'                { def_path_item_pt (def_alloc_pt (   $2   , HUGE_VAL)); }
    | '(' '*' '*' ')'                     { def_path_item_pt (def_alloc_pt (HUGE_VAL, HUGE_VAL)); }
    | '(' T_Number T_Number T_Number ')'  { def_path_item_pt (def_alloc_pt (   $2   ,    $3   )); }
    | '(' '*' T_Number T_Number ')'       { def_path_item_pt (def_alloc_pt (HUGE_VAL,    $3   )); }
    | '(' T_Number '*' T_Number ')'       { def_path_item_pt (def_alloc_pt (   $2   , HUGE_VAL)); }
    | '(' '*' '*' T_Number ')'            { def_path_item_pt (def_alloc_pt (HUGE_VAL, HUGE_VAL)); }
    ;

opt_taper
    : /* empty */
    | T_TAPER
    | T_TAPERRULE T_Ident
    ;

opt_shape
    : /* empty */
    | '+' T_SHAPE T_Ident
    ;

opt_width
    : /* empty */  {}
    | T_Number     { def_opt_width ($1); }
    ;

end_nets
    : T_END T_NETS                            { def_end_nets(); }
    ;

snets_section
    : start_snets snet_rules end_snets
    ;

snet_rules
    : /* empty */
    | snet_rules snet_rule
    ;

snet_rule
    : snet_start snet_connections snet_options ';'
    ;

snet_start
    : '-' T_Ident  { def_snet_start($2); }
    ;

snet_connections
    : /* empty */
    | snet_connections snet_connection
    ;

snet_connection
    : '(' snet_conn_start snet_conn_opt ')'
    ;

snet_conn_start
    : T_Ident T_Ident  { def_snet_conn_start($1, $2); }
    | T_PIN T_Ident    { def_snet_conn_start("PIN", $2); }
    | '*' T_Ident      { def_snet_conn_start("PIN", $2); }
    ;

snet_conn_opt
    : /* empty */
    | extension_stmt
    | '+' T_SYNTHESIZED  { def_snet_conn_opt_synthesized(); }
    ;

snet_options
    : /* empty */
    | snet_options snet_option
    ;

snet_option
    : snet_width
    | snet_voltage
    | snet_spacing
    | snet_other_option
    ;

snet_other_option
    : '+' net_type paths
    | '+' T_SHIELD T_Ident paths
    | '+' T_SOURCE T_Ident
    | '+' T_ORIGINAL T_Ident
    | '+' T_PATTERN T_Ident
    | '+' T_WEIGHT T_Number
    | '+' T_ESTCAP T_Number
    | '+' T_USE T_Ident
    | '+' T_STYLE T_Ident
    | '+' T_PROPERTY net_prop_list
    | extension_stmt
    ;

snet_width
    : '+' T_WIDTH T_Ident T_Number
    ;

snet_voltage
    : '+' T_VOLTAGE T_Number
    ;

snet_spacing
    : '+' T_SPACING T_Ident T_Number opt_snet_range
    ;

opt_snet_range
    : /* nothing */
    | T_RANGE T_Number T_Number
    ;

opt_range
    : /* nothing */
    | T_RANGE T_Number T_Number
    ;

start_snets
    : T_SNETS T_Number ';'                        { def_start_snets($2); }
    ;

end_snets
    : T_END T_SNETS                            { def_end_snets(); }
    ;

groups_section
    : groups_start group_rules groups_end
    ;

groups_start
    : T_GROUPS T_Number ';'
    ;

group_rules
    : /* empty */
    | group_rules group_rule
    ;

group_rule
    : start_group group_members group_options ';'
    ;

start_group
    : '-' T_Ident
    ;

group_members
    : /* empty */
    | group_members group_member
    ;

group_member
    : T_Ident                    {}
    ;

group_options
    : /* empty */
    | group_options group_option
    ;

group_option
    : '+' T_SOFT group_soft_options
    | '+' T_PROPERTY group_prop_list
    | '+' T_REGION group_region
    | extension_stmt
    ;

group_region
    : pt pt                        {}
    | T_Ident                    {}
    ;

group_prop_list
    : /* empty */
    | group_prop_list group_prop
    ;

group_prop
    : T_Ident property_val                {}
    ;

group_soft_options
    : /* empty */
    | group_soft_options group_soft_option
    ;

group_soft_option
    : T_MAXX T_Number
    | T_MAXY T_Number
    | T_MAXHALFPERIMETER T_Number
    ;

groups_end
    : T_END T_GROUPS
    ;

assertions_section
    : assertions_start constraint_rules assertions_end
    ;

constraint_section
    : constraints_start constraint_rules constraints_end
    ;

assertions_start
    : T_ASSERTIONS T_Number ';'
    ;

constraints_start
    : T_CONSTRAINTS T_Number ';'
    ;

constraint_rules
    : /* empty */
    | constraint_rules constraint_rule
    ;

constraint_rule
    : operand_rule
    | wiredlogic_rule
    ;

operand_rule
    : '-' operand delay_specs ';'
    ;

operand
    : T_NET T_Ident
    | T_PATH T_Ident T_Ident T_Ident T_Ident
    | T_SUM '(' operand_list ')'
    | T_DIFF '(' operand_list ')'
    ;

operand_list
    : operand
    | operand_list ',' operand
    ;

wiredlogic_rule
    : '-' T_WIREDLOGIC T_Ident opt_plus T_MAXDIST T_Number ';'
    ;

opt_plus
    : /* empty */
    | '+'
    ;

delay_specs
    : /* empty */
    | delay_specs delay_spec
    ;

delay_spec
    : '+' T_RISEMIN T_Number
    | '+' T_RISEMAX T_Number
    | '+' T_FALLMIN T_Number
    | '+' T_FALLMAX T_Number
    ;

constraints_end
    : T_END T_CONSTRAINTS
    ;

assertions_end
    : T_END T_ASSERTIONS
    ;

scanchains_section
    : scanchain_start scanchain_rules scanchain_end
    ;

scanchain_start
    : T_SCANCHAINS T_Number ';'
    ;

scanchain_rules
    : /* empty */
    | scanchain_rules scan_rule
    ;

scan_rule
    : start_scan scan_members ';'
    ;

start_scan
    : '-' T_Ident
    ;

scan_members
    : /* empty */
    | scan_members scan_member
    ;

opt_pin
    : /* empty */                    {}
    | T_Ident                    {}
    ;

scan_member
    : '+' T_START T_Ident opt_pin
    | '+' T_FLOATING floating_inst_list
    | '+' T_ORDERED ordered_inst_list
    | '+' T_STOP T_Ident opt_pin
    | '+' T_COMMONSCANPINS opt_common_pins
    | extension_stmt
    ;

opt_common_pins
    : /* empty */
    | '(' T_Ident T_Ident ')'
    | '(' T_Ident T_Ident ')' '(' T_Ident T_Ident ')'
    ;

floating_inst_list
    : /* empty */
    | floating_inst_list one_floating_inst
    ;

one_floating_inst
    : T_Ident floating_pins                {}
    ;

floating_pins
    : /* empty */
    | '(' T_Ident T_Ident ')'
    | '(' T_Ident T_Ident ')' '(' T_Ident T_Ident ')'
    ;

ordered_inst_list
    : /* empty */
    | ordered_inst_list one_ordered_inst
    ;

one_ordered_inst
    : T_Ident ordered_pins                {}
    ;

ordered_pins
    : /* empty */
    | '(' T_Ident T_Ident ')'
    | '(' T_Ident T_Ident ')' '(' T_Ident T_Ident ')'
    ;

scanchain_end
    : T_END T_SCANCHAINS
    ;

iotiming_section
    : iotiming_start iotiming_rules iotiming_end
    ;

iotiming_start
    : T_IOTIMINGS T_Number ';'
    ;

iotiming_rules
    : /* empty */
    | iotiming_rules iotiming_rule
    ;

iotiming_rule
    : start_iotiming iotiming_members ';'
    ;

start_iotiming
    : '-' '(' T_Ident T_Ident ')'
    ;

iotiming_members
    : /* empty */
    | iotiming_members iotiming_member
    ;

iotiming_member
    : '+' risefall T_VARIABLE T_Number T_Number
    | '+' risefall T_SLEWRATE T_Number T_Number
    | '+' T_CAPACITANCE T_Number
    | '+' T_DRIVECELL T_Ident
    | T_FROMPIN T_Ident
    | T_TOPIN T_Ident
    | T_PARALLEL T_Number
    | extension_stmt
    ;

risefall
    : T_RISE
    | T_FALL
    ;

iotiming_end
    : T_END T_IOTIMINGS
    ;

floorplan_contraints_section
    : fp_start fp_stmts T_END T_FPC
    ;

fp_start
    : T_FPC T_Number ';'
    ;

fp_stmts
    : /* empty */
    | fp_stmts fp_stmt
    ;

fp_stmt
    : '-' T_Ident h_or_v constraint_type constrain_what_list ';'
    ;

h_or_v
    : T_HORIZONTAL
    | T_VERTICAL
    ;

constraint_type
    : T_ALIGN
    | T_MAX T_Number
    | T_MIN T_Number
    | T_EQUAL T_Number
    ;

constrain_what_list
    : /* empty */
    | constrain_what_list constrain_what
    ;

constrain_what
    : '+' T_BOTTOMLEFT row_or_comp_list
    | '+' T_TOPRIGHT row_or_comp_list
    ;

row_or_comp_list
    : /* empty */
    | row_or_comp_list row_or_comp
    ;

row_or_comp
    : '(' T_ROWS T_Ident ')'
    | '(' T_COMPS T_Ident ')'
    ;

timingdisables_section
    : timingdisables_start timingdisables_rules timingdisables_end
    ;

timingdisables_start
    : T_TIMINGDISABLES T_Number ';'
    ;

timingdisables_rules
    : /* empty */
    | timingdisables_rules timingdisables_rule
    ;

timingdisables_rule
    : '-' T_FROMPIN T_Ident T_Ident T_TOPIN T_Ident T_Ident ';'
    | '-' T_THRUPIN T_Ident T_Ident ';'
    | '-' T_MACRO T_Ident td_macro_option ';'
    | '-' T_REENTRANTPATHS ';'
    ;

td_macro_option
    : T_FROMPIN T_Ident T_TOPIN T_Ident
    | T_THRUPIN T_Ident
    ;

timingdisables_end
    : T_END T_TIMINGDISABLES
    ;

partitions_section
    : partitions_start partition_rules partitions_end
    ;

partitions_start
    : T_PARTITIONS T_Number ';'
    ;

partition_rules
    : /* empty */
    | partition_rules partition_rule
    ;

partition_rule
    : start_partition partition_members ';'
    ;

start_partition
    : '-' T_Ident turnoff
    ;

turnoff
    : /* empty */
    | T_TURNOFF turnoff_setup turnoff_hold
    ;

turnoff_setup
    : /* empty */
    | T_SETUPRISE
    | T_SETUPFALL
    ;

turnoff_hold
    : /* empty */
    | T_HOLDRISE
    | T_HOLDFALL
    ;

partition_members
    : /* empty */
    | partition_members partition_member
    ;

partition_member
    : '+' T_FROMCLOCKPIN T_Ident T_Ident risefall minmaxpins
    | '+' T_FROMCOMPPIN T_Ident T_Ident risefallminmax2_list
    | '+' T_FROMIOPIN T_Ident risefallminmax1_list
    | '+' T_TOCLOCKPIN T_Ident T_Ident risefall minmaxpins
    | '+' T_TOCOMPPIN T_Ident T_Ident risefallminmax2_list
    | '+' T_TOIOPIN T_Ident risefallminmax1_list
    | extension_stmt
    ;

minmaxpins
    : min_or_max_list T_PINS pin_list
    ;

min_or_max_list
    : /* empty */
    | min_or_max_list min_or_max_member
    ;

min_or_max_member
    : T_MIN T_Number T_Number
    | T_MAX T_Number T_Number
    ;

pin_list
    : /* empty */
    | pin_list T_Ident
    ;

risefallminmax1_list
    : /* empty */
    | risefallminmax1_list risefallminmax1
    ;

risefallminmax1
    : T_RISEMIN T_Number
    | T_FALLMIN T_Number
    | T_RISEMAX T_Number
    | T_FALLMAX T_Number
    ;

risefallminmax2_list
    : risefallminmax2
    | risefallminmax2_list risefallminmax2
    ;

risefallminmax2
    : T_RISEMIN T_Number T_Number
    | T_FALLMIN T_Number T_Number
    | T_RISEMAX T_Number T_Number
    | T_FALLMAX T_Number T_Number
    ;

partitions_end
    : T_END T_PARTITIONS
    ;

comp_names
    : /* empty */
    | comp_names comp_name
    ;

comp_name
    : '(' T_Ident T_Ident subnet_opt_syn ')'
    ;

subnet_opt_syn
    : /* empty */
    | '+' T_SYNTHESIZED
    ;

subnet_options
    : /* empty */
    | subnet_options subnet_option
    ;

subnet_option
    : net_type paths
    | T_NONDEFAULTRULE T_Ident
    ;

pin_props_section
    : begin_pin_props pin_prop_list end_pin_props
    ;

begin_pin_props
    : T_PINPROPERTIES T_Number opt_semi
    ;

opt_semi
    : /* empty */
    | ';'
    ;

end_pin_props
    : T_END T_PINPROPERTIES
    ;

pin_prop_list
    : /* empty */
    | pin_prop_list pin_prop_terminal
    ;

pin_prop_terminal
    : '-' T_Ident T_Ident pin_prop_options ';'
    ;

pin_prop_options
    : /* empty */
    | pin_prop_options pin_prop
    ;

pin_prop
    : '+' T_PROPERTY pin_prop_name_value_list
    ;

pin_prop_name_value_list
    : /* empty */
    | pin_prop_name_value_list pin_prop_name_value
    ;

pin_prop_name_value
    : T_Ident property_val                {}
    ;

%%

void
yyerror(char *str)
{
  def_parse_error(str);
}

