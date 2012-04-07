#include <stdio.h>

void main()
{
	FILE *fp;
	int input[188*4];
	int data[188][4];
	int i, index;
	
	fp = fopen("table5.1.3-3.txt", "r");
	if (NULL == fp)
		exit(-1);
	
	for (i=0; i<188*4; i++)
		fscanf(fp, "%d", &input[i]);
		
	fclose(fp);	
	
	//for (i=0; i<188*4; i++)
	//	printf("%d ", input[i]);
		
	for (i=0; i<188; i++) {
		index = input[i*4];
		data[index-1][0] = index;
		data[index-1][1] = input[i*4+1];
		data[index-1][2] = input[i*4+2];
		data[index-1][3] = input[i*4+3];
	}
	
	//for (i=0; i<188; i++)
	//	printf("%d %d %d %d\n", data[i][0], data[i][1], data[i][2], data[i][3]);
	
	fp = fopen("output_table.txt", "w");
	if (NULL == fp)
		exit(-1);
		
	fprintf(fp, "K-table:\n");
	for (i=0; i<188; i++)
		fprintf(fp, "%d, ", data[i][1]);
	
	fprintf(fp, "\nf1-table:\n");
	for (i=0; i<188; i++)
		fprintf(fp, "%d, ", data[i][2]);
	
	fprintf(fp, "\nf2-table:\n");
	for (i=0; i<188; i++)
		fprintf(fp, "%d, ", data[i][3]);
		
	fprintf(fp, "\n");
	fclose(fp);
}