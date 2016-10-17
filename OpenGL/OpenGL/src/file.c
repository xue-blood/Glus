#include "..\inc\glus.h"
#include <stdio.h>
#include <strsafe.h>



//
// load point from file 
// work for 3d
//
// add [8/30/2016 tld]
void		
glusFileLoadPoint(
_In_	FILE *			_file,
_In_	str				_format,
_Outptr_ PGlusVector	_p_point)
{
	assertp(_file && _format && _p_point);

	glusFileScanf(_file,_format, &_p_point->X, &_p_point->Y, &_p_point->Z);
	_p_point->V = 1;
}

//
// load vector from file 
// work for 3d
//
// add [8/30/2016 tld]
void
glusFileLoadVector(
_In_	FILE *			_file,
_In_	str				_format,
_Outptr_ PGlusVector	_vector)
{
	assertp(_file && _format && _vector);


	glusFileScanf(_file,_format, &_vector->X, &_vector->Y, &_vector->Z);
	_vector->V = 0;
}


//
// load points to array from file
//
// add [8/30/2016 tld]
Glussize
glusFileLoadPoints_A(
_In_	FILE *		_file,
_Inout_	PGlusVector	_buffer,
_In_	Glussize	_max_size)
{
	assertp(_file && _buffer);
	assert(_max_size > 0);


	//
	// fill the data
	//
	Glussize i = 0;
	for (; i < _max_size;i++)
	{
		// is file end
		if (feof(_file))
			break;

		glusFileLoadPoint(_file, "(%lf,%lf,%lf)", _buffer + i);
	}

	return i;
}

//
// load points to doubly link list from file
//
// add [8/30/2016 tld]
Glussize
glusFileLoadPoints_L(
_In_	FILE *		_file,
_In_	PGlusLink	_head,
_In_	Glussize	_max_size)
{
	assertp(_file && _head );
	assert(_max_size > 0);

	Glussize i = 0;
	for (; i < _max_size; i++)
	{
		// is file end
		if (feof(_file))
			break;


		//
		// allocate memory for entry
		//
		PGlusPoints	p_entry; 
		glusAlloc(p_entry,GlusPoints);
		

		// get data
		glusFileLoadPoint(_file, "(%lf,%lf,%lf)", &p_entry->Point);

		// add to link list
		glusLinkInsertTail(_head, p_entry);
	}

	return i;
}



//
// load vectors to array from file
//
// add [8/31/2016 blue]
Glussize
glusFileLoadVectors_A(
_In_	FILE *		_file,
_Inout_	PGlusVector	_buffer,
_In_	Glussize	_max_size)
{
	assertp(_file && _buffer);
	assert(_max_size > 0);


	//
	// fill the data
	//
	Glussize i = 0;
	for (; i < _max_size; i++)
	{
		// is file end
		if (feof(_file))
			break;

		glusFileLoadVector(_file, "<%lf,%lf,%lf>", _buffer + i);
	}

	return i;
}


//
// load vectors to doubly link list from file
//
// add [8/31/2016 blue]
Glussize
glusFileLoadVectors_L(
_In_	FILE *		_file,
_In_	PGlusLink	_head,
_In_	Glussize	_max_size)
{
	assertp(_file && _head);
	assert(_max_size > 0);

	Glussize i = 0;
	for (; i < _max_size; i++)
	{
		// is file end
		if (feof(_file))
			break;


		//
		// allocate memory for entry
		//
		PGlusPoints	p_entry;
		glusAlloc(p_entry, GlusPoints);
		
		// get data
		glusFileLoadVector(_file, "<%lf,%lf,%lf>", &p_entry->Point);

		// add to link list
		glusLinkInsertTail(_head, p_entry);
	}

	return i;
}
