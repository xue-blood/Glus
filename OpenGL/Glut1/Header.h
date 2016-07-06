#include "..\OpenGL\inc\glus.h"
#pragma comment(lib,"../Debug/opengl.lib")
#include <stdio.h>

typedef struct _Link
{
	GlusSink	link;
	GLint		n;
}Link,*PLink;

GlusSink Head;