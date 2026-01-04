//
// Entirely random effect hiding discontinuities in the
// sequence by picking two different delays, and
// multiplying them with sin**2/cos**2
//
// Approximate a pitch shifter. Not a great one, I'm
// afraid.
//
struct {
	unsigned int i;
	float diff;
	float delay;
} disco;

#define DISCONT_STEPS 4096

void discont_init(float pot1, float pot2, float pot3, float pot4)
{
	float semitone = rintf(pot1 * 24 - 12) / 12;

	// Which direction do we walk the samples?
	// Walking backwards lowers the pitch
	// Walking forwards raises the pitch
	// Staying at the same delay keeps the pitch the same
	disco.diff = fastpow2(semitone) - 1;
	if (disco.diff <= 0)
		disco.delay = 0;
	else
		disco.delay = disco.diff * DISCONT_STEPS;
}

// sin_i is discontinuous when sin is 0
// cos_i is discontinuous when cos is 0
float discont_step(float in)
{
	int i = disco.i, cos_i;
	disco.i = (i+1)&(DISCONT_STEPS-1);
	cos_i = (i + DISCONT_STEPS/2)&(DISCONT_STEPS-1);

	struct sincos sincos = fastsincos(i / ((float)2*DISCONT_STEPS));
	sincos.sin = sincos.sin * sincos.sin;
	sincos.cos = sincos.cos * sincos.cos;

	sample_array_write(in);
	float d1 = sample_array_read(disco.delay - i*disco.diff) * sincos.sin;
	float d2 = sample_array_read(disco.delay - cos_i*disco.diff) * sincos.cos;

	return d1+d2;
}
