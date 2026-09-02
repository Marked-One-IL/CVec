#include <stdio.h>
#include "Vec.h"

DEFINE_VEC_HEADER(int, int)
DEFINE_VEC_SOURCE(int, int)
DEFINE_VEC_HEADER(Vec_int, intVec)
DEFINE_VEC_SOURCE(Vec_int, intVec)

#define LENGTH 10

int main()
{
	// Init
	Vec_intVec intVec = Vec_intVec_init();
	for (size_t i = 0; i < LENGTH; i++)
	{
		Vec_int vInt = Vec_int_init();

		for (size_t j = 0; j < LENGTH; j++)
		{
			Vec_int_push(&vInt, i + j);
		}

		Vec_intVec_push(&intVec, vInt);
	}

	// Use + Free
	for (size_t i = 0; i < intVec.length; i++)
	{
		Vec_int v1 = intVec.buffer[i];

		for (size_t j = 0; j < v1.length; j++)
		{
			int v2 = v1.buffer[j];
			printf("%d ", v2);
		}
		printf("\n");

		Vec_int_free(&v1);
	}

	Vec_intVec_free(&intVec);
	return 0;
}