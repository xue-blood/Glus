#ifndef _Glus_Link_
#define _Glus_Link_

// doubly link list
typedef struct _GlusLink
{
	struct _GlusLink*	FLink;
	struct _GlusLink*	BLink;
}GlusLink, *PGlusLink;

// singly link list
typedef struct _GlusSink
{
	struct _GlusSink*	Next;
}GlusSink, *PGlusSink;


typedef struct _GlusMulSink
{
	GlusSink	Sink;
	GlusSink	Data;
}GlusMulSink, *PGlusMulSink;

typedef struct _GlusLinks
{
	GlusLink	Link;
	GlusLink	Data;
}GlusLinks, *PGlusLinks;
#endif