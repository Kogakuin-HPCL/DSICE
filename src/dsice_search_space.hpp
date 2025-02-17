#ifndef DSICE_SEARCH_SPACE_HPP_
#define DSICE_SEARCH_SPACE_HPP_

#include <memory>
#include <vector>
#include "dsice_metric.hpp"

namespace dsice {

	/// <summary>
	/// 探索空間の次元数 (性能パラメタの数) を表現
	/// </summary>
	using dimension_size = std::size_t;

	/// <summary>
	/// 探索空間の座標位置 (インデックス) を表現
	/// </summary>
	using coordinate = std::vector<std::size_t>;

	/// <summary>
	/// 探索空間における座標のリストを表現
	/// </summary>
	using coordinate_list = std::vector<coordinate>;

	/// <summary>
	/// 探索空間のサイズ (各パラメタが取りうる値の数のリスト) を表現
	/// </summary>
	using space_size = std::vector<std::size_t>;

	/// <summary>
	/// DSICE における標本点情報を表現
	/// </summary>
	using dsice_sample = std::pair<coordinate, std::shared_ptr<Metric>>;

	/// <summary>
	/// DSICE の実測データを表現
	/// </summary>
	using dsice_measure_data = std::pair<coordinate, double>;
}

namespace std {

	/// <summary>
	/// dsice::coordinate 型を検索キーで使うためのハッシュ定義
	/// </summary>
	template<>
	class hash<dsice::coordinate> {
	public:
		size_t operator()(const dsice::coordinate& v) const {
			std::hash<std::size_t> hasher;
			size_t seed = 0;
			for (std::size_t i : v) {
				seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};
}

#endif // !DSICE_SEARCH_SPACE_HPP_
