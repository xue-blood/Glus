#include "..\inc\glus.h"


void	
glusMeshDraw(
_In_	PGlusMesh	_mesh)
{
	assertp(_mesh);

	for (Glusnum i = 0; i < _mesh->FaceNum;i++)	// draw each face
	{
		glusDebug("\nface:\t%d\n", i);

		glEnable(GL_LIGHTING);

		/*
		*	is require texture
		*/
		if (_mesh->Textures)
		{
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, _mesh->TextureID);
		}
		else	glBindTexture(GL_TEXTURE_2D, -1); // use unvalid texture

		if (glusGetShadeLevel()== Glus_Shade_Wire)	glBegin(GL_LINE_LOOP);
		else										glBegin(GL_POLYGON);
		/*
		 *	send coordinate to pipeline
		 */
		for (Glusnum j = 0; j < _mesh->Faces[i].FaceIDNum;j++) // draw each one
		{
			Glusindex	id_normal = _mesh->Faces[i].FaceIDs[j].NormalID;
			Glusindex	id_point =  _mesh->Faces[i].FaceIDs[j].PointID;

			/*
			 *	is back face
			 */
			if ((_mesh->NormalNum > 0) && glusIsFaceBack(glusGetEye(), _mesh->Points + id_point, _mesh->Normals + id_normal))
				goto _mesh_end;

			if (_mesh->Textures)
				glTexCoord2d(_mesh->Textures[id_point].X, _mesh->Textures[id_point].Y);
			if (_mesh->NormalNum > 0)
				glNormal3dv((pdouble)(_mesh->Normals+id_normal));	// normal
			glVertex3dv((pdouble)(_mesh->Points+id_point));		// point	// change [9/1/2016 blue] : add a (), and now work fine
			
			//glusDebug("point,normal:\t%d,%d\n", id_point, id_normal);
			glusDebug("point:\t%d\t", id_point);
			glusDebug("normal:\t%d\n", id_normal);
		}

_mesh_end:
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
}

//
// load mesh from file, use array
//
// add [8/31/2016 blue]
// change return type [8/31/2016 blue] : 
Glus_Status
glusMeshLoad(
_In_	FILE *		_file,
_Inout_	PGlusMesh	*_mesh)
{
	assertp(_file);

	*_mesh = NULL;
	PGlusMesh	p_mesh;		glusAlloc(p_mesh,GlusMesh);
	*_mesh = p_mesh;

	/*
	 *	first: get the point,normal and face number
	 */
	glusFileScanf(_file, "%d", &p_mesh->PointNum);
	glusFileScanf(_file, "%d", &p_mesh->NormalNum);
	glusFileScanf(_file, "%d", &p_mesh->FaceNum);

	/*
	 *	then read the points
	 */
	glusAllocN(p_mesh->Points, GlusVector, p_mesh->PointNum);
	glusFileLoadPoints_A(_file, p_mesh->Points, p_mesh->PointNum);

	/*
	 *	read the normals
	 */
	if (p_mesh->NormalNum > 0)
	{
		glusAllocN(p_mesh->Normals, GlusVector, p_mesh->NormalNum);
		glusFileLoadVectors_A(_file, p_mesh->Normals, p_mesh->NormalNum);
	}

	/*
	 *	read the faces
	 */
	glusAllocN(p_mesh->Faces, GlusFace, p_mesh->FaceNum); 
	for (Glusnum i = 0; i < p_mesh->FaceNum; i++) // each face
	{
		/*
		 *	read the number of id for current face
		 */
		glusFileScanf(_file, "%d", &p_mesh->Faces[i].FaceIDNum);

		glusAllocN(p_mesh->Faces[i].FaceIDs, GlusFaceIndex, p_mesh->Faces[i].FaceIDNum);
		

		// for point
		for (Glusnum j = 0; j < p_mesh->Faces[i].FaceIDNum; j++)	
			glusFileScanf(_file, "%d", &p_mesh->Faces[i].FaceIDs[j].PointID);

		// for normal
		if (p_mesh->NormalNum > 0)
		{
			for (Glusnum j = 0; j < p_mesh->Faces[i].FaceIDNum; j++)
				glusFileScanf(_file, "%d", &p_mesh->Faces[i].FaceIDs[j].NormalID);
		}
	}
	glusCheck(p_mesh);
	return Glus_Status_Success;
}

