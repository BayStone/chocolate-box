#include "Stdafx.h"
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

char* CopyXFileSafeString(const char* src)
{
	int len = strlen(src);
	char* ret;
	if (src[0] >= '0' && src[0] <= '9')
	{
		len++;
		ret = new char[len+1];
		strcpy(ret+1, src);
		ret[0] = 'X';
	}
	else
	{
		ret = new char[len+1];
		strcpy(ret, src);
	}

	for(int i=0;i<len;i++)
		if (ret[i] == ' ')
			ret[i] = '-';
		else if (ret[i] == '#')
			ret[i] = '_';

	return ret;
}

char* StringFromXFileString(const char* src)
{
	int len = strlen(src);
	char* ret;
	if (src[0] == 'X')
	{
		ret = new char[len+1];
		strcpy(ret, src+1);
	}
	else
	{
		ret = new char[len+1];
		strcpy(ret, src);
	}

	for(int i=0;i<len;i++)
		if (ret[i] == '-')
			ret[i] = ' ';

	return ret;
}

}
}
}