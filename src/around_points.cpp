#include <vector>
#include <bitset>
#include <stdexcept>

#include "dsice_search_space.hpp"
#include "around_points.hpp"

namespace dsice {

	AroundPoints::AroundPoints(const space_size& space, const coordinate& target_coordinate, std::size_t max_changed_axis_num) {

		// Memo : 
		// 再帰呼び出しアルゴリズムだと変化する軸数を増やしながらやると n * 3^n
		// 事前列挙 + ソートだと 3^n * log(3^n) = だいたい 3^n * n でオーダー変わらないっぽい
		// コードを読む際に分かりやすいよう，ここでは事前列挙 + ソート形式で実装

		// Memo :
		// 3 進数でのビット全探索だと列挙順序が直感に反する．
		// 2 進数のビット全探索を 2 重にやる (外側で立っているビットに対してのみさらにビット全探索) としても総ループ数に変化はないためこちらで実装
		// 2 進 2 重方式なら軸指定→方向指定になる (と思われる) ので直感通り．

		std::size_t parameters_num = space.size();

		if (max_changed_axis_num > parameters_num) {
			throw std::invalid_argument("max_changed_axis_num must be less than the number of parameters.");
		}

		// 変化軸数でソートするためのバケット準備
		std::vector<coordinate_list> coordinates_bucket;
		for (std::size_t i = 0; i <= max_changed_axis_num; i++) {
			coordinates_bucket.emplace_back();
		}

		// 変更する軸をビット全探索
		for (size_t i = 1; i < (static_cast<std::size_t>(1) << parameters_num); i++) {

			// bitset のビット長は定数式指定のため，ここでは決め打ちが必要
			// i 自体が 64 bit なので 64 でとる
			constexpr std::size_t MAX_BIT = 64;
			std::bitset<MAX_BIT> changed_axis(i);

			// 最大軸変化数を超えてしまう場合は中断して枝刈り
			if (changed_axis.count() > max_changed_axis_num) {
				continue;
			}

			// 各軸の変化方向をビット全探索
			for (std::size_t j = 0; j < (static_cast<std::size_t>(1) << changed_axis.count()); j++) {

				// マイナス方向の場合にビットがたつ
				std::bitset<MAX_BIT> move_minus(j);

				// 座標の各ビットを確認し，移動方向の点 tmp_around_coordinate を生成
				std::vector<std::size_t> tmp_around_coordinate;
				std::size_t target_changed_bit = 0;
				for (std::size_t k = 0; k < parameters_num; k++) {

					// 移動する軸は移動可能かどうかをチェック
					if (changed_axis[k] == 1) {

						// 基点座標が 0 ならマイナス移動できない
						if (move_minus[target_changed_bit] == 1 && target_coordinate[k] == 0) {
							break;
						}

						// 基点座標が空間の最大端だとプラス移動できない
						if (move_minus[target_changed_bit] == 0 && (target_coordinate[k] + 1) == space[k]) {
							break;
						}

						long long moved = static_cast<long long>(changed_axis[k]) - 2 * static_cast<long long>(move_minus[target_changed_bit]);
						tmp_around_coordinate.push_back(target_coordinate[k] + moved);

						target_changed_bit++;
					}
					else {
						tmp_around_coordinate.push_back(target_coordinate[k]);
					}
				}

				// 移動後座標が生成できた場合はバケットに追加
				if (tmp_around_coordinate.size() == target_coordinate.size()) {
					coordinates_bucket[changed_axis.count()].push_back(tmp_around_coordinate);
				}
			}
		}

		// 生成された座標を，軸変化数順に追加
		for (std::size_t i = 1; i <= max_changed_axis_num; i++) {
			for (const coordinate& c : coordinates_bucket[i]) {
				this->around_points.push_back(c);
				this->changed_axis_nums.push_back(i);
			}
		}
	}

	AroundPoints::AroundPoints(const AroundPoints& original) {
		this->around_points = original.around_points;
		this->changed_axis_nums = original.changed_axis_nums;
	}

	AroundPoints::~AroundPoints(void) {
		// DO_NOTHING
	}

	const coordinate_list& AroundPoints::getAroundPoints(void) const {
		return this->around_points;
	}

	const std::vector<std::size_t>& AroundPoints::getChangedAxisNums(void) const {
		return this->changed_axis_nums;
	}
}