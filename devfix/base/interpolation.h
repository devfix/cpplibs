//
// Created by core on 6/8/20.
//

#include <vector>

namespace devfix::base
{

	template<typename FloatT>
	struct interpolation
	{
		/**
		 * \brief Calculates the coefficients of an Newton polynomial interpolation
		 * \param points the
		 * \return coefficients
		 */
		static std::vector<FloatT> calc_coeffs(const std::vector<std::pair<FloatT, FloatT>>& points)
		{
			if (points.empty()) { throw std::invalid_argument("points vector cannot be empty"); }
			std::vector<FloatT> coeffs(points.size());
			std::vector<FloatT> last_row;
			std::vector<FloatT> row(points.size());
			for (std::size_t j = 0; j < points.size(); j++)
			{
				row[0] = points[j].second;
				for (std::size_t k = 1; k <= j; k++) { row[k] = (row[k - 1] - last_row[k - 1]) / (points[j].first - points[j - k].first); }
				coeffs[j] = row[j];
				last_row = row;
			}
			return coeffs;
		}

		/**
		 * \brief evaluates the polynomial by an argument
		 * \param points of interpolation
		 * \param coeffs of interpolation
		 * \param x argument for evaluation
		 * \return value of polynomial at x
		 */
		static FloatT eval(const std::vector<std::pair<FloatT, FloatT>>& points, const std::vector<FloatT>& coeffs, FloatT x)
		{
			if (coeffs.empty()) { throw std::invalid_argument("coefficients vector cannot be empty"); }
			if (coeffs.size() != points.size()) { throw std::invalid_argument("mismatch in vector lengths"); }
			FloatT y = 0;
			for (std::size_t i = coeffs.size() - 1; i > 0; i--) { y = (y + coeffs[i]) * (x - points[i - 1].first); }
			return y + coeffs[0];
		}

		/**
		 * \brief evaluates the polynomial by an argument
		 * \param xs x values of interpolation
		 * \param coeffs of interpolation
		 * \param x argument for evaluation
		 * \return value of polynomial at x
		 */
		static FloatT eval(const std::vector<FloatT>& xs, const std::vector<FloatT>& coeffs, FloatT x)
		{
			if (coeffs.empty()) { throw std::invalid_argument("coefficients vector cannot be empty"); }
			if (coeffs.size() != xs.size()) { throw std::invalid_argument("mismatch in vector lengths"); }
			FloatT y = 0;
			for (std::size_t i = coeffs.size() - 1; i > 0; i--) { y = (y + coeffs[i]) * (x - xs[i - 1]); }
			return y + coeffs[0];
		}

		/**
		 * \brief Finds the argument for a given value y
		 * \param points of interpolation
		 * \param coeffs of interpolation
		 * \param x_low start of search boundary
		 * \param x_up end of search boundary
		 * \param y value to search argument for
		 * \param abs_error
		 * \return found x argument
		 */
		static FloatT bisec(const std::vector<std::pair<FloatT, FloatT>>& points, const std::vector<FloatT>& coeffs,
			FloatT x_low, FloatT x_up, FloatT y, FloatT abs_error)
		{
			return basic_bisec<decltype(points)>(points, coeffs, x_low, x_up, y, abs_error);
		}

		/**
		 * \brief Finds the argument for a given value y
		 * \param xs x values of interpolation
		 * \param coeffs of interpolation
		 * \param x_low start of search boundary
		 * \param x_up end of search boundary
		 * \param y value to search argument for
		 * \param abs_error desired precision for x
		 * \return found x argument
		 */
		static FloatT bisec(const std::vector<FloatT>& xs, const std::vector<FloatT>& coeffs,
			FloatT x_low, FloatT x_up, FloatT y, FloatT abs_error)
		{
			return basic_bisec<decltype(xs)>(xs, coeffs, x_low, x_up, y, abs_error);
		}

	private:
		template<typename PointsT>
		static FloatT basic_bisec(const PointsT& points, const std::vector<FloatT>& coeffs,
			FloatT x_low, FloatT x_up, FloatT y, FloatT abs_error)
		{
			FloatT y_low = eval(points, coeffs, x_low) - y;
			FloatT y_high = eval(points, coeffs, x_up) - y;
			if ((y_low * y_high > 0) || (std::abs(y_high - y_low) <= abs_error))
			{
				throw std::invalid_argument("bad bisection boundaries");
			}

			FloatT x_mid, y_mid;
			std::size_t k = 0;  // safety variable to detect infinite loop
			if (y_low < y_high)
			{
				do
				{
					x_mid = (x_low + x_up) / 2;
					y_mid = eval(points, coeffs, x_mid);
					if (y_mid > y) { x_up = x_mid; }
					else { x_low = x_mid; }
					if (k++ >= sizeof(double) * 8) { throw std::runtime_error("max bisection regression depth reached"); }
				} while (std::abs(y_mid - y) > abs_error);
			}
			else
			{
				do
				{
					x_mid = (x_low + x_up) / 2;
					y_mid = eval(points, coeffs, x_mid);
					if (y_mid < y) { x_up = x_mid; }
					else { x_low = x_mid; }
					if (k++ >= sizeof(double) * 8) { throw std::runtime_error("max bisection regression depth reached"); }
				} while (std::abs(y_mid - y) > abs_error);
			}
			return x_mid;
		}
	};
}
