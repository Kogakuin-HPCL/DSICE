#ifndef COORDINATE_LINE_HPP_
#define COORDINATE_LINE_HPP_

#include <vector>
#include <unordered_map>

#include "dsice_search_space.hpp"
#include "direction_line.hpp"

namespace dsice {

	/// <summary>
	/// 探索空間上の直線
	/// </summary>
	class CoordinateLine final {
	private:
		/// <summary>
		/// 直線の方向情報
		/// </summary>
		DirectionLine direction;
		/// <summary>
		/// 直線を構成する点の座標
		/// </summary>
		std::vector<coordinate> line_coordinates;
		/// <summary>
		/// 座標から点のインデックスを逆引きするためのメモ
		/// </summary>
		std::unordered_map<coordinate, std::size_t> point_indexes;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="space_info">探索空間</param>
		/// <param name="example_coordinate">直線を構成する点の一例</param>
		/// <param name="line_info">直線の方向情報</param>
		CoordinateLine(const space_size& space_info, const coordinate& example_coordinate, const DirectionLine& line_info);
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="space_info">探索空間</param>
		/// <param name="coordinate1">直線を構成する点の 1 つめ</param>
		/// <param name="coordinate2">直線を構成する点の 2 つめ</param>
		CoordinateLine(const space_size& space_info, const coordinate& coordinate1, const coordinate& coordinate2);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		CoordinateLine(const CoordinateLine& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~CoordinateLine(void);
		/// <summary>
		/// 直線の方向情報を取得
		/// </summary>
		/// <returns>直線の方向情報</returns>
		const DirectionLine& getLineDirection(void) const;
		/// <summary>
		/// 直線を構成する点の数を取得
		/// </summary>
		/// <returns>直線を構成する点の数</returns>
		std::size_t getPointsNum(void) const;
		/// <summary>
		/// 直線を構成する点の座標一覧を取得
		/// </summary>
		/// <returns>直線を構成する点の座標一覧</returns>
		const std::vector<coordinate>& getPoints(void) const;
		/// <summary>
		/// 直線を構成する点の座標を 1 点取得
		/// </summary>
		/// <param name="point_index">指定点のインデックス</param>
		/// <returns>指定点の座標</returns>
		const coordinate& getPointCoordinate(std::size_t point_index) const;
		/// <summary>
		/// 直線を構成する点の座標インデックスを座標から取得．<br/>
		/// 直線上に存在しない座標を指定した場合の挙動は未定義です．
		/// </summary>
		/// <param name="point_coordinate">探索空間における座標</param>
		/// <returns>直線におけるインデックス</returns>
		std::size_t getPointIndex(const coordinate& point_coordinate) const;
	};
}

#endif // !COORDINATE_LINE_HPP_
