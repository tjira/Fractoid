inline double distance(double zRe, double zIm, double zMag, int trap) {
	switch (trap) {
		case 1: return zMag;
		case 2: return MIN(ABS(zRe), ABS(zIm));
		case 3: return 0.70710678f * MIN(ABS(zRe - zIm), ABS(zRe + zIm));
		case 4: return ABS(zMag - 1);
		default: return zMag;
	}
}
