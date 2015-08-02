#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <bitset>

void test_tree();
void test_callback(bis key, int val, void *p);

int   read_isi(FILE *in, float **arr, float *sum, float *min);
int   binarize(float *isi, int isi_len, int *bin, float dt);
void  getHcallback(bis key, int val, void *Haddr);
double getG(int order);

#define MAX_ORDER 200
#define GSIZE 1000000
double g_G[GSIZE];

typedef struct cbParTag
{
	double H;
	int max_ni;
} cbPar;

int main(int argc, char *argv[])
{
	char infile[500];
	char outfile[500];
	printf("Version 1.1\n");
	if (argc < 2)
	{
		printf("Use as following:\n");
		printf("   entisi.exe isi.txt out.txt [max_order [Tmin]]\n\n  or  \n\n");
		printf("Input file name: ");
		scanf("%s", &infile);
		printf("Output file name: ");
		scanf("%s", &outfile);
		//sprintf(infile, "isiN5maj.txt");
		//sprintf(outfile, "outMaj.txt");
	}
	else
	{
		sprintf(infile, "%s", argv[1]);
		sprintf(outfile, "%s", argv[2]);
	}
	FILE  *in  = fopen(infile, "r");
	FILE  *out = fopen(outfile, "w");
	float *isi = 0;
	int    isi_len;
	float  isi_sum = 0.0f, isi_min = 1.e+10f;
	float  tmin = 0.0f;
	int   *bin;
	int    bin_len;
	int    MaxOrder = MAX_ORDER;
	
	printf("Input file %s has got the pointer: %d\n",  infile, (int)in);
	printf("Output file %s has got the pointer: %d\n", outfile, (int)out);

	if (!in || !out) return -1;

	printf("Reading ISI file...\n");
	isi_len = read_isi(in, &isi, &isi_sum, &isi_min);
	if (!isi_len)
	{
		printf("No ISI in the input file.\n");
		return -2;
	}
	
	if (argc >= 4)
	{
		MaxOrder = atoi(argv[3]);
		if (MaxOrder > MAX_ORDER)
		{
			printf("Maximal order can be set only up to %d\n", MAX_ORDER);
			MaxOrder = MAX_ORDER;
		}
	}

	if (argc >= 5)
	{
		tmin = (float)atof(argv[4]);
	}
	else
	{
		tmin = 0.9f*isi_min;
	}

	bin = new int[1 + (int)(isi_sum / tmin)];
	printf("Binarization with time period %f...\n", tmin);
	bin_len = binarize(isi, isi_len, bin, tmin);
	printf("Digit count: %d\n", bin_len);
	if (!bin_len) return -3;
	
	double Hprev = 0.0;
	double hprev = 0.0;
	printf("Calculating entropies:\n");
	fprintf(out, "# Order, H, h, Different words, All words, max(n_i)\n");
	fprintf(out, "#-------------------------------------------------------------------------\n");

	for (int order = 1; order <= MaxOrder; order++)
	{
		BinarySearchTree *tree = new BinarySearchTree(200);
		int N = 0;
		int i = 0;
		while (i + order < bin_len)
		{
			std::bitset<MAX_ORDER> word(0);
			for (int j = 0; j < order; j++)
			{
				word <<= 1;
				word  |= bin[i + j];
			}
			if (order < 65)
				tree->put(word.to_ullong(), 1, order);
			else
				tree->put((bis)word.hash(), 1, order);
			N++;
			i++;
		}
		cbPar pars;
		pars.H = 0.0;
		pars.max_ni = 0;
		double H = 0.0;
		double h = 0.0;
		bool   breakFlag = false;
		tree->inorder(getHcallback, (void*)&pars);
		H = pars.H;
		H /= (double)N;
		H += log((double)N);
		H *= 1.4426950408889634; // log2(e)
		if (order > 1)
		{
			h = H - Hprev;
			if (abs(h - hprev) < 1.e-5)
			{
				breakFlag = true;
			}
			hprev = h;
		}
		Hprev = H;
		fprintf(out, "%d, %0.12f, %0.12f, %d, %d, %d\n", order, H, h, tree->getNodeCount(), N, pars.max_ni);
		printf("H(%d): %0.12f, h(%d): %0.12f", order, H, order, h);
		int levels = 0;
		tree->getDepth(levels);
		printf("  Tree nodes: %d, levels: %d, max n_i: %d. Analyzed words: %d\n", tree->getNodeCount(), levels, pars.max_ni, N);
		delete tree;
		if (breakFlag) break;
	}
	printf("Bin interval: %0.12f\n", tmin);
	delete[] isi;
	delete[] bin;
	fclose(in);
	fclose(out);
	
	//test_tree();
	
}

int  read_isi(FILE *in, float **arr, float *sum, float *min)
{
	int   isi_len = 0;
	float isi;
	int   arr_len = 500;
	*arr = new float[arr_len];
	*sum = 0.0f;
	*min = 1.e+10f;
	while (fscanf(in, "%f", &isi) > 0)
	{
		(*arr)[isi_len++] = isi;
		*sum += isi;
		if (isi < *min)
		{
			*min = isi;
		}
		if (isi_len >= arr_len)
		{
			float *new_arr = new float[2 * arr_len];
			memcpy(new_arr, *arr, arr_len*sizeof(float));
			arr_len *= 2;
			delete[] *arr;
			*arr = new_arr;
		}
	}
	return isi_len;
}

int binarize(float *isi, int isi_len, int *bin, float dt)
{
	int bin_idx = 0;
	int isi_idx = 0;
	float Tcurr = isi[0];
	while (true)
	{
		if (dt >= Tcurr)
		{
			bin[bin_idx++] = 1;
			isi_idx++;
			if (isi_idx >= isi_len) break;
			Tcurr = isi[isi_idx] - (dt - Tcurr);
		}
		else
		{
			bin[bin_idx++] = 0;
			Tcurr -= dt;
		}
	}
	return bin_idx;
}

void  getHcallback(bis key, int val, void *p)
{
	cbPar *par = (cbPar*)p;
	par->H -= (double)val * getG(val);
	if (val > par->max_ni)
	{
		par->max_ni = val;
	}
	//*H -= (double)val * log((double)val);
}

double getG(int order)
{
	double gam = 0.577215664901532;
	static int    last_order = 0;
	
	if (last_order == 0)
	{
		g_G[0] = 0.0f;
		g_G[1] = -gam - log(2.0);
		g_G[2] = g_G[1] + 2.0;
		g_G[3] = g_G[2];
		last_order = 3;
	}
	if (last_order < order)
	{
		while (last_order < 2 * order)
		{
			double g = g_G[last_order] + 2.0 / (double)last_order;
			if (last_order + 2 >= GSIZE - 1)
			{
				printf("Not enough G[] for Order = %d!!!\n", order);
				return g_G[last_order];
			}
			g_G[++last_order] = g;
			g_G[++last_order] = g;
		}
	}
	return g_G[order];
}

void test_tree()
{
	BinarySearchTree *tree = new BinarySearchTree(5);
	char s[100];
	printf("command>> ");
	scanf("%s", &s);
	while (s[0] != 'q')
	{
		if (s[0] == 'i')
		{
			tree->inorder(test_callback, 0);
			printf("\ncommand>> ");
			scanf("%s", &s);
			continue;
		}
		int d = 0;
		int k = atoi(s);
		tree->put(k, 1, 32);
		tree->getDepth(d);
		printf("Depth: %d\n", d);
		printf("command>> ");
		scanf("%s", &s);
	}
	delete tree;
}

void test_callback(bis key, int val, void *p)
{
	printf(" %d::%d ", key, val);
}