#ifndef AROUND_POINTS_HPP_
#define AROUND_POINTS_HPP_

#include <vector>

#include "dsice_search_space.hpp"

namespace dsice {

	// TODO: コンストラクタでの周辺点列挙が非常に重い処理のため要検討．手元では 10 パラメタ 10 方向で 0.5 秒

	/// <summary>
	/// ある点の周辺点 (各軸方向距離が 1 以内の点) の一覧
	/// </summary>
	class AroundPoints final {
	private:
		/// <summary>
		/// 周辺点の一覧
		/// </summary>
		coordinate_list around_points;
		/// <summary>
		/// 各周辺点に影響する軸 (指定点から変化している軸) の数一覧
		/// </summary>
		std::vector<std::size_t> changed_axis_nums;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="space">空間の定義</param>
		/// <param name="target_coordinate">基点となる座標</param>
		/// <param name="max_changed_axis_num">同時変化させる軸数の最大値</param>
		AroundPoints(const space_size& space, const coordinate& target_coordinate, std::size_t max_changed_axis_num);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		AroundPoints(const AroundPoints& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~AroundPoints(void);
		/// <summary>
		/// 周辺点一覧を取得
		/// </summary>
		/// <returns>周辺点一覧</returns>
		const coordinate_list& getAroundPoints(void) const;
		/// <summary>
		/// getAroundPoints() で取得した周辺点がそれぞれ基点座標からいくつの軸を変化させているのかを取得
		/// </summary>
		/// <returns>同時変化軸数の一覧</returns>
		const std::vector<std::size_t>& getChangedAxisNums(void) const;
	};
}

#endif // !AROUND_POINTS_HPP_
