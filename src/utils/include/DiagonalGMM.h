/*

Aalto University Game Tools license

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef DIAGONALGMM_H
#define DIAGONALGMM_H
#include <Eigen/Eigen> 
#include <vector>


namespace AaltoGames
{
	class DynamicPdfSampler;  //forward declare

	class DiagonalGMM
	{
	public:
		DiagonalGMM();
		void resize(int nComponents, int nDimensions);
		void resample(DiagonalGMM &dst, int nDstComponents);
		void copyFrom(DiagonalGMM &src);
		int sampleComponent();
		void sample(Eigen::VectorXf &dst);
		//Sample within limits. Note that this is only an approximation, as the component normalizing constants are currently computed without limits
		void sampleWithLimits( Eigen::VectorXf &dst, const Eigen::VectorXf &minValues, const Eigen::VectorXf &maxValues  );
		void sampleWithLimits( Eigen::Map<Eigen::VectorXf> &dst, const Eigen::VectorXf &minValues, const Eigen::VectorXf &maxValues  );

		//Call this after manipulating the weights vector. Normalizes the weights and updates the internal data for sampling from the GMM
		void weightsUpdated();
		//inits the GMM with a single component of infinite variance
		//		void setUniform(int nDimensions);
		void setStds(Eigen::VectorXf &src);
		double p(Eigen::VectorXf &v);
		std::vector<Eigen::VectorXf> mean;
		std::vector<Eigen::VectorXf> std;
		//note: after you manipulate the weights, call weigthsUpdated() to normalize them and update the internal data needed for the sampling functions
		Eigen::VectorXd weights;
		//Note: src1 and src2 may also have 0 or FLT_MAX as std, corresponding to fixed or unconstrained variables.
		static void multiply(DiagonalGMM &src1, DiagonalGMM &src2, DiagonalGMM &dst);
		//fixedVars contains all the known variables (the lowest indices). Returns the sum of weights before normalizing to 1
		double makeConditional(Eigen::VectorXf fixedVars, DiagonalGMM &dst);
	protected:
		DynamicPdfSampler *sampler;
		int nDimensions;
		int nComponents;
	};

} //AaltoGames


#endif //DIAGONALGMM_H