/*
 *	clear memory for mesh
 */
// add [8/31/2016 blue]
void
glusMeshClear(
_In_	PGlusMesh	_mesh)
{
	assertp(_mesh);

	glusDebug(__FUNCTION__"\n");

	// free texture-id
	glusFree(_mesh->Textures);

	// free point
	glusFree(_mesh->Points);	_mesh->PointNum = 0;

	// free normal
	glusFree(_mesh->Normals);	_mesh->NormalNum = 0;

	/*
	 *	free face
	 */
	for (Glusindex i = 0;	i < _mesh->FaceNum;		i++)
	{
		glusFree(_mesh->Faces[i].FaceIDs);	// free index
	}
	glusFree(_mesh->Faces);		_mesh->FaceNum = 0;
}

//
// add a mesh to scene
//
// add [8/30/2016 tld]
Glus_Status
glusMeshAddToScene(
_In_	PGlusMesh	_mesh,
_Inout_	PGlusScene	_scene)
{
	assertp(_mesh && _scene);

	/*
	 *	create a new default shape
	 */
	PGlusShape	p_shape = glusSceneCreateNewShape(_scene);

	// set the draw function
	p_shape->Draw = glusMeshDraw;
	
	p_shape->Clear = glusMeshClear;	// clear function

	// set the data
	p_shape->Extern = _mesh;

	return Glus_Status_Success;
}

/*
 *	compute the normal for each the face
 *	all vertex of face use the same normal
 */
