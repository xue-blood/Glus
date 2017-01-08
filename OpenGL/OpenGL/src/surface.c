#include "..\inc\glus.h"


/*
 *	build face from u v
 */
// create [9/20/2016 blue]
bool 
glusSurfaceBuildFace(
_In_	PGlusMesh	_mesh,
_In_	Glusnum		_n_piece,
_In_	Glusnum		_n_stack)
{
	assertp(_mesh);
	assert(_n_stack > 0 && _n_piece > 0);

	glusAllocex(_mesh->Faces, GlusFace, _n_piece*_n_stack, return false);
#define F			(_mesh->Faces[_mesh->FaceNum])
#define FP(n)		F.FaceIDs[n].PointID
#define FN(n)		F.FaceIDs[n].NormalID
	for (Glusnum v = 0; v < _n_stack; v++)
	{
		for (Glusnum u = 0; u < _n_piece; u++, _mesh->FaceNum++)
		{
			// allocate the face-index
			// we all use quad-face
			F.FaceIDNum = 4; glusAllocex(F.FaceIDs, GlusFaceIndex, 4, return false);

			FP(0) = v*(_n_piece + 1) + u;
			FP(1) = FP(0) + (_n_piece + 1);
			FP(2) = FP(1) + 1;
			FP(3) = FP(0) + 1;

			FN(0) = _mesh->FaceNum;
			FN(1) = _mesh->FaceNum;
			FN(2) = _mesh->FaceNum;
			FN(3) = _mesh->FaceNum;
		}
	}
#undef FN
#undef FP
#undef F

	return true;
}

/*
 *	compute point from u,v for sphere
 *	u and v  are both use rad
 */
// create [9/20/2016 blue]
void
glusSurfaceSphere(
_In_	GLdouble	_u,
_In_	GLdouble	_v,
_Out_	PGlusVector	_o)
{
	assertp(_o);

	_o->X = cos(_v)*cos(_u);
	_o->Y = cos(_v)*sin(_u);
	_o->Z = sin(_v);
	_o->V = 1;

	glusVNormalize(_o);
}

/*
 *	compute the ruled surface point from u v
 */
// create [9/20/2016 blue]
void
glusSurfaceBilinear(
_In_	GLdouble	_u,
_In_	GLdouble	_v,
_In_	void(*_f_a)(GLdouble u, PGlusVector o),
_In_	void(*_f_b)(GLdouble u, PGlusVector o),
_Out_	PGlusVector	_o)
{
	assertp(_o);

	GlusVector	a, b;
	_f_a(_u, &a);
	_f_b(_u, &b);

	glusVAdd(&a, 1 - _v, &b, _v, _o);
}