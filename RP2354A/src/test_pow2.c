#include <math.h>
#include <stdio.h>

#define SAMPLES_PER_SEC 52083.33333f

typedef unsigned int uint;
#include "util.h"

int main(int argc, char **argv)
{
	double error = 0;

	for (double f = -2; f < 2.01; f+=0.1) {
		float my = fastpow2(f);
		double exact = pow(2.0, f);
		double epow = (my - exact) / exact;
		printf("%.06f: %.08f %.08f (%.08f)\n", f, my, exact, epow);
		error = fmax(fabs(epow),error);
	}
	printf("Max error %.8f (%.1f digits)\n", error, -log10(error));
}
