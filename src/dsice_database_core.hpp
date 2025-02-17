#ifndef DSICE_DATABASE_CORE_HPP_
#define DSICE_DATABASE_CORE_HPP_

#include "dsice_metric.hpp"
#include "dsice_search_space.hpp"
#include "dsice_database_metric_type.hpp"

namespace dsice {

	/// <summary>
	/// DSICE のデータベース型における最小要件を定義
	/// </summary>
	class DatabaseCore {
	public:
		/// <summary>
		/// 探索空間の次元数 (探索対象のパラメタ数) を取得
		/// </summary>
		/// <returns>探索空間の次元数 (探索対象のパラメタ数)</returns>
		virtual dimension_size getSpaceDimension(void) const = 0;
		/// <summary>
		/// 探索空間の大きさ (各性能パラメタの取りうる値の数) を取得
		/// </summary>
		/// <returns>探索空間の大きさ (各性能パラメタの取りうる値の数)</returns>
		virtual const space_size& getSpaceSize(void) const = 0;
		/// <summary>
		/// 標本データを登録
		/// </summary>
		/// <param name="target_coordinate">標本データの座標</param>
		/// <param name="metric_value">性能評価値</param>
		virtual void setSampleMetricValue(const coordinate& target_coordinate, double metric_value) = 0;
		/// <summary>
		/// 標本点が登録済かどうか
		/// </summary>
		/// <param name="target_coordinates">標本点の座標</param>
		/// <returns>登録済なら true</returns>
		virtual bool hasSample(const coordinate& target_coordinate) const = 0;
		/// <summary>
		/// 登録済標本点の数を取得
		/// </summary>
		/// <returns>登録済標本点の数</returns>
		virtual std::size_t getExistSamplesNum(void) const = 0;
		/// <summary>
		/// 登録済の標本の性能評価値を取得
		/// </summary>
		/// <param name="target_coordinate">取得したい標本データの座標</param>
		/// <returns>当該標本の性能評価値</returns>
		virtual double getSampleMetricValue(const coordinate& target_coordinate) const = 0;
	};
}

#endif // !DSICE_DATABASE_CORE_HPP_