// create [9/19/2016 blue]
void
glusMeshFaceNormal(
_Inout_		PGlusMesh	_mesh)
{
	glusLink(h_point);
#define POINT(id_face,id_index) _mesh->Points[_mesh->Faces[id_face].FaceIDs[id_index].PointID]
	/*
	*	compute the normals
	*/
	for (Glusindex f = 0; f < _mesh->FaceNum; f++)
	{
		/*
		*	copy point to a link list
		*/
		for (Glusindex i = 0; i < _mesh->Faces[f].FaceIDNum; i++)
		{
			PGlusPoints p;
			glusAllocex(p, GlusPoints, 1, goto _free_);

			p->Point = POINT(f, i);

			glusLinkInsertTail(&h_point, p);
		}

		// compute the normal
		glusPointsNormal_L(&h_point, (_mesh->Normals + f));

	_free_:
		glusLinkClear(&h_point); // we should free the current link list for next face
	}
#undef POINT
}
/*
*	use some points from link list to build prism
*/
// create [9/6/2016 blue]
// looks ok [9/12/2016 blue]
PGlusMesh
glusMeshMakePrism(
_In_	PGlusLink	_point,
_In_	PGlusVector	_vector)
{
	assertp(_point);

	Glusnum n = glusLinkLength(_point);	// get the points num

	// create a new mesh
	PGlusMesh	p_prism;
	glusAllocex(p_prism, GlusMesh, 1, return NULL);

	/*
	*	allocate memory for the points
	*/
	p_prism->PointNum = 2 * n;	// doubly number of point
	glusAllocex(p_prism->Points, GlusVector, p_prism->PointNum, goto _make_prism_failed);

	/*
	*	copy the points
	*/
	glusPointsConvert_L_A(_point, p_prism->Points, n);	// the bottom
	glusPointsExtrude(p_prism->Points, p_prism->Points + n, n, _vector);	// the cap


	/*
	 *	write the faces point and normal id 
	 */
	p_prism->FaceNum = 2 +n; // bottom, cap ,and the n rect
	glusAllocex(p_prism->Faces, GlusFace, p_prism->FaceNum, goto _make_prism_failed);

	/*
	 *	the bottom (0) and the cap (1)
	 */
	p_prism->Faces[0].FaceIDNum =
		p_prism->Faces[1].FaceIDNum = n;

	glusAllocex(p_prism->Faces[0].FaceIDs, GlusFaceIndex, n, goto _make_prism_failed); // bottom
	glusAllocex(p_prism->Faces[1].FaceIDs, GlusFaceIndex, n, goto _make_prism_failed); // cap
	
	for (Glusindex i = 0; i < n; i++)
	{
		// bottom
		p_prism->Faces[0].FaceIDs[i].PointID = n-i-1;
		p_prism->Faces[0].FaceIDs[i].NormalID = 0;	// use the same normal

		// cap
		p_prism->Faces[1].FaceIDs[i].PointID = i + n; 
		p_prism->Faces[1].FaceIDs[i].NormalID = 1;	// 
	}

	/*
	 *	the other faces
	 */
#define N(i) (((i)+1)%n)
	for (Glusindex f = 2; f < 2 + n; f++) //  face
	{
		p_prism->Faces[f].FaceIDNum = 4;	// each has 4 points
		
		// allocate the memory
		glusAllocex(p_prism->Faces[f].FaceIDs, GlusFaceIndex, 4, goto _make_prism_failed); // cap


		p_prism->Faces[f].FaceIDs[0].PointID = f - 2;
		p_prism->Faces[f].FaceIDs[1].PointID = N(f - 2);
		p_prism->Faces[f].FaceIDs[2].PointID = N(f - 2)+n;
		p_prism->Faces[f].FaceIDs[3].PointID = f - 2 + n;

		/*
		 *	we use the same normal
		 */
		for (Glusindex i = 0; i < 4; i++)
			p_prism->Faces[f].FaceIDs[i].NormalID = f;
	}
#undef N


	/*
	 *	now we can compute the normal
	 */
	p_prism->NormalNum = 2 + n;
	glusAllocex(p_prism->Normals, GlusVector, p_prism->NormalNum, goto _make_prism_failed);
	
	glusMeshFaceNormal(p_prism);
	
	// return the mesh
	// so it should free later
	return p_prism;

_make_prism_failed:
	
	glusMeshClear(p_prism);

	return NULL;
}



/*
 *	extrude quad-strip
 */
