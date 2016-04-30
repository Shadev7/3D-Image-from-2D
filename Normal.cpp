#include <iostream>
#include <cmath>

#include <Normal.h>

Normal::Normal(const Matrix& m, const SqMatrix& var):
		mean(m), inv_cov(var.inverse()){
	if (m.rowCount() != var.rowCount()) {
		std::cerr<<"Invalid dimensions\n";
	}
	dimensions = var.rowCount();
	this->coefficient = 1.0/std::sqrt(pow(2*M_PI, dimensions)*var.determinant());
}

double Normal::eval(const Matrix& input) {
	if (input.rowCount() != mean.rowCount() && 
				input.rowCount() != mean.colCount()) {
		std::cerr<<"Invalid dimensions\n";
		return 0.0;
	}
	double param = -0.5 * 
			((input - mean).transpose() * inv_cov * (input - mean))(0,0);
	return coefficient * std::exp(param);
}

