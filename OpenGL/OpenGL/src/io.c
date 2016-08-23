#include "..\inc\glus.h"
#include <stdio.h>
#include <strsafe.h>

//
// read a watch-able string from file 
// trash ' ',  '\t'
//
// add [8/23/2016 tld]
size
glusFileRead(
_In_	FILE *	_file,
_Inout_	str	_str,
_In_	size	_max_size)
{
	char c = ' ';
	size n = 0;

	ZeroMemory(_str, _max_size*sizeof(char));

	//
	// trash head
	//
	while (c == ' ' || c == '\t')
	{
		c = fgetc(_file);
		
		// is line end
		if (c == '\n')
			return n;
	}
	_str[n++] = c;	// get the first char

	//
	// get context
	//
	while (true)
	{
		c = fgetc(_file);
		// is line end
		if (c == '\n')
			return n;

		if (c == ' ' || c == '\t')
			break;



		_str[n++] = c;	// get the next char

		// is full
		if (n == _max_size)
			break;
	}

	return n;
}