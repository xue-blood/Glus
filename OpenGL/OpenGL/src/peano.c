#include "../inc/glus.h"

typedef	struct _PeanoString
{
	GlusLink	Link;

	int		N;
	int		Index;

	str		Buff;
}PeanoString,*PPeanoString;

typedef	struct _PeanoRule
{
	GlusLink	Link;
	char		Name;
	char		New[30];
}PeanoRule, *PPeanoRule;



void peano_data(PPeano _peano)
{

	assert(_peano);
	/*
	*	get the atom
	*/
	fscanf_s(_peano->File, "%*[^atom]atom]");
	glusFileScanf(_peano->File, "%s", _peano->Atom, _countof(_peano->Atom));

	glusFileScanf(_peano->File, "(%f %f %d)", &_peano->F, &_peano->A, &_peano->Ratio);	// get the forward


	glusLinkInit(&_peano->Rules);

	fscanf_s(_peano->File, "%*[^rule]rule]");	// goto to rule
	int n;
	glusFileScanf(_peano->File, "(%d)", &n);
	for (int i = 0; i < n; i++)
	{
		PPeanoRule	p; glusAllocex(p, PeanoRule,1,return);

		glusFileScanf(_peano->File, "%c", &p->Name);	// get the name
		glusFileScanf(_peano->File, "%s", p->New,_countof(p->New));	// get the new 

		glusLinkInsertTail(&_peano->Rules, p);
	}

	/*
	 *	load the exit string count
	 */
	glusLinkInit(&_peano->Strings);

	while (!feof(_peano->File))
	{
		int n;
		PPeanoString s; glusAllocex(s, PeanoString, 1, return);

		/*
		 *	get current level
		 */
		n = fscanf_s(_peano->File, "%*[^[][%d]\n", &s->N); 
		if (n <= 0) goto _no_data_;
		 
		
		s->Index = ftell(_peano->File);

		glusLinkInsertTail(&_peano->Strings, s);
		continue;

_no_data_:
		glusFree(s);
		break;
	}
}

str peano_cache(PPeano _peano, int _level)
{
	assert(_peano);

	if (_level <= 0)	_level = 1;


	/*
	*	find the current level
	*/
	PPeanoString ps = (PPeanoString)_peano->Strings.BLink;
	while (!glusLinkIsHead(ps, &_peano->Strings))
	{
		if (ps->N == _level) break;
		ps = (PPeanoString)ps->Link.BLink;
	}

	if (ps->Buff)	return ps->Buff;	//is current level already load

	int i_end;
	if (ps->Link.BLink == &_peano->Strings)	// is the end level
	{
		fseek(_peano->File, 0, SEEK_END);
		i_end = ftell(_peano->File);
	}
	else
	{
		i_end = ((PPeanoString)ps->Link.BLink)->Index;
	}

	/*
	 *	read new string
	 */
	int size = i_end - ps->Index + 1;
	glusAllocex(ps->Buff, char, size, return NULL);

	fseek(_peano->File, ps->Index, SEEK_SET);

	fread_s(ps->Buff, size - 1, 1, size - 1, _peano->File);
	ps->Buff[size - 2] = ';';

	return ps->Buff;
}

void
glusPeanoDraw(
_In_	PPeano		_peano,
_In_	int			_level)
{
	assert(_peano);

	if (_level <= 0) _level = 1;

	float len = _peano->F / pow(3,_level);

	str	as = peano_cache(_peano, _level);

	while (*as != ';')
	{
		switch (*as)
		{
		case 'F':	glusForward(len, 1); break;
		case '+':	glusTurn(_peano->A); break;
		case '-':	glusTurn(-_peano->A); break;
		default:
			break;
		}
		as++;
	}
}

void 
glusPeanoProduc(
_In_	PPeano		_peano,
_In_	int			_level)
{
	assert(_peano);

	if (_level <= 0)	_level = 1;


	PPeanoString s = (PPeanoString)_peano->Strings.FLink;
	
	if (s->N >= _level)	return; // current level already in file
	
	
	fseek(_peano->File, 0, SEEK_END);
	
	for (int i = s->N+1; i <= _level;i++)
	{
		fprintf_s(_peano->File, "\n\n[%d]\n", i); // the level
		
		/*
		 *	store new level
		 */
		PPeanoString ps; glusAllocex(ps, PeanoString, 1, return);
		ps->Index = ftell(_peano->File);
		ps->N = i;
		glusLinkInsertTail(&_peano->Strings, ps);

		str as_org = peano_cache(_peano, i-1);
		/*
		 *	produce the string
		 */
		while (*as_org != ';' )
		{

			PPeanoRule	rule = (PPeanoRule)_peano->Rules.BLink;
			bool is_convert = false;

			while (!glusLinkIsHead(rule, &_peano->Rules))
			{
				if (*as_org == rule->Name) {
					is_convert = true;break;
				}
				
				rule = (PPeanoRule)rule->Link.BLink;
			}

			if (is_convert)	fputs(rule->New, _peano->File);
			else			fputc(*as_org, _peano->File);

			as_org++;
		}

		fputc(';', _peano->File);	// put an end tag

	}

}

void 
glusPeano(
_In_	str		_file_name,
_In_	PPeano	_pea,
_In_	int		_level)
{
	assert(_file_name &&_level);
	
	
	fopen_s(&_pea->File,_file_name, "a+");
	if (!_pea->File) return;

	if (_level >= 10) glusLog("Warning : ( %d )Level is to high.\n", _level);
	if (_level <= 0)	_level = 1;
	
	peano_data(_pea);

	glusPeanoProduc(_pea, _level);

}

void 
glusPeanoClear(
_In_	PPeano	_pea)
{
	glusLinkClear(&_pea->Rules);

	while (!glusLinkIsEmpty(&_pea->Strings))
	{
		PPeanoString ps = glusLinkRemoveTail(&_pea->Strings);
		glusFree(ps->Buff);
		glusFree(ps);
	}

	fclose(_pea->File);
}