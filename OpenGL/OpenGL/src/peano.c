#include "../inc/glus.h"

void
glusPeano(
_In_	PPeano		_peano,
_In_	str			_as,
_In_	double		_len,
_In_	int			_level)
{
	assert(_peano && _as);

	if (_level < 0) _level = 0;
	if (_level > 10) glusLogex(Glus_Log_Info, "[Peano] : level is to high (%d).\n", _level);

	_len /= _peano->Ratio;
	
	while (*_as)
	{
		switch (*_as)
		{
		case '+':	glusTurn(_peano->A + -5 *_peano->Radom); break;
		case '-':	glusTurn(-_peano->A + -5 * _peano->Radom); break;
		case 'F':	
			if (_level > 0)	glusPeano(_peano, _peano->FString, _len, _level - 1);
			else			
			{
				if (_peano->Level > 0)
				{
					glLineWidth(6 / _peano->Level);
				}
				glusForward(_len * (0.8+_peano->Radom), 1);
			}
			break;
		case 'X':
			if (_level > 0)	glusPeano(_peano, _peano->XString, _len, _level - 1);
			break;
		case 'Y':
			if (_level > 0)	glusPeano(_peano, _peano->YString, _len, _level - 1);
			break;
		case '[': glusPushCS(); break;
		case ']': glusPopCS();	break;
		}
		glusLogex(Glus_Log_Info, "%c", *_as);
		_as++;
	}

}

void 
glusPeanoDraw(
_In_	PPeano	_peano)
{
	assert(_peano);

	glusLogex(Glus_Log_Info, "\r");

	glusMoveTo(0, 0, 0);
	glusTurnTo(_peano->StartAngle);
	glusPeano(_peano, _peano->Atom, _peano->F,_peano->Level);
}

void 
glusPeanoLoad(
_In_	FILE *	_file,
_In_	PPeano	_pea)
{
	assert(_file && _pea);
	fscanf_s(_file, "%*[^(]"); // skip the '('
	fgetc(_file);


	/*
	 *	load the main information
	 */
	glusScanf(_file, "%s,", _pea->Atom, _countof(_pea->Atom)); // get the atom
	*(strchr(_pea->Atom, ','))=0;

	glusScanf(_file, "%s,", _pea->FString, _countof(_pea->FString)); // f-string
	*(strchr(_pea->FString, ',')) = 0;

	glusScanf(_file, "%s,", _pea->XString, _countof(_pea->XString)); // x-string
	if (strequ(_pea->XString, "nil,", _countof("nil,")))	_pea->XString[0] = 0;
	else								*(strchr(_pea->XString, ',')) = 0;

	glusScanf(_file, "%s,", _pea->YString, _countof(_pea->YString)); // y-string
	if (strequ(_pea->YString, "nil,", _countof("nil,")))	_pea->YString[0] = 0;
	else								*(strchr(_pea->YString, ',')) = 0;

	glusScanf(_file, "%f,", &_pea->F); // length 

	glusScanf(_file, "%f,", &_pea->A); // angle

	glusScanf(_file, "%f", &_pea->Ratio); // length ratio

	fscanf_s(_file, "%*[^)])"); // skip the ')'
	fgetc(_file);

	/*
	 *	load other control information
	 */

	glusScanf(_file, "%d", &_pea->Level);

	glusScanf(_file, "%f", &_pea->StartAngle);

	glusScanf(_file, "%f", &_pea->Radom);
}