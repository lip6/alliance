/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

# include <X11/Xlib.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"

# include "v_view.h"
# include "v_extern.h"
# include "d_asm.h"

d_clean_err()
{
extern BOOL D_ModeTrace;

  D_ModeTrace = TRUE;
  
  V_changeCursor(1);
  if (M_window != 0) { 
    XDestroyWindow (display, M_window);
    M_window = 0;
  }
  if (I_window != 0) { 
    XDestroyWindow (display, I_window);
    I_window = 0;
  }
  if (C_window != 0) { 
    XDestroyWindow (display, C_window);
    C_window = 0;
  }
  E_setMenu();
  E_drawMenu();
}
