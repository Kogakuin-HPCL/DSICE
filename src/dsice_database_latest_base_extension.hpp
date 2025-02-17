#ifndef DSICE_DATABASE_LATEST_BASE_EXTENSION_HPP_
#define DSICE_DATABASE_LATEST_BASE_EXTENSION_HPP_

#include "dsice_search_space.hpp"
#include "dsice_database_core.hpp"

namespace dsice {

	/// <summary>
	/// DSICE のデータベース型に登録された最新基準点データに関する機能を定義
	/// </summary>
	class DatabaseLatestBaseExtension : virtual public DatabaseCore {
	public:
		/// <summary>
		/// 現在の基準点の座標を取得
		/// </summary>
		/// <returns>現在の基準点の座標</returns>
		virtual const coordinate& getLatestBaseCoordinate(void) const = 0;
		/// <summary>
		/// 直近で基準点が登録された際，基準点は変化していたかを取得
		/// </summary>
		/// <returns>変化した (直近基準点がその前の基準点と異なる座標の) 場合は true</returns>
		virtual bool hasBaseCoordinateChanged(void) const = 0;
	};
}

#endif // !DSICE_DATABASE_LATEST_BASE_EXTENSION_HPP_