// create [9/19/2016 blue]
PGlusMesh
glusMeshExtrudeQuadStrip(
_In_	PGlusLink	_quad_strip,
_In_	PGlusVector	_vector)
{
	assertp(_quad_strip);

	Glusnum n = glusLinkLength(_quad_strip);	// get the points num

	// create a new mesh
	PGlusMesh	p_prism;
	glusAllocex(p_prism, GlusMesh, 1, return NULL);

	/*
	*	allocate memory for the points
	*/
	p_prism->PointNum = 2 * n;	// doubly number of point
	glusAllocex(p_prism->Points, GlusVector, p_prism->PointNum, goto _make_prism_failed);

	/*
	*	copy the points
	*/
	glusPointsConvert_L_A(_quad_strip, p_prism->Points, n); // origin
	glusPointsExtrude(p_prism->Points, p_prism->Points + n, n, _vector); // the extruded
	
	/*
	 *	build the face list 
	 */
	// allocate memory for face list
	glusAllocex(p_prism->Faces, GlusFace, 4 * (n / 2 - 1) + n / 2, goto _make_prism_failed);

	// we use a macro for current face 
#define F	(p_prism->Faces[p_prism->FaceNum])
	/*
	 *	for bottom
	 */
	for (Glusindex i = 0; i < n-2; i += 2, p_prism->FaceNum++)
	{
		// allocate memory for face-index
		F.FaceIDNum = 4;
		glusAllocex(F.FaceIDs, GlusFaceIndex, 4, goto _make_prism_failed);

		/*
		*	each face
		*/
		F.FaceIDs[0].PointID = i;
		F.FaceIDs[1].PointID = i + 1;
		F.FaceIDs[2].PointID = i + 3;
		F.FaceIDs[3].PointID = i + 2;

		/*
		*	we use the same normal as the current face-id
		*/
		F.FaceIDs[0].NormalID = p_prism->FaceNum;
		F.FaceIDs[1].NormalID = p_prism->FaceNum;
		F.FaceIDs[2].NormalID = p_prism->FaceNum;
		F.FaceIDs[3].NormalID = p_prism->FaceNum;
	}

	/*
	*	for top
	*/
	for (Glusindex i = 0; i < n - 2; i += 2, p_prism->FaceNum++)
	{
		// allocate memory for face-index
		F.FaceIDNum = 4;
		glusAllocex(F.FaceIDs, GlusFaceIndex, 4, goto _make_prism_failed);

		/*
		*	each face
		*/
		F.FaceIDs[0].PointID = i+n;
		F.FaceIDs[1].PointID = i + 1+n;
		F.FaceIDs[2].PointID = i + 3+n;
		F.FaceIDs[3].PointID = i + 2+n;

		/*
		*	we use the same normal as the current face-id
		*/
		F.FaceIDs[0].NormalID = p_prism->FaceNum;
		F.FaceIDs[1].NormalID = p_prism->FaceNum;
		F.FaceIDs[2].NormalID = p_prism->FaceNum;
		F.FaceIDs[3].NormalID = p_prism->FaceNum;
	}

	/*
	*	for front
	*/
	for (Glusindex i = 0; i < n - 2; i += 2, p_prism->FaceNum++)
	{
		// allocate memory for face-index
		F.FaceIDNum = 4;
		glusAllocex(F.FaceIDs, GlusFaceIndex, 4, goto _make_prism_failed);

		/*
		*	each face
		*/
		F.FaceIDs[0].PointID = i;
		F.FaceIDs[1].PointID = i + 2;
		F.FaceIDs[2].PointID = i + 2 + n;
		F.FaceIDs[3].PointID = i + n;

		/*
		*	we use the same normal as the current face-id
		*/
		F.FaceIDs[0].NormalID = p_prism->FaceNum;
		F.FaceIDs[1].NormalID = p_prism->FaceNum;
		F.FaceIDs[2].NormalID = p_prism->FaceNum;
		F.FaceIDs[3].NormalID = p_prism->FaceNum;
	}
	/*
	*	for back
	*/
	for (Glusindex i = 0; i < n - 2; i += 2, p_prism->FaceNum++)
	{
		// allocate memory for face-index
		F.FaceIDNum = 4;
		glusAllocex(F.FaceIDs, GlusFaceIndex, 4, goto _make_prism_failed);

		/*
		*	each face
		*/
		F.FaceIDs[0].PointID = i + 1;
		F.FaceIDs[1].PointID = i + 1 + 8;
		F.FaceIDs[2].PointID = i + 3 + 8;
		F.FaceIDs[3].PointID = i + 3;

		/*
		*	we use the same normal as the current face-id
		*/
		F.FaceIDs[0].NormalID = p_prism->FaceNum;
		F.FaceIDs[1].NormalID = p_prism->FaceNum;
		F.FaceIDs[2].NormalID = p_prism->FaceNum;
		F.FaceIDs[3].NormalID = p_prism->FaceNum;
	}
	/*
	 *	for end wall 
	 */
	for (Glusindex i = 0; i < n; i+=2, p_prism->FaceNum++)
	{
		// allocate memory for face-index
		F.FaceIDNum = 4;
		glusAllocex(F.FaceIDs, GlusFaceIndex, 4, goto _make_prism_failed);

		/*
		 *	each face
		 */
		F.FaceIDs[0].PointID = i;
		F.FaceIDs[1].PointID = i + n;
		F.FaceIDs[2].PointID = i + 1 + n;
		F.FaceIDs[3].PointID = i + 1;
		
		/*
		 *	we use the same normal as the current face-id
		 */
		F.FaceIDs[0].NormalID = p_prism->FaceNum;
		F.FaceIDs[1].NormalID = p_prism->FaceNum;
		F.FaceIDs[2].NormalID = p_prism->FaceNum;
		F.FaceIDs[3].NormalID = p_prism->FaceNum;
	}
#undef F


	/*
	 *	so we can build the normal list
	 */
	// the normals' number equal the faces' number
	p_prism->NormalNum = p_prism->FaceNum;
	glusAllocex(p_prism->Normals, GlusVector, p_prism->NormalNum, goto _make_prism_failed);

	glusMeshFaceNormal(p_prism);

	// return the mesh
	// so it should free later
	return p_prism;

_make_prism_failed:

	glusMeshClear(p_prism);

	return NULL;
}


