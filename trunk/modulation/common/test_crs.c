#include "complex.h"
#include "lte_macros.h"

complex dl_resources[14][72];
int dl_resource_map[14][72];

extern void crs_generate(int ns, int cptype, int cellid, int num_rb, int num_antport);

int test_crs()
{
	int i, j;
	crs_generate(0, NORMAL_CP, 0, 6, 2);
	crs_generate(1, NORMAL_CP, 0, 6, 2);
	
	for (i=71; i>=0; i--) {
		for (j=0; j<14; j++) {
			printf("%08X ", dl_resource_map[j][i]);
		}
		printf("\n");
	}
	
	return 0;
}

void main()
{
	test_crs();
}