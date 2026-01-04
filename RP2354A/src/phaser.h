struct {
	struct lfo_state lfo;
	struct biquad_coeff coeff;
	struct biquad_state ph1, ph2, ph3;
	float center_f, octaves, Q;
	float feedback, prev;
} phaser;

#define linear(pot, a, b) ((a)+pot*((b)-(a)))
#define cubic(pot, a, b) linear((pot)*(pot)*(pot), a, b)

void phaser_init(float pot1, float pot2, float pot3, float pot4)
{
	set_lfo_ms(&phaser.lfo, cubic(pot1, 50, 2000)); // 0.05 .. 2s
	phaser.feedback = linear(pot2, 0, 0.75);

	pot3 = 2*pot3;
	phaser.center_f = 110 * pot3* pot3 *pot3;	// 110 .. 880
	phaser.octaves = 1;
	phaser.Q = linear(pot4, 0.25, 2);
}

float phaser_step(float in)
{
	float lfo = lfo_step(&phaser.lfo, lfo_triangle);
	float freq = fastpow(2, lfo*phaser.octaves) * phaser.center_f;
	float out;

	biquad_allpass_filter(&phaser.coeff, freq, phaser.Q);

	out = biquad_step(&phaser.coeff, &phaser.ph1, in + phaser.prev * phaser.feedback);
	out = biquad_step(&phaser.coeff, &phaser.ph2, out);
	out = biquad_step(&phaser.coeff, &phaser.ph3, out);
	phaser.prev = out;

	return in + out;
}
