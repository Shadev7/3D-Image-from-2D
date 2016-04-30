#ifndef NORMAL_H__

#include <Matrix.h>

class Normal {
	Matrix mean;
	Matrix inv_cov;
	int dimensions;
	double coefficient;
public:
	Normal(const Matrix& mean, const SqMatrix& var);

	double eval(const Matrix&);
};

#endif
