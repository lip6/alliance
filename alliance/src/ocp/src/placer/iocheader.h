#ifndef __IOC_HEADER_H
#define __IOC_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct connector
{
    struct connector	*NEXT	;
    char		ORIENT	;
    char		*NAME	;
    int			VALUE	; // in case of SPACE
} con_list;

extern con_list* add_con(con_list* ptlist, char orient, char* name);
extern con_list* add_space(con_list* ptlist, char orient, char* value);
extern con_list* iocparse(char *file);

#ifdef __cplusplus
}
#endif

#endif /* __IOC_HEADER_H */
