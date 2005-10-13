
// -*- C++ -*-
//
// $Id: RMBK.cpp,v 1.12 2005/10/13 12:44:40 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./RMBK.cpp"                              |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "RDefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Modifier  :  "CRBox::mbkload()".

void  CRBox::mbkload (MBK::CFig *mbkfig
                     , int z
                     , int zup
                     , int rtype
                     , bool halfpitch
                     , bool rotate )
{
    MBK::MIns::iterator   itIns, endInstances, endOrphans;
  MBK::MLosig::iterator   endSig;
                   long   mX, mY, mZ, x, y, zz, xadjust, yadjust, yoffsetslice;
                   long   XRW1, YRW1, XRW2, YRW2;
                   bool   use_global;
                   long   northPad, southPad, eastPad, westPad;
                   long   xoffsettrack, yoffsettrack;
        MBK::chain_list  *pChain;
        MBK::locon_list  *pLocon;
        MBK::phcon_list  *pPhcon;
        MBK::losig_list  *pSig;
        MBK::phseg_list  *pSeg, flatSeg;
        MBK::phvia_list  *pVIA;
        MBK::phfig_list  *pModel;
            MBK::CXRect  *rect;
              MBK::CIns  *pIns;
                   MNet   routeds;
                   CNet  *pNet;
                 string   sig_name, term_name, ins_name;
      CDRGrid::iterator   coord;
                  CNode  *node;


  cmess1 << "\n";
  cmess1 << "  o  Loading design into grid...\n";

  ischip       = false;
  fig          = mbkfig;
  endInstances = fig->instances.end ();
  endOrphans   = fig->orphans.end   ();
  endSig       = fig->lofig.signals.end ();

  northPad     = 0;
  southPad     = 0;
  westPad      = 0;
  eastPad      = 0;
  xoffsettrack = 0;
  yoffsettrack = 0;

  // Half pitch offset.
  if ( halfpitch ) {
    xoffsettrack = D::X_GRID / 2;
    yoffsettrack = D::Y_GRID / 2;
  }

  // Search for pads.
  for (itIns = fig->instances.begin(); itIns != endInstances; itIns++) {
    pModel = itIns->second->getmodel ();

    if ( MBK::IsPxLib(pModel) ) {
      switch ( itIns->second->phins->TRANSF ) {
        case NOSYM:
          ischip = true;
          if ( northPad == 0 ) {
            cmess2 << "     o  North pad found.\n";
            northPad = pModel->YAB2 - pModel->YAB1 - MBK::SCALE(15);
          }
          break;
        case SYM_Y:
          ischip = true;
          if ( southPad == 0 ) {
            cmess2 << "     o  South pad found.\n";
            southPad = pModel->YAB2 - pModel->YAB1 - MBK::SCALE(15);
          }
          break;
        case ROT_P:
          ischip = true;
          if ( eastPad == 0 ) {
            cmess2 << "     o  East pad found.\n";
            eastPad = pModel->XAB2 - pModel->XAB1 - MBK::SCALE(15);
          }
          break;
        case SY_RP:
          ischip = true;
          if ( westPad == 0 ) {
            cmess2 << "     o  West pad found.\n";
            westPad = pModel->XAB2 - pModel->XAB1 - MBK::SCALE(15);
          }
          break;
        default:
          cerr << hwarn ("")
               << "  Pad " << itIns->second->phins->INSNAME
               << " have an invalid orientation.\n";
          break;
      }
    }
  }


  //southPad = northPad = westPad = eastPad = MBK::SCALE(50);
  //westPad = MBK::SCALE(100);


  // Default Routing Widow size : the AB.
  XRW1 = fig->XAB1 () + westPad;
  YRW1 = fig->YAB1 () + southPad;
  XRW2 = fig->XAB2 () - eastPad;
  YRW2 = fig->YAB2 () - northPad;

  // Find the a seed cell (one either from sxlib or dp_sxlib to
  // track adjust the grid).
  for (itIns = fig->instances.begin(); ; itIns++) {
    if (itIns == endInstances) {
      xadjust      = xoffsettrack;
      yadjust      = yoffsettrack;
      yoffsetslice = 0;
      cout << hwarn ("")
           << "  Unable to found a seed cell (i.e. belonging to either\n"
           << "  sxlib or dp_sxlib) grid could be misplaced.\n";
      break;
    }

    pModel = itIns->second->getmodel ();

    if ( !MBK::IsPxLib(pModel) ) {
      cmess2 << "     o  Using seed cell \"" << itIns->first
             << "\" (model \"" << pModel->NAME << "\").\n";
      xadjust      = abs((itIns->second->phins->XINS - XRW1) % D::X_GRID ) + xoffsettrack;
      yadjust      = abs((itIns->second->phins->YINS - YRW1) % D::Y_GRID ) + yoffsettrack;
      yoffsetslice = abs((itIns->second->phins->YINS - YRW1) % D::Y_SLICE) + YRW1;

      break;
    }
  }
  
  xoffsetgrid = XRW1 + xadjust;
  yoffsetgrid = YRW1 + yadjust;

  cmess2 << "     o  Grid offset : ("
         << MBK::UNSCALE(xoffsetgrid) << ","
         << MBK::UNSCALE(yoffsetgrid) << ")"
         << " [adjust ("
         << MBK::UNSCALE(xadjust) << ","
         << MBK::UNSCALE(yadjust) << ")]\n";

  // Compute the routing grid size.
  mX = (XRW2 - XRW1) / D::X_GRID + ((xadjust==0)?1:0);
  mY = (YRW2 - YRW1) / D::Y_GRID + ((yadjust==0)?1:0);
  mZ = z;


  // Is the design a complete chip.
  if (ischip)
    cmess2 << "     o  Design have pads, treated as a complete chip.\n";


  // Selecting the whether to use global routing.
  use_global = (mX + mY) > (2 * D::GLOBAL_HP);
  if (rtype == D::ROUTING_CHOOSE) {
    if (use_global) {
      cmess2 << "     o  Big design, global routing enabled.\n";
      rglobal = true;

      if (z < 5) {
        cmess2 << "        - Forcing 4 routing layers.\n";
        mZ = z = 5;
      }
    } else {
      cmess2 << "     o  Small design, global routing disabled.\n";
      rglobal = false;
    }
  } else {
    if (rtype == D::ROUTING_GLOBAL) {
      rglobal = true;
      if (!use_global) {
        cout << hwarn ("")
             << "  You have enabled global routing on a small design,\n"
             << "  this will waste upper routing layers.\n";
      }
    }
    if (rtype == D::ROUTING_LOCAL ) {
      rglobal = false;
      if (use_global) {
        cout << hwarn ("")
             << "  You have disabled global routing on a big design,\n"
             << "  this will slow down the routing.\n";
      }
    }
  }


  cmess2 << "     o  Allocating grid size ["
         << mX << "," << mY << "," << mZ << "].\n";

  // Allocating the routing grid.
  drgrid = new CDRGrid (xoffsetgrid, yoffsetgrid, mX, mY, mZ, zup);


  rect = new MBK::CXRect (drgrid);


  cmess2 << "     o  Loading external terminals.\n";

  // Browse layout for terminals.
  for (pPhcon = fig->phfig.fig->PHCON; pPhcon != NULL; pPhcon = pPhcon->NEXT) {
    if (fig->lofig.signals.find(pPhcon->NAME) == endSig) {
      cerr << hwarn ("")
           << "  The terminal \"" << pPhcon->NAME << "\" at ("
           << MBK::UNSCALE (pPhcon->XCON) << ","
           << MBK::UNSCALE (pPhcon->YCON) << ") layer "
           << MBK::layer2a (pPhcon->LAYER) << "\n"
           << "  do not not belong to any logical signal : ignored.\n";

      continue;
    }

    pNet = getnet (pPhcon->NAME);

    term_name  = "external.";
    term_name += pPhcon->NAME;

    flatSeg.X1    = pPhcon->XCON;
    flatSeg.Y1    = pPhcon->YCON;
    flatSeg.X2    = pPhcon->XCON;
    flatSeg.Y2    = pPhcon->YCON;
    flatSeg.WIDTH = MBK::env.layer2width (pPhcon->LAYER);
    flatSeg.LAYER = pPhcon->LAYER;
    flatSeg.NAME  = pPhcon->NAME;

    rect->setSeg  (flatSeg);

    if ( rect->isInGrid() ) {
      pNet->newaccess ( term_name
                      , rect->grid
                      , MBK::env.layer2z (pPhcon->LAYER)
                      );
    } else {
      cerr << hwarn ("")
           << "  The terminal \"" << pPhcon->NAME << "\" at ("
           << MBK::UNSCALE (pPhcon->XCON) << ","
           << MBK::UNSCALE (pPhcon->YCON) << ") layer "
           << MBK::layer2a (pPhcon->LAYER) << "\n"
           << "  is outside the routing grid : ignored.\n";
    }

  }


  cmess2 << "     o  Finding obstacles.\n";


  // Browse father for obstacles (powers are obstacles) and already
  // routed or partially routed signals.
  for (pSeg = fig->phfig.fig->PHSEG; pSeg != NULL; pSeg = pSeg->NEXT) {
    // There must not be obstacle in the father!
    if (MBK::isobs (pSeg->LAYER)) {
      cerr << hwarn ("")
           << "  An obstacle has been found at design top level, ignored.\n";

      continue;
    }

    // Power grid.
    if (MBK::ISVDD (pSeg->NAME) || MBK::ISVSS (pSeg->NAME)) {
      if (pSeg->LAYER != MBK::CALU1) {
        rect->setSeg (*pSeg);
        
        //cerr << "+ Top power obstacle" << endl;
        drgrid->nodes->obstacle (rect->grid, MBK::env.layer2z (pSeg->LAYER));
      }

      continue;
    }

    // Unnamed signals : ignored.
    if (pSeg->NAME == NULL) {
      cerr << hwarn ("")
           << "  An unnamed segment has been found at design top level, ignored.\n";

      continue;
    }

    // Partially routed signals.
    if (fig->lofig.signals.find(pSeg->NAME) == endSig) {
      cerr << hwarn ("")
           << "  The segment \"" << pSeg->NAME << "\" at ("
           << MBK::UNSCALE (pSeg->X1) << ","
           << MBK::UNSCALE (pSeg->Y1) << ") ("
           << MBK::UNSCALE (pSeg->X2) << ","
           << MBK::UNSCALE (pSeg->Y2) << ") layer "
           << MBK::layer2a (pSeg->LAYER) << "\n"
           << "  do not not belong to any logical signal : ignored.";

      continue;
    }

    pNet = getnet (pSeg->NAME);
    if ( !pNet->fixed ) {
      cmess2 << "     o  Signal " << pNet->name << " is assumed to be routed.\n";
      pNet->fixed = true;
    }

    rect->setSeg  (*pSeg);

    if ( rect->isInGrid() ) {
      //pNet->newaccess ( pSeg->NAME
      //                , rect->grid
      //                , MBK::env.layer2z (pSeg->LAYER)
      //                );
      drgrid->nodes->obstacle (rect->grid, MBK::env.layer2z (pSeg->LAYER));
    } else {
      cerr << hwarn ("")
           << "  The segment \"" << pSeg->NAME << "\" at ("
           << MBK::UNSCALE (pSeg->X1) << ","
           << MBK::UNSCALE (pSeg->Y1) << ") ("
           << MBK::UNSCALE (pSeg->X2) << ","
           << MBK::UNSCALE (pSeg->Y2) << ") layer "
           << MBK::layer2a (pSeg->LAYER) << "\n"
           << "  is outside the routing grid : ignored.";
    }
  }


  // Browse for obstacle VIAs.
  for (pVIA = fig->phfig.fig->PHVIA; pVIA != NULL; pVIA = pVIA->NEXT) {
    // Only power VIAs must be obstacles.
    if ( (! MBK::ISVDD (pVIA->NAME)) && (! MBK::ISVSS (pVIA->NAME))) {
      pNet = getnet (pVIA->NAME);
      if ( pNet && !pNet->fixed ) continue;
    }

    for (x = 0; x < 2; x++) {
      switch (x) {
        case 0: flatSeg.LAYER = MBK::topVIALayer    (pVIA->TYPE); break;
        case 1: flatSeg.LAYER = MBK::bottomVIALayer (pVIA->TYPE); break;
      }
    }

    if (flatSeg.LAYER == MBK::CALU1) continue;

    long  xVIAshrink = 0;
    if (pVIA->DX) { xVIAshrink = (pVIA->DX - MBK::SCALE(3)) / 2; }
    flatSeg.X1    = pVIA->XVIA - xVIAshrink;
    flatSeg.X2    = pVIA->XVIA + xVIAshrink;
    flatSeg.Y1    = pVIA->YVIA;
    flatSeg.Y2    = pVIA->YVIA;
    flatSeg.WIDTH = pVIA->DY;

    rect->setSeg (flatSeg);

    //cerr << "+ Top VIA obstacle (" << pVIA->XVIA << "," << pVIA->YVIA << ")" << endl;
    drgrid->nodes->obstacle (rect->grid, MBK::env.layer2z (flatSeg.LAYER));
  }


  // Browse instances & orphans for obstacles.
  for (itIns = fig->instances.begin(); ; itIns++) {
    if (itIns == endInstances) itIns = fig->orphans.begin ();
    if (itIns == endOrphans  ) break;

    pModel = itIns->second->getmodel ();

    cdebug << "+          - \"" << itIns->first
           << "\" (model \"" << pModel->NAME << "\").\n";


    // Find the obstacles in the current instance :
    // 1. - TALUx segments.
    // 2. - Power Terminals (vdd & vss), in CALUx layers & not CALU1.
    for (pSeg = pModel->PHSEG; pSeg != NULL; pSeg = pSeg->NEXT) {
      if (   MBK::isobs (pSeg->LAYER)
          || ((   MBK::ISVDD (pSeg->NAME)
               || MBK::ISVSS (pSeg->NAME)) && MBK::isCALU (pSeg->LAYER)
                                           && (pSeg->LAYER != MBK::CALU1))) {
        itIns->second->flatseg (flatSeg, *pSeg);
        rect->setSeg (flatSeg);

        //cerr << "+ Instance obstacle (" << flatSeg.X1 << "," << flatSeg.Y1 << ")" << endl;
        drgrid->nodes->obstacle (rect->grid, MBK::env.layer2z (pSeg->LAYER));

        if ( !MBK::ISVDD (pSeg->NAME) && !MBK::ISVSS (pSeg->NAME) )
          fig->addphseg ( flatSeg, true, ischip );
      }
    }
  }


  cmess2 << "     o  Loading nets into grid.\n";

  // Process the signals.
  for (pSig = fig->LOSIG (); pSig != NULL; pSig = pSig->NEXT) {
    sig_name = MBK::getsigname (pSig);

    cdebug << "+          - Signal \"" << sig_name << "\".\n";

    // Do not process power nets.
    // Temporary disabled.
    //if (   (MBK::ISVDD ((char*)sig_name.c_str ()) != 0)
    //    || (MBK::ISVSS ((char*)sig_name.c_str ()) != 0)) continue;

    // In the case of external terminals, override the signal name by
    // the terminal name.
    pChain = (MBK::chain_list*)(MBK::getptype (pSig->USER, LOFIGCHAIN)->DATA);
    for (; pChain != NULL; pChain = pChain->NEXT) {
      pLocon = (MBK::locon_list *)(pChain->DATA);

      if (pLocon->TYPE == EXTERNAL) {
        sig_name = pLocon->NAME;
        break;;
      }
    }

    pNet = getnet (sig_name);

    // Process each terminal of the signal.
    pChain = (MBK::chain_list*)(MBK::getptype (pSig->USER, LOFIGCHAIN)->DATA);

    for (; pChain != NULL; pChain = pChain->NEXT) {
      pLocon = (MBK::locon_list *)(pChain->DATA);

      if (pLocon->TYPE == EXTERNAL) {
        pNet->external = true;

        continue;
      }

      ins_name  = ((MBK::loins_list*)(pLocon->ROOT))->INSNAME;
      pIns      = fig->instances[ins_name];

      term_name = ins_name + "." + pLocon->NAME;

      // Find physical segments / CA of the terminal.
      for (pSeg = pIns->model->PHSEG; pSeg != NULL; pSeg = pSeg->NEXT) {
        if (!MBK::isCALU (pSeg->LAYER)) continue;
        if (pLocon->NAME != pSeg->NAME) continue;

        pIns->flatseg (flatSeg, *pSeg);
        rect->setSeg  (flatSeg);

        if ( MBK::IsPxLib(pModel) &&
             ( (pLocon->NAME == MBK::namealloc("pad" )) ||
               (pLocon->NAME == MBK::namealloc("vddi")) || 
               (pLocon->NAME == MBK::namealloc("vssi")) || 
               (pLocon->NAME == MBK::namealloc("vdde")) || 
               (pLocon->NAME == MBK::namealloc("vsse")) ) ) {
          flatSeg.NAME = (char*)sig_name.c_str();
          fig->addphseg (flatSeg,true,false);
          continue;
        }

        if (   (MBK::ISVDD ((char*)sig_name.c_str ()) != 0)
            || (MBK::ISVSS ((char*)sig_name.c_str ()) != 0)) continue;

        if (rect->isInGrid()) {
          if ( pNet->fixed ) {
            drgrid->nodes->obstacle (rect->grid, MBK::env.layer2z (pSeg->LAYER));
          } else {
            pNet->newaccess ( term_name
                            , rect->grid
                            , MBK::env.layer2z (pSeg->LAYER)
                            );
          }
        } else {
          //if ( MBK::IsPxLib(pModel) ) {
          //  flatSeg.NAME = (char*)sig_name.c_str();
          //  fig->addphseg (flatSeg,true,false);
          //} else {
            cerr << hwarn ("")
                 << "  The connector segment \"" << pSeg->NAME << "\" at ("
                 << MBK::UNSCALE (pSeg->X1) << ","
                 << MBK::UNSCALE (pSeg->Y1) << ") ("
                 << MBK::UNSCALE (pSeg->X2) << ","
                 << MBK::UNSCALE (pSeg->Y2) << ") layer "
                 << MBK::layer2a (pSeg->LAYER) << "\n"
                 << "  is outside of the grid : ignored.";
          }
        //}
      }
    } // End of "pChain" (terminal) loop.

    // Reorder terminals (nearest).
    pNet->order     ();
    pNet->lockalone (rglobal);
    
    cdebug << "+            " << pNet->bb << ".\n";
  } // End of "pSig" (signal) loop.


  // Allocate the net scheduler.
  cmess2 << "     o  Allocating the net scheduler.\n";
  netsched  = new CASimple (&nets, drgrid);


  // Rebuild the power grid from the instances.
  if ( !ischip ) {
    cmess2 << "     o  Reading power grid.\n";
    powers[MBK::CALU1] = new MBK::CPowers ( fig
                                          , xadjust - xoffsettrack
                                          , yoffsetslice
                                          , C_HORIZONTAL
                                          , MBK::ALU1
                                          , D::WIDTH_VDD
                                          );
  }


  // Forbid the use of the edges of the routing box (only allow
  // peripheral terminals).
  coord = drgrid->origin;

  // Left & Right vertical edges.
  if ( xadjust == 0 ) {
    for (x = 0; x < mX; x += mX - 1) {
      for (y = 0; y < mY; y++) {
        for (zz = 1; zz < mZ; zz++) {
          node = &( coord.set(x,y,zz).node() );
          
          if ( !node->terminal() ) node->data.obstacle = true;
        }
      }
    }
  }

  // Bottom & top horizontal edges.
  if ( xadjust == 0 ) {
    for (y = 0; y < mY; y += mY - 1) {
      for (x = 0; x < mX; x++) {
        for (zz = 1; zz < mZ; zz++) {
          node = &( coord.set(x,y,zz).node() );
          
          if ( !node->terminal() ) node->data.obstacle = true;
        }
      }
    }
  }

  // On routing level above zupper (ALU4), use only half of the tracks.
  for (zz = zup; zz < mZ; zz++) {
    switch ((zz+rotate) % 2) {
      case 0:
        // Vertical tracks.
        for (x = 2; x < mX; x += 2) {
          for (y = 1; y < mY - 1; y++) {
            node = &( coord.set(x,y,zz).node() );

            if ( !node->terminal() ) node->data.obstacle = true;
          }
        }
        break;
      case 1:
        // Horizontal tracks.
        for (y = 2; y < mY; y += 2) {
          for (x = 1; x < mX; x++) {
            node = &( coord.set(x,y,zz).node() );
            if ( !node->terminal() ) node->data.obstacle = true;
          }
        }
        break;
    }
  }


  // This flag ensure that a figure has been successfully loaded.
  loaded = true;


  delete rect;
}



