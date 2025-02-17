#ifndef DSICE_DATABASE_BASE_POINT_EXTENSION_HPP_
#define DSICE_DATABASE_BASE_POINT_EXTENSION_HPP_

#include <unordered_map>
#include <unordered_set>

#include "dsice_search_space.hpp"
#include "direction_line.hpp"
#include "dsice_database_core.hpp"

namespace dsice {

	/// <summary>
	/// DSICE のデータベース型における基準点関係機能を定義
	/// </summary>
	class DatabaseBasePointExtension : virtual public DatabaseCore {
	public:
		/// <summary>
		/// 指定座標を探索基準点として登録
		/// </summary>
		/// <param name="target_coordinates">探索基準点の座標</param>
		virtual void setBasePoint(const coordinate& target_coordinate) = 0;
		/// <summary>
		/// 基準点およびその探索済方向の一覧を取得
		/// </summary>
		/// <returns>基準点およびその探索済方向の一覧</returns>
		virtual const std::unordered_map<coordinate, std::unordered_set<DirectionLine>>& getBasePoints(void) const = 0;
		/// <summary>
		/// 指定座標が探索基準点になったことがあるか
		/// </summary>
		/// <param name="target_coordinates">調べる座標</param>
		/// <returns>過去に探索基準点となったことがあれば true</returns>
		virtual bool hasBeenBasePoint(const coordinate& target_coordinate) const = 0;
		/// <summary>
		/// 指定座標を通る直線のうち，指定直線を探索済として登録
		/// </summary>
		/// <param name="base_coordinates">基準となる探索基準点</param>
		/// <param name="line_info">探索した直線</param>
		virtual void recordSearchedDirection(const coordinate& base_coordinate, const DirectionLine& line_info) = 0;
		/// <summary>
		/// 指定座標を通る直線のうち，指定直線が探索済かどうかを判定
		/// </summary>
		/// <param name="base_coordinates">基準となる探索基準点</param>
		/// <param name="line_info">判定する直線</param>
		/// <returns>指定直線が探索済なら true</returns>
		virtual bool isSearchedDirection(const coordinate& base_coordinate, const DirectionLine& line_info) const = 0;
	};
}

#endif // !DSICE_DATABASE_BASE_POINT_EXTENSION_HPP_
