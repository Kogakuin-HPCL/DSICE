#ifndef DSICE_DATABASE_LOGGING_EXTENSION_HPP_
#define DSICE_DATABASE_LOGGING_EXTENSION_HPP_

#include <vector>

#include "dsice_search_space.hpp"
#include "direction_line.hpp"
#include "dsice_database_standard_functions.hpp"
#include "sampling_log.hpp"
#include "suggest_group_log.hpp"
#include "base_point_log.hpp"

namespace dsice {

	/// <summary>
	/// DSICE のデータベース型におけるログ機能を定義．<br/>
	/// ログ提供機能のみが定義され，記録の方法自体は実装に依存します (ログとは無関係な関数内で記録することが想定されるため)．
	/// </summary>
	class DatabaseLoggingExtension : virtual public DatabaseStandardFunctions {
	public:
		/// <summary>
		/// 歴代探索基準点の一覧を取得
		/// </summary>
		/// <returns>歴代探索基準点の一覧</returns>
		virtual coordinate_list getBaseCoordinateListLog(void) const = 0;
		/// <summary>
		/// 歴代の探索候補リストの一覧を取得
		/// </summary>
		/// <returns>歴代の探索候補リストの一覧</returns>
		virtual std::vector<coordinate_list> getSuggestedCoordinateListsLog(void) const = 0;
		/// <summary>
		/// 実測データ (パラメタ設定値と性能評価値の組) の一覧を取得
		/// </summary>
		/// <returns>実測データの一覧</returns>
		virtual std::vector<dsice_measure_data> getMeasuredDataLog(void) const = 0;
		/// <summary>
		/// 基準点，実測候補，実測データの全ログデータを取得
		/// </summary>
		/// <returns>全ログデータ</returns>
		virtual const std::vector<BasePointLog>& getAllMeasuringLog(void) const = 0;
	};
}

#endif // !DSICE_DATABASE_LOGGING_EXTENSION_HPP_
