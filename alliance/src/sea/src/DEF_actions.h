/*
 * $Author: jpc $
 * $Id: DEF_actions.h,v 1.1 2002/04/25 16:16:20 jpc Exp $
 * $Date: 2002/04/25 16:16:20 $
 */

#ifndef __deffunc_h
#define __deffunc_h

#ifdef __cplusplus
extern "C" {
#endif

#define        DEF_NOSYM      0x0000
#define        DEF_SYM_X      0x0001
#define        DEF_SYM_Y      0x0002
#define        DEF_ROT_P      0x0004
#define        DEF_ROT_M      0x0008

#define        DEF_N          (DEF_NOSYM)
#define        DEF_S          (DEF_SYM_X | DEF_SYM_Y)
#define        DEF_W          (DEF_ROT_P)
#define        DEF_E          (DEF_ROT_M)
#define        DEF_FN         (DEF_SYM_X)
#define        DEF_FS         (DEF_SYM_Y)
#define        DEF_FW         (DEF_SYM_Y | DEF_ROT_M)
#define        DEF_FE         (DEF_SYM_Y | DEF_ROT_P)

#define        DEF_INPUT     10
#define        DEF_OUTPUT    11
#define        DEF_INOUT     12
#define        DEF_FEEDTHRU  13

struct pt { double x, y; };

extern void	 def_design_name(char *name);
extern void	 def_tech_name(char *name);
extern void	 def_end_design(void);

extern void  def_units(double number);

extern void  def_die_area(struct pt *ab1, struct pt *ab2);

extern void  def_row_rule(char *row_name,
                          char *row_type,
                          double x,
                          double y,
                          double orient,
                          double do_number,
                          double by_number,
                          double step_x,
                          double step_y);

extern void  def_track_rule(char *axis_name,
                            double start,
                            double do_number,
                            double step,
                            char *layer_list);
extern char *def_track_layers(char *layer_name, char *layer_list);

extern void	 def_start_pins(double number);
extern void	 def_end_pins(void);
extern void	 def_pin_start(char *pin_name, char *net_name);
extern void	 def_pin_options(void);
extern void	 def_pin_option_direction(double dir);
extern void	 def_pin_option_place(struct pt *pt, double orient);
extern void	 def_pin_option_layer(char *layer_name, struct pt *pt1, struct pt *pt2);

extern void	 def_start_vias(double number);
extern void	 def_end_vias(void);
extern void	 def_via_start(char *via_name);
extern void	 def_via_stmt_rect(char *layer_name, struct pt *pt1, struct pt *pt2);
extern void	 def_via_stmt_pattern(char *pattern_name);
extern void	 def_via_end(void);

extern void	 def_start_comps(double number);
extern void	 def_end_comps(void);
extern void	 def_comp_start(char *ins_name, char *cell_name);
extern void	 def_comp_net_list(char *name);
extern void	 def_comp_type(struct pt* pt, double orient);

extern void	 def_start_nets(double number);
extern void	 def_end_nets(void);
extern void	 def_net_start(char *name);
extern void	 def_net_conn_start(char *ins_name, char *pin_name);
extern void	 def_net_conn_opt_synthesized(void);
extern void	 def_path_start(char *layer_name);
extern void	 def_path_item_via(char *via_name);
extern void	 def_path_item_pt(struct pt *pt1);
extern void	 def_opt_width(double number);

extern void	 def_start_snets(double number);
extern void	 def_end_snets(void);
extern void	 def_snet_start(char *name);
extern void	 def_snet_conn_start(char *ins_name, char *pin_name);
extern void	 def_snet_conn_opt_synthesized(void);

extern struct pt *def_alloc_pt(double x, double y);
extern void  def_free_pt(struct pt *pt);

extern void  def_parse_error(char *str);


#ifdef __cplusplus
}
#endif

#endif /* __deffunc_h */
