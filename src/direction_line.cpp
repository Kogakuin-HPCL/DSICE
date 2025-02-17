#include <numeric>
#include <stdexcept>

#include "dsice_search_space.hpp"
#include "direction_line.hpp"

namespace dsice {

	// TODO: vector で方向情報を指定したときに最小距離単位を格納していない点について検討

	DirectionLine::DirectionLine(const std::vector<char>& direction) {

		long long direction_sign = 0;
		
		for (long long d : direction) {

			// 順方向を抽出するための符号を取得
			if (direction_sign == 0 && d != 0) {
				if (d > 0) {
					direction_sign = 1;
				}
				else {
					direction_sign = -1;
				}
			}

			this->direction.push_back(d * direction_sign);
			this->reverse.push_back(d * -direction_sign);

			if (d != 0) {
				this->dimension++;
			}
		}
	}

	DirectionLine::DirectionLine(const std::vector<short>& direction) {

		long long direction_sign = 0;

		for (long long d : direction) {

			// 順方向を抽出するための符号を取得
			if (direction_sign == 0 && d != 0) {
				if (d > 0) {
					direction_sign = 1;
				}
				else {
					direction_sign = -1;
				}
			}

			this->direction.push_back(d * direction_sign);
			this->reverse.push_back(d * -direction_sign);

			if (d != 0) {
				this->dimension++;
			}
		}
	}

	DirectionLine::DirectionLine(const std::vector<int>& direction) {

		long long direction_sign = 0;

		for (long long d : direction) {

			// 順方向を抽出するための符号を取得
			if (direction_sign == 0 && d != 0) {
				if (d > 0) {
					direction_sign = 1;
				}
				else {
					direction_sign = -1;
				}
			}

			this->direction.push_back(d * direction_sign);
			this->reverse.push_back(d * -direction_sign);

			if (d != 0) {
				this->dimension++;
			}
		}
	}

	DirectionLine::DirectionLine(const std::vector<long long>& direction) {

		long long direction_sign = 0;

		for (long long d : direction) {

			// 順方向を抽出するための符号を取得
			if (direction_sign == 0 && d != 0) {
				if (d > 0) {
					direction_sign = 1;
				}
				else {
					direction_sign = -1;
				}
			}

			this->direction.push_back(d * direction_sign);
			this->reverse.push_back(d * -direction_sign);

			if (d != 0) {
				this->dimension++;
			}
		}
	}

	DirectionLine::DirectionLine(const coordinate& coordinate1, const coordinate& coordinate2) {

		if (coordinate1.size() != coordinate2.size()) {
			throw std::invalid_argument("The dimension of two coordinates must be the same.");
		}

		if (coordinate1.size() == 0) {
			return;
		}

		// 方向情報を求めるために必要な要素を準備
		long long direction_sign = 0;
		long long coordinate_gcd = 0;
		for (std::size_t i = 0; i < coordinate1.size(); i++) {

			// 単純な差分 (最終的な方向情報の自然数倍)
			this->direction.push_back((long long)coordinate2[i] - (long long)coordinate1[i]);

			if (this->direction[i] != 0) {

				// 順方向を取得するための符号
				if (direction_sign == 0) {
					if (this->direction[i] > 0) {
						direction_sign = 1;
					}
					else {
						direction_sign = -1;
					}
				}

				// 最小距離単位で見た時の距離 (2 点間にある離散点の数 + 1)
				if (coordinate_gcd == 0) {
					coordinate_gcd = std::abs(this->direction[i]);
				}
				else {
					coordinate_gcd = std::gcd(coordinate_gcd, std::abs(this->direction[i]));
				}
			}
		}

		// 全要素 0 (2 点が一致) なら終了してゼロ除算を避ける
		if (coordinate_gcd == 0) {

			for (std::size_t i = 0; i < this->direction.size(); i++) {
				this->reverse.push_back(0);
			}

			return;
		}

		// 最大公約数で割れば最小離散距離
		for (std::size_t i = 0; i < this->direction.size(); i++) {
			this->direction[i] /= coordinate_gcd * direction_sign;
			this->reverse.push_back(-this->direction[i]);
		}

		for (long long d : this->direction) {
			if (d != 0) {
				this->dimension++;
			}
		}
	}

	DirectionLine::DirectionLine(const DirectionLine& original) {
		this->direction = original.direction;
		this->reverse = original.reverse;
		this->dimension = original.dimension;
	}

	DirectionLine::~DirectionLine(void) {
		// DO_NOTHING
	}

	bool DirectionLine::operator ==(const DirectionLine& other) const {

		if (this->direction.size() != other.direction.size()) {
			return false;
		}

		for (std::size_t i = 0; i < this->direction.size(); i++) {
			if (this->direction[i] != other.direction[i]) {
				return false;
			}
		}

		return true;
	}

	bool DirectionLine::operator !=(const DirectionLine& other) const {

		if (this->direction.size() != other.direction.size()) {
			return true;
		}

		for (std::size_t i = 0; i < this->direction.size(); i++) {
			if (this->direction[i] != other.direction[i]) {
				return true;
			}
		}

		return false;
	}

	const std::vector<long long>& DirectionLine::getDirection(void) const {
		return this->direction;
	}

	const std::vector<long long>& DirectionLine::getReverse(void) const {
		return this->reverse;
	}

	bool DirectionLine::hasDimension(void) const {
		return this->dimension != 0;
	}

	std::size_t DirectionLine::getDirectionDimension(void) const {
		return this->dimension;
	}
}