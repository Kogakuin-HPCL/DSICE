#include <vector>
#include <cmath>

#include "d_spline_core.hpp"

namespace dsice {

	void DSplineCore::givensRotate(std::size_t target_col, double& appended_row1, double& appended_row2, double& appended_b) {

		double target_row_Z1 = this->Z[3 * target_col];
		double target_row_Z2 = this->Z[3 * target_col + 1];
		double target_row_Z3 = this->Z[3 * target_col + 2];

		// 回転角度を求める
		double r = std::sqrt(target_row_Z1 * target_row_Z1 + appended_row1 * appended_row1);
		double c = target_row_Z1 / r;	// cos
		double s = appended_row1 / r;	// sin

		// Z の計算
		this->Z[3 * target_col] = c * target_row_Z1 + s * appended_row1;
		this->Z[3 * target_col + 1] = c * target_row_Z2 + s * appended_row2;
		this->Z[3 * target_col + 2] = c * target_row_Z3;
		appended_row1 = -s * target_row_Z2 + c * appended_row2;
		appended_row2 = -s * target_row_Z3;

		// b の計算
		double target_b = b[target_col];
		b[target_col] = c * target_b + s * appended_b;
		appended_b = -s * target_b + c * appended_b;
	}

	void DSplineCore::updateZAndB(std::size_t index, double value) {

		// データを追加する座標を表す「1」 (理論における行列 E の非ゼロ要素)
		double appended_row1 = 1.0;
		double appended_row2 = 0.0;

		// 追加行が消えるまで Z の対角要素をたどって QR 分解
		for (std::size_t target_col = index; target_col < this->marker_num; target_col++) {

			this->givensRotate(target_col, appended_row1, appended_row2, value);

			// TODO: 誤差の影響がある気がするので閾値比較の方が良いのでは？要検討
			if (appended_row1 == 0) {
				if (appended_row2 == 0) {
					break;	// // 追加行が 0 になれば QR 分解が完了しているため終了
				}
				else {
					// appended_row1 が 0.0 なら 1 回スキップ可能
					appended_row1 = appended_row2;
					appended_row2 = 0.0;
					target_col++;
				}
			}
		}
	}

	void DSplineCore::updateF(void) {

		for (std::size_t i = 0; i < this->marker_num; i++) {
			this->f[i] = this->b[i];
		}

		// 最後の行が埋まるまで (最低 2 データ入力されるまで) のゼロ割を回避
		if (this->Z[3 * (this->marker_num - 1)] == 0.0) {
			return;
		}

		// 後退代入
		for (std::size_t i = 1; i <= this->marker_num; i++) {

			// size_t (unsigned 型) で逆順走査するための変換
			std::size_t idx_i = this->marker_num - i;

			this->f[idx_i] /= this->Z[3 * idx_i];

			if (idx_i > 0) {

				std::size_t idx_j = idx_i - 1;

				this->f[idx_j] -= this->Z[3 * idx_j + 1] * this->f[idx_i];
			}

			if (idx_i > 1) {

				std::size_t idx_j = idx_i - 2;

				this->f[idx_j] -= this->Z[3 * idx_j + 2] * this->f[idx_i];
			}
		}
	}

	DSplineCore::DSplineCore(std::size_t value_num, double alpha) {

		this->marker_num = value_num;

		// α = 0 だと全点を忠実に繋ごうとする (全点のデータが必要になる)
		// テキトーな値でゼロ除算を避ける
		if (alpha == 0) {
			alpha = 1e-10;
		}

		this->Z = std::vector<double>(value_num * 3, 0);
		this->b = std::vector<double>(value_num, 0);
		this->f = std::vector<double>(value_num, 0);

		for (std::size_t i = 0; i < this->marker_num - 2; i++) {
			this->Z[i * 3] = alpha;
			this->Z[i * 3 + 1] = -2 * alpha;
			this->Z[i * 3 + 2] = alpha;
		}
	}

	DSplineCore::DSplineCore(const DSplineCore& original) {
		this->marker_num = original.marker_num;
		this->Z = original.Z;
		this->b = original.b;
		this->f = original.f;
	}

	DSplineCore::~DSplineCore(void) {
		// DO_NOTHING
	}

	void DSplineCore::update(std::size_t target_index, double value) {
		this->updateZAndB(target_index, value);
		this->updateF();
	}

	void DSplineCore::update(const std::pair<std::size_t, double>& data) {
		this->updateZAndB(data.first, data.second);
		this->updateF();
	}

	void DSplineCore::update(const std::vector<std::pair<std::size_t, double>>& data) {

		for (const std::pair<std::size_t, double>& one_data : data) {
			this->updateZAndB(one_data.first, one_data.second);
		}

		this->updateF();
	}

	std::size_t DSplineCore::getMarkerNum(void) const {
		return this->marker_num;
	}

	const std::vector<double>& DSplineCore::getZ(void) const {
		return this->Z;
	}

	const std::vector<double>& DSplineCore::getB(void) const {
		return this->b;
	}

	const std::vector<double>& DSplineCore::getF(void) const {
		return this->f;
	}
}