#include "../inc/glus.h"

void
glusPeano(
_In_	PPeano		_peano,
_In_	str			_as,
_In_	int			_level)
{
	assert(_peano && _as);

	if (_level < 0) _level = 0;
	if (_level > 10) glusLogex(Glus_Log_Info, "[Peano] : level is to high (%d).\n", _level);


	while (*_as)
	{
		switch (*_as)
		{
		case '+':	glusTurn(_peano->A); break;
		case '-':	glusTurn(-_peano->A); break;
		case 'F':	
			if (_level > 0)	glusPeano(_peano, _peano->FString, _level - 1);
			else			
			{
				float len = _peano->F;
				if (_peano->Level > 0) 
					len= _peano->F / pow(_peano->Ratio, _peano->Level);

				glusForward(len, 1);
			}
			break;
		case 'X':
			if (_level > 0)	glusPeano(_peano, _peano->XString, _level - 1);
			break;
		case 'Y':
			if (_level > 0)	glusPeano(_peano, _peano->YString, _level - 1);
			break;
		}
		_as++;
	}

}

void 
glusPeanoDraw(
_In_	PPeano	_peano)
{
	assert(_peano);

	glusMoveTo(0, 0, 0);
	glusTurnTo(0);
	glusPeano(_peano, _peano->Atom, _peano->Level);
}

void 
glusPeanoLoad(
_In_	FILE *	_file,
_In_	PPeano	_pea)
{
	assert(_file && _pea);
	fscanf_s(_file, "%*[^(]"); // skip the '('
	fgetc(_file);


	glusFileScanf(_file, "%s,", _pea->Atom, _countof(_pea->Atom)); // get the atom
	*(strchr(_pea->Atom, ','))=0;

	glusFileScanf(_file, "%s,", _pea->FString, _countof(_pea->FString)); // f-string
	*(strchr(_pea->FString, ',')) = 0;

	glusFileScanf(_file, "%s,", _pea->XString, _countof(_pea->XString)); // x-string
	if (strequ(_pea->XString, "nil,"))	_pea->XString[0] = 0;
	else								*(strchr(_pea->XString, ',')) = 0;

	glusFileScanf(_file, "%s,", _pea->YString, _countof(_pea->YString)); // y-string
	if (strequ(_pea->YString, "nil,"))	_pea->YString[0] = 0;
	else								*(strchr(_pea->YString, ',')) = 0;

	glusFileScanf(_file, "%f,", &_pea->F); // length 

	glusFileScanf(_file, "%f,", &_pea->A); // angle

	glusFileScanf(_file, "%f", &_pea->Ratio); // length ratio

	fscanf_s(_file, "%*[^)])"); // skip the ')'
	fgetc(_file);
}