// -------------------------------------------------------------------
// Modifier  :  "CRBox::mbksave()".

  void  CRBox::mbksave (string &name) throw (except_done)
{
                int  x, y, z, mX, mY, mZ, pitch, spaceVIA;
               bool  inseg;
              CRect  rect;
  CDRGrid::iterator  coord;
              CNode *node;
               CNet *pNextNet, *pNet, *pNetTop, *pNetBot;
    MBK::phseg_list  seg;
    MBK::phvia_list  via;
    MNet::iterator   itNet, endNet;


  if (!loaded) {
    cerr << hwarn ("")
         << "\n  The MBK figure has not been loaded into the grid yet, nothing"
         << "\n  to save at this point.\n";
      
    return;
  }

  if (insave) throw except_done ();

  insave = true;

  cmess2 << "\n";
  cmess1 << "  o  Dumping routing grid.\n";

  mX = drgrid->X;
  mY = drgrid->Y;
  mZ = drgrid->Z;
  coord = drgrid->origin;
  

  // Horizontal planes loop in both directions.
  for (z = 1; z < mZ; z++) {

    // Loop for horizontal segments.
    for (y = 0; y < mY; y++) {

      inseg = false;
      pNet  = NULL;

      for (x = 0; x < mX; x++) {
        node     = & ( coord.set(x,y,z).node() );
        pNextNet = node->data.owner;

        if (inseg && (pNextNet != pNet)) {
          // We are changing of segment owner.
          // Dump the current one.
          if (seg.X1 < seg.X2) {
            // This is not a "dot" segment (i.e a VIA).
            fig->addphseg (seg,pNet->external,ischip);
          } else if (z % 2) {
            char layer = seg.LAYER;
            seg.LAYER = MBK::layer2TALU (seg.LAYER);
            fig->addphseg (seg,false,ischip);
            seg.LAYER = layer;
          }

          // Force segment restarting.
          inseg = false;
        }

        pNet = pNextNet;

        if (pNet) {
          if (!inseg) {
            // We encounter the left edge of a segment.
            inseg = true;

            seg.X1    = xoffsetgrid + x * D::X_GRID;
            seg.Y1    = yoffsetgrid + y * D::Y_GRID;
            seg.Y2    = yoffsetgrid + y * D::Y_GRID;
            seg.NAME  = MBK::namealloc(pNet->name.c_str ());
            seg.WIDTH = MBK::env.z2width (z);

            if (pNet->external && !ischip) seg.LAYER = MBK::env.z2calu (z);
            else                           seg.LAYER = MBK::env.z2alu (z);
          }

          // Update the right edge.
          seg.X2 = xoffsetgrid + x * D::X_GRID;
        } else {
          if (inseg) {
            // Dump the current one.
            if (seg.X1 < seg.X2) {
              // This is not a "dot" segment (i.e a VIA).
              fig->addphseg (seg,pNet->external,ischip);
            } else if (z % 2) {
              char layer = seg.LAYER;
              seg.LAYER = MBK::layer2TALU (seg.LAYER);
              fig->addphseg (seg,false,ischip);
              seg.LAYER = layer;
            }
          }

          // We encounter the right edge of a segment.
          inseg = false;
        }
      } // End of "x" loop.

      if (inseg) {
        // This segment touch the AB.
        if (seg.X1 < seg.X2) {
          // This is not a "dot" segment (i.e a VIA).
          fig->addphseg (seg,pNet->external,ischip);
        } else if (z % 2 ) {
          char layer = seg.LAYER;
          seg.LAYER = MBK::layer2TALU (seg.LAYER);
          fig->addphseg (seg,false,ischip);
          seg.LAYER = layer;
        }
      }

    } // End of "y" loop (horizontal segments).

    // Loop for vertical segments.
    for (x = 0; x < mX; x++) {

      inseg = false;
      pNet  = NULL;

      for (y = 0; y < mY; y++) {
        node     = & ( coord.set(x,y,z).node() );
        pNextNet = node->data.owner;

        if (inseg && (pNextNet != pNet)) {
          // We are changing of segment owner.
          // Dump the current one.
          if (seg.Y1 < seg.Y2) {
            // This is not a "dot" segment (i.e a VIA).
            fig->addphseg (seg,pNet->external,ischip);
          } else if (! (z % 2)) {
            char layer = seg.LAYER;
            seg.LAYER = MBK::layer2TALU (seg.LAYER);
            fig->addphseg (seg,false,ischip);
            seg.LAYER = layer;
          }

          // Force segment restarting.
          inseg = false;
        }

        pNet = pNextNet;

        if (pNet) {
          if (!inseg) {
            // We encounter the left edge of a segment.
            inseg = true;

            seg.X1    = xoffsetgrid + x * D::X_GRID;
            seg.X2    = xoffsetgrid + x * D::X_GRID;
            seg.Y1    = yoffsetgrid + y * D::Y_GRID;
            seg.NAME  = MBK::namealloc(pNet->name.c_str ());
            seg.WIDTH = MBK::env.z2width (z);

            if (pNet->external && !ischip) seg.LAYER = MBK::env.z2calu (z);
            else                           seg.LAYER = MBK::env.z2alu (z);

          }

          // Update the right edge.
          seg.Y2 = yoffsetgrid + y * D::Y_GRID;
        } else {
          if (inseg) {
            // Dump the current one.
            if (seg.Y1 < seg.Y2) {
              // This is not a "dot" segment (i.e a VIA).
              fig->addphseg (seg,pNet->external,ischip);
            } else if (! (z % 2)) {
              char layer = seg.LAYER;
              seg.LAYER = MBK::layer2TALU (seg.LAYER);
              fig->addphseg (seg,false,ischip);
              seg.LAYER = layer;
            }
          }

          // We encounter the right edge of a segment.
          inseg = false;
        }
      } // End of "y" loop.

      if (inseg) {
        // This segment touch the AB.
        if (seg.Y1 < seg.Y2) {
          // This is not a "dot" segment (i.e a VIA).
          fig->addphseg (seg,pNet->external,ischip);
        } else if (! (z % 2)) {
          char layer = seg.LAYER;
          seg.LAYER = MBK::layer2TALU (seg.LAYER);
          fig->addphseg (seg,false,ischip);
          seg.LAYER = layer;
        }
      }

    } // End of "x" loop (vertical segments).

  } // End of "z" loop (planes).


  via.DX = 0;
  via.DY = 0;

  // Plane loop for VIAs.
  for (z = 1; z < mZ; z++) {
    // Track loop for VIAs.
    if (z >= drgrid->zupper) pitch = 2;
    else                     pitch = 1;

    if (z % 2) {
      // z=1 (ALU2) : horizontal tracks.
      for (y = 1; y < mY; y += pitch) {
        for (x = 0, spaceVIA = 2; x < mX; x++, spaceVIA++) {
          // Bottom node.
          pNetBot = NULL;
          node = & ( coord.set(x,y,z-1).node() );
          if ( !coord.isnodehole() && coord.inside() )
            pNetBot = node->data.owner;

          // Top node.
          pNetTop = NULL;
          node = & ( coord.set(x,y,z).node() );
          if ( !coord.isnodehole() && coord.inside() )
            pNetTop = node->data.owner;

          // Are the top & bottom nodes belonging to the same net ?
          if ( (pNetBot != NULL) && (pNetBot == pNetTop) ) {
            if (spaceVIA < pitch) continue;

            via.TYPE = MBK::env.z2via (z);
            via.XVIA = xoffsetgrid + x * D::X_GRID;
            via.YVIA = yoffsetgrid + y * D::Y_GRID;
            via.NAME = MBK::namealloc(pNetTop->name.c_str ());

            fig->addphvia (via);

            spaceVIA = 0;
          }
        }
      }
    } else {
      // z=2 (ALU3) : vertical tracks.
      for (x = 1; x < mX; x += pitch) {
        for (y = 0, spaceVIA = 2; y < mY; y++, spaceVIA++) {
          // Bottom node.
          pNetBot = NULL;
          node = & ( coord.set(x,y,z-1).node() );
          if ( !coord.isnodehole() && coord.inside() )
            pNetBot = node->data.owner;

          // Top node.
          pNetTop = NULL;
          node = & ( coord.set(x,y,z).node() );
          if ( !coord.isnodehole() && coord.inside() )
            pNetTop = node->data.owner;

          // Are the top & bottom nodes belonging to the same net ?
          if ( (pNetBot != NULL) && (pNetBot == pNetTop) ) {
            if (spaceVIA < pitch) continue;

            via.TYPE = MBK::env.z2via (z);
            via.XVIA = xoffsetgrid + x * D::X_GRID;
            via.YVIA = yoffsetgrid + y * D::Y_GRID;
            via.NAME = MBK::namealloc(pNetTop->name.c_str ());

            fig->addphvia (via);

            spaceVIA = 0;
          }
        }
      }
    }
  }

# if 0
  // Plane loop for VIAs.
  for (x = 0; x < mX; x++) {
    for (y = 0; y < mY; y++) {
      // Loop on Z axis.
      for (z = 1; z < mZ; z++) {
        if ( (z >= drgrid->zupper) && ( (x % 2) || (y % 2) ) )
          break;

        node = & ( coord.set(x,y,z).node() );
        if (coord.inside()) pNet = node->data.owner;
        else                pNet = NULL;

        node = & ( coord.set(x,y,z-1).node() );
        if (coord.inside()) pNextNet = node->data.owner;

        if (pNet && (pNet == pNextNet)) {
          via.TYPE = MBK::env.z2via (z);
          via.XVIA = fig->XAB1() + x * D::X_GRID;
          via.YVIA = fig->YAB1() + y * D::Y_GRID;
          via.NAME = MBK::namealloc(pNet->name.c_str ());

          fig->addphvia (via);
        }
      } // End of Z axis loop.
    }
  } // End of plane loop for VIAs.
# endif


  // Special case of nets with only one terminal.
  endNet = nets.end ();
  for (itNet = nets.begin(); itNet != endNet; itNet++) {
    if (!itNet->second->external) continue;
    if (itNet->second->size != 1) continue;
    if (itNet->second->terms[0]->rects.size() < 1) continue;

    rect = itNet->second->terms[0]->rects.front ();

    seg.X1    = xoffsetgrid + rect.x1 * D::X_GRID;
    seg.X2    = xoffsetgrid + rect.x2 * D::X_GRID;
    seg.Y1    = yoffsetgrid + rect.y1 * D::Y_GRID;
    seg.Y2    = yoffsetgrid + rect.y2 * D::Y_GRID;
    seg.NAME  = MBK::namealloc(itNet->second->name.c_str ());
    seg.WIDTH = MBK::env.z2width (0);
    seg.LAYER = MBK::env.z2calu (0);

    fig->addphseg (seg,itNet->second->external,ischip);
  }


  // Add powers lines.
  if ( !ischip ) powers[MBK::CALU1]->dump (fig);
  

  cmess1 << "  o  Saving MBK figure \"" << name << "\".\n";
  fig->phfig.saveas (name);
}
