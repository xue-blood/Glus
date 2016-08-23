#include "..\inc\glus.h"

/*
//  [7/6/2016 Tld]
	glusLinkLength	add
*/

//------------Doubly link list---------------

void 
glusLinkInit(
_Out_	void*	_linkHead)
{
	assertp(_linkHead);
	PGlusLink	h = _linkHead;

	h->FLink =	h;
	h->BLink =	h;
}
GLint	
glusLinkLength(
_In_	PGlusLink	_linkHead)
{
	assertp(_linkHead);

	GLint	l = 0;
	PGlusLink	p = _linkHead->BLink;
	while (p != _linkHead)
	{
		p = p->BLink;
		l++;
	}
	return l;
}

void	
glusLinkInsertHead(
_Inout_ void*	_linkHead,
_Inout_ void*	_linkNode)
{
	assertp(_linkHead && _linkNode);

	PGlusLink	h = _linkHead;
	PGlusLink	n = _linkNode;

	n->FLink =	h;
	n->BLink =	h->BLink;
	
	h->BLink =	n;
	if (h->FLink == h)
		h->FLink = n;
}

void
glusLinkInsertTail(
_Inout_ void*	_linkHead,
_Inout_ void*	_linkNode)
{
	assertp(_linkHead && _linkNode);

	PGlusLink	h = _linkHead;
	PGlusLink	n = _linkNode;

	n->FLink =	h->FLink;
	n->BLink =	h;

	n->FLink->BLink = n;
	h->FLink =	n;
}

pvoid
glusLinkRemoveHead(
_Inout_	pvoid	_linkHead)
{
	assertp(_linkHead );

	PGlusLink	h = _linkHead,
				n = h->BLink;

	h->BLink =	n->BLink;

	n->BLink->FLink = h;

	return	n;
}
pvoid
glusLinkRemoveTail(
_Inout_	pvoid	_linkHead)
{
	assertp(_linkHead );

	PGlusLink	h = _linkHead,
				n = h->FLink;

	h->FLink =	n->FLink;
	n->FLink->BLink = h;

	return	n;
}
bool	
glusLinkRemoveEntry(
_Inout_ pvoid	_linkEntry)
{
	assertp(_linkEntry);

	PGlusLink	n = _linkEntry;

	n->BLink->FLink	=	n->FLink;
	n->FLink->BLink =	n->BLink;

	//
	// is the link list is empty
	//
	if (n->FLink->FLink == n->FLink)
		return true;
	else
		return false;
}

//
// clear the memory for the link list
//
// add [8/22/2016 tld]
void
glusLinkClear(
_Inout_	PGlusLink	_link_head)
{
	assertp(_link_head);

	while (!glusLinkIsEmpty(_link_head))
	{
		PGlusLink p = glusLinkRemoveHead(_link_head);
		glusFree(p);
	}
	//glusFree(_link_head);  // we needn't free head
}
//============Doubly link list===============

//------------Singly link list---------------
void	
glusSinkPush(
_Inout_ PGlusSink	_linkHead,
_In_	PGlusSink	_linkEntry)
{
	assertp(_linkHead && _linkEntry);

	//  [7/6/2016 Tld]: change from tail to head
	_linkEntry->Next = _linkHead->Next;
	_linkHead->Next = _linkEntry;
}

PGlusSink
glusSinkPop(
_Inout_ PGlusSink	_linkHead)
{
	assertp(_linkHead );

	PGlusSink	p = _linkHead->Next;

	//  [7/6/2016 Tld]: change from tail to head
	_linkHead->Next = p->Next;

	return p;
}

GLint
glusSinkLength(
_In_	PGlusSink	_linkHead)
{
	GLint	l = 0;

	while (_linkHead->Next)
	{
		_linkHead = _linkHead->Next;
		l++;
	}
	return l;
}
//============Singly link list===============