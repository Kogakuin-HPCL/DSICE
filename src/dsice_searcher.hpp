#ifndef DSICE_SEARCHER_HPP_
#define DSICE_SEARCHER_HPP_

#include <memory>
#include <unordered_set>

#include "dsice_search_space.hpp"

namespace dsice {

	/// <summary>
	/// 探索対象に対し探索優先順位をつける，探索アルゴリズムの実態
	/// </summary>
	class Searcher {
	public:
		/// <summary>
		/// 次に実測すべき性能パラメタ性能値 (座標) を取得
		/// </summary>
		/// <returns>次に実測すべき性能パラメタ性能値 (座標)</returns>
		virtual const coordinate& getSuggested(void) const = 0;
		/// <summary>
		/// 実測すべき性能パラメタ性能値 (座標) のリストを取得
		/// </summary>
		/// <returns>実測すべき性能パラメタ性能値 (座標) のリスト</returns>
		virtual const coordinate_list& getSuggestedList(void) const = 0;
		/// <summary>
		/// 実測データをデータベースと次回更新用バッファに記録
		/// </summary>
		/// <param name="measured_coordinate">実測した座標</param>
		/// <param name="metric_value">実測した値</param>
		virtual void setMetricValue(const coordinate& measured_coordinate, double metric_value) = 0;
		/// <summary>
		/// 記録されている実測データを用いて状態を更新
		/// </summary>
		/// <returns>更新用バッファにデータがあり，更新されたら true</returns>
		virtual bool updateState(void) = 0;
		/// <summary>
		/// 探索アルゴリズムが終了条件を満たしたかを取得．<br/>
		/// 終了条件を持たない探索アルゴリズムの場合，この関数の挙動は各実装クラス依存です．
		/// </summary>
		/// <returns>終了条件を満たしていれば true</returns>
		virtual bool isSearchFinished(void) const = 0;
		/// <summary>
		/// 探索機構が探索対象とする座標リストを取得
		/// </summary>
		/// <returns>探索機構が探索対象とする座標リスト</returns>
		virtual const coordinate_list& getTargetCoordinate(void) const = 0;
		/// <summary>
		/// 対象座標内で探索済の座標リストを取得
		/// </summary>
		/// <returns>対象座標内で探索済の座標リスト</returns>
		virtual const std::unordered_set<coordinate>& getMeasuredCoordinates(void) const = 0;
		/// <summary>
		/// 探索アルゴリズムが最良と判定した性能パラメタ設定値 (座標) を取得
		/// </summary>
		/// <returns>探索アルゴリズムが最良と判定した性能パラメタ設定値 (座標)</returns>
		virtual const coordinate& getBestJudgedCoordinate(void) const = 0;
		/// <summary>
		/// 探索アルゴリズムによって実測されたうち，性能評価値が最良だった性能パラメタ設定値 (座標) を取得．<br/>
		/// 対象内に事前に探索された点があった場合，その探索済点も反映されます．
		/// </summary>
		/// <returns>実測されたうち，性能評価値が最良だった性能パラメタ設定値 (座標)</returns>
		virtual const coordinate& getBestMeasuredCoordinate(void) const = 0;
	};
}

#endif // !DSICE_SEARCHER_HPP_
