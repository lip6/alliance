#ifndef __PMove_h
#define __PMove_h

#include <PPlacement.h>
#include <PPos.h>
#include <map>

class PMove {
  private:
      
    PPlacement&			Placement;

    PToPlaceIns*		SrcIns;
    PBin*			SrcBin;
    double			SrcBinInitCost;
    PSubRow*			_srcSubRow;
    PRow*			_srcRow;
    double			SrcRowInitCost;
    double			SrcWidth;

    PBin*			DstBin;
    double			DstBinInitCost;
    PSubRow*			_dstSubRow;
    PRow*			_dstRow;
    double			DstRowInitCost;
    PToPlaceIns*		DstIns;
    double			DstWidth;

    map<PONet*, unsigned>	AffectedNets;

  public:
			PMove(PPlacement& placement);

    double		GetDeltaRowCost();
    double		GetDeltaBinCost();
    double		GetDeltaNetCost();

    bool		Next(const double dist);
    void		Accept();
    void		Reject();
    void		Move();
};

#endif /* __PMove_h */
