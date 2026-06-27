#include <stdio.h>
#include "Vec.h"

DEFINE_VEC(int, int)
DEFINE_VEC(Vec_int, DDVec)

int main()
{
	Vec_DDVec dd = Vec_DDVec_init();

	for (int i = 1; i <= 10; i++)
	{
		Vec_int v = Vec_int_init();
		Vec_int_push(&v, i);
		Vec_DDVec_push(&dd, v);
	}
	Vec_DDVec_eraseN(&dd, 1, 2);
	for (int i = 0; i < dd.length; i++)
	{
		printf("%d ", dd.buffer[i].buffer[0]);
	}

	for (int i = 0; i < dd.length; i++)
	{
		Vec_int_free(&dd.buffer[i]);
	}
	Vec_DDVec_free(&dd);
	return 0;
}