#include "PCon.h"
#include "mut.h"
#include "mph.h"
#include "PConstants.h"

PCon::PCon(const locon* con):
		PElem(), _con(con), _pos()
{}

PCon::PCon(const locon* con, PPos pos, char orient):
		PElem(), _con(con), _pos(pos), _orient(orient)
{}

void
PCon::Save(struct phfig *physicalfig, const double dx, const double dy) const
{

    addphcon(physicalfig,
			_orient,
			_con->NAME,
			(int)(GetPosX() * PITCH + dx),
			(int)(GetPosY() * PITCH + dy),
			_orient==NORTH || _orient == SOUTH ? ALU2 : ALU3,
			(_orient==NORTH || _orient == SOUTH ? 2 : 1) * (PITCH/5));
}

void
PCon::RingSave(struct phfig *physicalfig, const double dx, const double dy) const
{
    addphcon(physicalfig,
			_orient,
			_con->NAME,
			(int)(GetPosX() * PITCH + dx),
			(int)(GetPosY() * PITCH + dy),
			ALU2,
			2 * (PITCH/5));
}

ostream&
PCon::Print(ostream& os) const
{
    return os << _con->NAME <<  GetPos();
}