/*
 *	circularly sweeping a profile
 */
// create [9/19/2016 blue]
PGlusMesh
glusMeshRevolution(
_In_	PGlusLink	_points,
_In_	Glusnum		_n_piece,
_In_	GLdouble	_ang_start,
_In_	GLdouble	_ang_sweep)
{
	assertp(_points);
	assert(_n_piece > 0);

	if (_ang_sweep == 0) _ang_sweep = 360;

	Glusnum		n = glusLinkLength(_points);

	PGlusMesh	mesh;
	glusAllocex(mesh, GlusMesh, 1, goto _make_mesh_failed);


	/*
	 *	build points
	 */
	mesh->PointNum = (_n_piece + 1)*n;	// !!! a piece of points will be the same if it's a circularly sweep
	glusAllocex(mesh->Points, GlusVector, mesh->PointNum, goto _make_mesh_failed);
// 	glusPointsConvert_L_A(_points, mesh->Points, n);

	GLdouble	ang_del = _ang_sweep / _n_piece;
	GLdouble	ang_sin, ang_cos;

	/*
	*	each piece
	*/
	for (Glusindex piece = 0; piece <= _n_piece;
		piece++,_ang_start+=ang_del)
	{
		ang_sin = sina(_ang_start);
		ang_cos = cosa(_ang_start);

		/*
		 *	each face
		 */
		PGlusPoints p = (PGlusPoints)_points->BLink;
		for (Glusindex i = 0; i < n; i++, p = (PGlusPoints)p->Link.BLink)
		{
			glusP(p->Point.X*ang_cos, p->Point.Y, p->Point.X*ang_sin,
				mesh->Points + i + piece*n);
		}
	}

	/*
	 *	build face list 
	 */
	glusAllocex(mesh->Faces, GlusFace, _n_piece*n, goto _make_mesh_failed);
#define F	(mesh->Faces[mesh->FaceNum])
	for (Glusindex piece = 0; piece < _n_piece;piece++) // each piece
	{
		for (Glusindex i = 0; i < n-1;i++,mesh->FaceNum++) // each face
		{
			F.FaceIDNum = 4;
			glusAllocex(F.FaceIDs, GlusFaceIndex, 4, goto _make_mesh_failed);

			/*
			*	point id
			*/
			F.FaceIDs[0].PointID = i+piece*n;
			F.FaceIDs[1].PointID = i + n + piece*n;
			F.FaceIDs[2].PointID = i + 1 + n + piece*n;
			F.FaceIDs[3].PointID = i + 1 + piece*n;

			/*
			*	we use the same normal as the current face-id
			*/
			F.FaceIDs[0].NormalID = mesh->FaceNum;
			F.FaceIDs[1].NormalID = mesh->FaceNum;
			F.FaceIDs[2].NormalID = mesh->FaceNum;
			F.FaceIDs[3].NormalID = mesh->FaceNum;
		}
	}
#undef F

	/*
	*	so we can build the normal list
	*/
	// the normals' number equal the faces' number
	mesh->NormalNum = mesh->FaceNum;
	glusAllocex(mesh->Normals, GlusVector, mesh->NormalNum, goto _make_mesh_failed);

	glusMeshFaceNormal(mesh);


	// return the mesh
	// so it should free later
	return mesh;

_make_mesh_failed:

	glusMeshClear(mesh);

	return NULL;
}

