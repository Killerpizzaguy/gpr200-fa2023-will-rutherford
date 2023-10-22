#include "wrMath.h"

namespace wr {

	float clamp(float in, float min, float max)
	{
		if (in < min)
			in = min;
		if (in > max)
			in = max;
		return in;
	}

}
