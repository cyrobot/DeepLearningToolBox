//training data:nDim * nExamples
#include <Eigen/Dense>
#include "FunctionBase.h"
using namespace Eigen;
using namespace std;


class CA:public FunctionBase
{
public:
	MatrixXd PCA(MatrixXd &x,double ratio);
	MatrixXd PCAWhite(MatrixXd &x);
	MatrixXd ZCAWhite(MatrixXd &x);
	MatrixXd getZCAWhiteMat(MatrixXd &x);
	MatrixXd getPCAWhiteMat(MatrixXd &x);
};

//Principal component analysis
MatrixXd CA::PCA(MatrixXd &x,double ratio)
{
	if(ratio <= 0 || ratio > 1)
	{
		return MatrixXd::Zero(x.rows(),x.cols());
	}
	MatrixXd avg = x.rowwise().mean();
	//Zero mean the data set
	x = x - avg.replicate(1,x.cols());
	//Calculate the covariance
	MatrixXd sigma = x * x.transpose() * (1.0 / x.cols());
	//Singular value decomposition
	JacobiSVD<MatrixXd> svd(sigma, ComputeFullU | ComputeFullV);
	MatrixXd U = svd.matrixU();
	MatrixXd V = svd.matrixV();
	MatrixXd S = svd.singularValues();
	// Project the data set
	MatrixXd xRot = U.transpose() * x;
	//Calculate the covariance after projection
	MatrixXd cov = xRot * xRot.transpose() * (1.0 / xRot.cols());
	int k = 0;
	double p = 0;
	double curVar = 0;
	//summation of singular values
	double sumVar = S.array().sum();
	for(k = 0;k < S.rows() && p < ratio;k++)
	{
		k = k + 1;
		curVar += S(k,0);
		p = curVar / sumVar;
	}
	xRot.bottomRows(S.rows() - k) = MatrixXd::Zero(S.rows() - k,xRot.cols());
	MatrixXd xHat = U * xRot;
	return xHat;
}

//Calculate the PCA White value of input
MatrixXd CA::PCAWhite(MatrixXd &x)
{
	
	MatrixXd avg = x.rowwise().mean();
	//Zero mean the data set
	x = x - avg.replicate(1,x.cols());
	//Calculate the covariance
	MatrixXd sigma = x * x.transpose() * (1.0 / x.cols());
	//Singular value decomposition
	JacobiSVD<MatrixXd> svd(sigma, ComputeFullU | ComputeFullV);
	MatrixXd U = svd.matrixU();
	MatrixXd V = svd.matrixV();
	MatrixXd S = svd.singularValues();
	//calculate the pca white value
	double epsilon = 0.1;
	MatrixXd term1 = S + MatrixXd::Ones(S.rows(),S.cols()) * epsilon;
	MatrixXd term2 = reciprocalMat(sqrtMat(term1));
	MatrixXd xPCAWhite = term2.asDiagonal() * U.transpose() * x;
	return xPCAWhite;
}

//Calculate the ZCA White value of input
MatrixXd CA::ZCAWhite(MatrixXd &x)
{
	MatrixXd avg = x.rowwise().mean();
	//Zero mean the data set
	x = x - avg.replicate(1,x.cols());
	//Calculate the covariance
	MatrixXd sigma = x * x.transpose() * (1.0 / x.cols());
	//Singular value decomposition
	JacobiSVD<MatrixXd> svd(sigma, ComputeFullU | ComputeFullV);
	MatrixXd U = svd.matrixU();
	MatrixXd V = svd.matrixV();
	MatrixXd S = svd.singularValues();
	//calculate the pca white value
	double epsilon = 0.1;
	MatrixXd term1 = S + MatrixXd::Ones(S.rows(),S.cols()) * epsilon;
	MatrixXd term2 = reciprocalMat(sqrt(term1));
	MatrixXd xPCAWhite = term2.asDiagonal() * U.transpose() * x;
	return U * xPCAWhite;
}

//Calculate the PCA White transform matrix
MatrixXd CA::getPCAWhiteMat(MatrixXd &x)
{
	MatrixXd avg = x.rowwise().mean();
	x = x - avg.replicate(1,x.cols());
	MatrixXd sigma = x * x.transpose() * (1.0 / x.cols());
	JacobiSVD<MatrixXd> svd(sigma, ComputeFullU | ComputeFullV);
	MatrixXd U = svd.matrixU();
	MatrixXd V = svd.matrixV();
	MatrixXd S = svd.singularValues();
	double epsilon = 0.1;
	MatrixXd term1 = S + MatrixXd::Ones(S.rows(),S.cols()) * epsilon;
	MatrixXd term2 = reciprocalMat(sqrtMat(term1));
	MatrixXd xPCAWhiteMat = term2.asDiagonal() * U.transpose();
	return xPCAWhiteMat;
}

//Calculate the ZCA White transform matrix
MatrixXd CA::getZCAWhiteMat(MatrixXd &x)
{
	MatrixXd avg = x.rowwise().mean();
	x = x - avg.replicate(1,x.cols());
	MatrixXd sigma = x * x.transpose() * (1.0 / x.cols());
	JacobiSVD<MatrixXd> svd(sigma, ComputeFullU | ComputeFullV);
	MatrixXd U = svd.matrixU();
	MatrixXd V = svd.matrixV();
	MatrixXd S = svd.singularValues();
	double epsilon = 0.1;
	MatrixXd term1 = S + MatrixXd::Ones(S.rows(),S.cols()) * epsilon;
	MatrixXd term2 = reciprocalMat(sqrtMat(term1));
	MatrixXd xZCAWhiteMat = U * term2.asDiagonal() * U.transpose();
	return xZCAWhiteMat;
}