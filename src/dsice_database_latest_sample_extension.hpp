#ifndef DSICE_DATABASE_LATEST_SAMPLE_EXTENSION_HPP_
#define DSICE_DATABASE_LATEST_SAMPLE_EXTENSION_HPP_

#include "dsice_search_space.hpp"
#include "dsice_database_core.hpp"

namespace dsice {

	/// <summary>
	/// DSICE のデータベース型に登録された最新標本データに関する機能を定義
	/// </summary>
	class DatabaseLatestSampleExtension : virtual public DatabaseCore {
	public:
		/// <summary>
		/// ループの終わりを記録
		/// </summary>
		virtual void setLoopEnd(void) = 0;
		/// <summary>
		/// 直近ループで登録した標本データの座標一覧を取得
		/// </summary>
		/// <returns>直近に登録した標本データの値</returns>
		virtual const std::vector<coordinate>& getLatestSampleCoordinate(void) const = 0;
		/// <summary>
		/// 直近ループで登録した性能評価値一覧を取得
		/// </summary>
		/// <returns>直近に登録した性能評価値</returns>
		virtual const std::vector<double>& getLatestMeasuredValue(void) const = 0;
	};
}

#endif // !DSICE_DATABASE_LATEST_SAMPLE_EXTENSION_HPP_