/*
 *	draw function for meshs store in link list
 */
// create [9/20/2016 blue]
void
glusMeshsDraw(
_In_	PGlusLink	_head)
{
	assertp(_head);

	PGlusMeshs	e_meshs = (PGlusMeshs)_head->BLink;
	while (!glusLinkIsHead(e_meshs,_head))
	{
		glusMeshDraw(e_meshs->Mesh);

		e_meshs = (PGlusMeshs)e_meshs->Link.BLink;
	}
}

/*
 *	free memory for meshs' link list
 */
// create [9/20/2016 blue]
void
glusMeshsClear(
_In_	PGlusLink	_head)
{
	assertp(_head);
	
	PGlusMeshs	e_meshs = (PGlusMeshs)_head->BLink;
	while (!glusLinkIsHead(e_meshs, _head))
	{
		glusMeshClear(e_meshs->Mesh);	// clear mesh

		e_meshs = (PGlusMeshs)e_meshs->Link.BLink;
	}

	/*
	 *	clear link list
	 */
	glusLinkClear(_head);

}
/*
*	make prism from some polygon store in link list
*/
// create [9/19/2016 blue]
PGlusLink
glusMakePrismArray(
_In_	PGlusLink	_polygons,
_In_	PGlusVector	_vector)
{
	assertp(_polygons && _vector);

	PGlusLinks	p = (PGlusLinks)_polygons->BLink;
	PGlusLink	h_meshs; glusAllocex(h_meshs, GlusLink, 1, return NULL); glusLinkInit(h_meshs);	// create a new link head

	while (!glusLinkIsHead(p, _polygons))
	{
		PGlusMeshs	entry; glusAllocex(entry, GlusMeshs, 1, break);	// allocate a new entry

		entry->Mesh = glusMeshMakePrism(&p->Data, _vector); // p->Data is pointer to a polygon

		// add to meshs'list
		glusLinkInsertTail(h_meshs, entry);

		p = (PGlusLinks)p->Link.BLink;
	}

	return h_meshs;
}


/*
 *	make surface mesh from parameter formal
 */
// create [9/20/2016 blue]
// look ok,but the memory heavy at pole [9/20/2016 blue]
PGlusMesh
glusMeshSurface(
_In_	Glusnum		_n_piece,	// for u
_In_	GLdouble	_u_start,
_In_	GLdouble	_u_sweep,
_In_	Glusnum		_n_stack,	// for v
_In_	GLdouble	_v_start,
_In_	GLdouble	_v_sweep,
_In_    void(*_f_point)(GLdouble u, GLdouble v,PGlusVector o))	// for the point
{
	assertp(_f_point);
	assert(_n_piece > 0 && _n_stack > 0);

	PGlusMesh	p_mesh;		glusAllocex(p_mesh, GlusMesh, 1, goto _make_mesh_failed);

	/*
	 *	build points
	 *	we will not be care of the point at pole !!!!!!!!
	 */
	p_mesh->PointNum = (_n_piece + 1)*(_n_stack + 1);
	glusAllocex(p_mesh->Points, GlusVector, p_mesh->PointNum, goto _make_mesh_failed);
	
	GLdouble	u_del = _u_sweep / _n_piece,
				v_del = _v_sweep / _n_stack;
	GLdouble v_ang = 0, u_ang = 0;

	for (Glusnum v = 0; v <= _n_stack; v++, v_ang += v_del)
	{
		GLdouble u_ang = 0;
		for (Glusnum u = 0; u <= _n_piece; u++, u_ang += u_del)
		{
			// use the extern function to compute the point 
			_f_point(_u_start+u_ang, _v_start+v_ang, p_mesh->Points + v*(_n_piece + 1) + u); 
		}
	}

	/*
	 *	build face list
	 */
	if(glusSurfaceBuildFace(p_mesh, _n_piece, _n_stack))
		goto _make_mesh_failed;

	/*
	*	so we can build the normal list
	*/
	// the normals' number equal the faces' number
	p_mesh->NormalNum = p_mesh->FaceNum;
	glusAllocex(p_mesh->Normals, GlusVector, p_mesh->NormalNum, goto _make_mesh_failed);

	glusMeshFaceNormal(p_mesh);


	// return the p_mesh
	// so it should free later
	return p_mesh;

_make_mesh_failed:

	glusMeshClear(p_mesh);

	return NULL;
}

