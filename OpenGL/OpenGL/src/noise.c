#include "../inc/glus.h"

void
glusNoiseInit(PGlusNoise _p)
{
	assert(_p);

	/*
	 *	build noise tables
	 */
	for (int i = 0; i < 256; i++) _p->NoiseTable[i] = rand() / 32767.99;

	/*
	 *	build indexs
	 */
	for (int i = 0; i < 256; i++) _p->Indexs[i] = i;	// fill array with indices
	for (int i = 0;	i < 256; i++)
	{
		int which = rand() % 256;	// choose random place

		/*
		 *	swap them
		 */
		unsigned char tmp = _p->Indexs[which];
		_p->Indexs[which] = _p->Indexs[i];
		_p->Indexs[i] = tmp;
	}
}
// get the true index
#define PERM(x)			_p->Indexs[ (x) &255]
// index for x,y,z
#define INDEX(ix,iy,iz) PERM((ix) + PERM( (iy) + PERM(iz)))

// get the value from noise table
float NOISE(PGlusNoise _p,int i, int j,int k)	
{
	return _p->NoiseTable[INDEX(i, j, k)];
}
float 
glusNoise(PGlusNoise _p, float _scale, float _x, float _y, float _z)
{
	assert(_p);

#define lerp(f,A,B) (A + f* (B-A))

	float d[2][2][2];

	// get x,y,z
	float x, y, z;
	x = _x * _scale + 10000; // offset avoid negative value
	y = _y * _scale + 10000;
	z = _z * _scale + 10000;

	// convent float to long
	long ix, iy, iz;
	ix = (long)x; iy = (long)y; iz = (long)z;

	// get offset for lerp
	float tx, ty, tz;
	tx = x - ix; ty = y - iy; tz = z - iz;

	float mtx, mty, mtz;
	mtx = 1.0 - tx; mty = 1.0 - ty; mtz = 1.0 - tz;

	for (int k = 0; k <= 1; k++) // get noise at 8 lattice points
		for (int j = 0; j <= 1;j++)
			for (int i = 0; i <= 1; i++)
			{
				d[k][j][i] = NOISE(_p,ix + i, iy + j, iz + k);
			}

	float x0, x1, x2, x3, y0, y1;
	x0 = lerp(tx, d[0][0][0], d[0][0][1]);
	x1 = lerp(tx, d[0][1][0], d[0][1][1]);
	x2 = lerp(tx, d[1][0][0], d[1][0][1]);
	x3 = lerp(tx, d[1][1][0], d[1][1][1]);
	y0 = lerp(ty, x0, x1);
	y1 = lerp(ty, x2, x3);

	return lerp(tz, y0, y1);
#undef lerp
}

float
glusNoiseTurb(PGlusNoise _p, float _scale, float _x, float _y, float _z,int M)
{
	assert(_p);

	double turb = 0, fa =1,fb = 1;
	for (int i = 0; i < M; i++)
	{
		turb += fa*glusNoise(_p, fb*_scale, _x, _y, _z);
		fa /= 2;
		fb *= 2;
	}

	return turb/2;
}

/*
 *	compute value in spline
 */
float undulate(float x)
{
	if (x < -0.4)		return 0.15 + 2.857  * sqr(x + 0.75);
	else if (x < 0.4)	return 0.95 - 2.8125 * sqr(x);
	else				return 0.26 + 2.666  * sqr(x - 0.7);
}

float
glusNoiseMarble(PGlusNoise _p, float _x, float _y, float _z,int A)
{
	assert(_p);

	double value;
	value = sinf(Glus_2PI * _z + A * glusNoiseTurb(_p,5,_x,_y,_z,3));

	return undulate(value);
}
