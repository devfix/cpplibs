//
// Created by core on 6/8/20.
//

#include <vector>

namespace devfix::base
{

	template<typename FloatT>
	struct interpolation
	{
		static std::vector<FloatT> calc_coeffs(const std::vector<std::pair<FloatT, FloatT>>& points)
		{
			if (points.empty()) { throw std::invalid_argument("points vector cannot be empty"); }
			std::vector<FloatT> coeffs;
			coeffs.reserve(points.size());
			std::vector<FloatT> last_row;
			for (std::size_t j = 0; j < points.size(); j++)
			{
				std::vector<FloatT> row(points.size());
				row[0] = points[j].second;
				//std::cout << "[" << j << ";" << j<< "] ";
				//std::cout << row[0] << " ";
				for (std::size_t k = 1; k <= j; k++)
				{
					std::size_t i = j - k;
					//std::cout << "[" << i << ";" << j<< "] ";
					row[k] = (row[k - 1] - last_row[k - 1]) / (points[j].first - points[i].first);
					//std::cout << row[k] << " ";
				}
				//std::cout << "\n";
				last_row = row;
				coeffs.push_back(row[j]);
			}
			return coeffs;
		}

		static FloatT eval(const std::vector<std::pair<FloatT, FloatT>>& points, const std::vector<FloatT>& coeffs, FloatT x)
		{
			if (coeffs.empty()) { throw std::invalid_argument("coefficients vector cannot be empty"); }
			if (coeffs.size() != points.size()) { throw std::invalid_argument("mismatch in vector lengths"); }
			FloatT y = 0;
			for (std::size_t i = coeffs.size() - 1; i > 0; i--) { y = (y + coeffs[i]) * (x - points[i - 1].first); }
			return y + coeffs[0];
		}
	};
}