/*
*	make ruled surface mesh
*/
// create [9/20/2016 blue]
PGlusMesh
glusMeshSurfaceBilinear(
_In_	Glusnum		_n_piece,	// for u
_In_	GLdouble	_u_start,
_In_	GLdouble	_u_sweep,
_In_	Glusnum		_n_stack,	// for v
_In_	GLdouble	_v_start,
_In_	GLdouble	_v_sweep,
_In_	void(*_f_a)(GLdouble u, PGlusVector o),
_In_	void(*_f_b)(GLdouble u, PGlusVector o))	// for the point
{
	assertp(_f_a && _f_b);
	assert(_n_piece > 0 && _n_stack > 0);

	PGlusMesh	p_mesh;		glusAllocex(p_mesh, GlusMesh, 1, goto _make_mesh_failed);

	/*
	*	build points
	*	we will not be care of the point at pole !!!!!!!!
	*/
	p_mesh->PointNum = (_n_piece + 1)*(_n_stack + 1);
	glusAllocex(p_mesh->Points, GlusVector, p_mesh->PointNum, goto _make_mesh_failed);

	GLdouble	u_del = _u_sweep / _n_piece,
		v_del = _v_sweep / _n_stack;
	GLdouble v_ang = 0, u_ang = 0;

	for (Glusnum v = 0; v <= _n_stack; v++, v_ang += v_del)
	{
		GLdouble u_ang = 0;
		for (Glusnum u = 0; u <= _n_piece; u++, u_ang += u_del)
		{
			// use the extern function to compute the point 
			glusSurfaceBilinear(_u_start + u_ang, _v_start + v_ang,
				_f_a, _f_b,
				p_mesh->Points + v*(_n_piece + 1) + u);
		}
	}

	/*
	*	build face list
	*/
	if (glusSurfaceBuildFace(p_mesh, _n_piece, _n_stack))
		goto _make_mesh_failed;

	/*
	*	so we can build the normal list
	*/
	// the normals' number equal the faces' number
	p_mesh->NormalNum = p_mesh->FaceNum;
	glusAllocex(p_mesh->Normals, GlusVector, p_mesh->NormalNum, goto _make_mesh_failed);

	glusMeshFaceNormal(p_mesh);


	// return the p_mesh
	// so it should free later
	return p_mesh;

_make_mesh_failed:

	glusMeshClear(p_mesh);

	return NULL;
}

/*
 *	test is the face back of eye
 */
// create [10/18/2016 blue]
bool
glusIsFaceBack(
_In_	PGlusVector	_eye,
_In_	PGlusVector	_p,
_In_	PGlusVector	_v)
{
	assert(_eye && _p && _v);

	GlusVector	v_eye; glusVFromPoint(_eye, _p, &v_eye);

	if (glusVDotPro(&v_eye, _v) > 0)
		return true;
	else
		return false;
}