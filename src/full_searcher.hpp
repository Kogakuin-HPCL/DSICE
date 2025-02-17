#ifndef FULL_SEARCHER_HPP_
#define FULL_SEARCHER_HPP_

#include <memory>
#include <vector>
#include <unordered_set>

#include "dsice_search_space.hpp"
#include "dsice_database_core.hpp"
#include "dsice_search_engine.hpp"

namespace dsice {

	/// <summary>
	/// 全探索機構
	/// </summary>
	class FullSearcher final : public SearchEngine {
	private:
		/// <summary>
		/// 探索対象
		/// </summary>
		coordinate_list search_target;
		/// <summary>
		/// 実測するように提案する対象
		/// </summary>
		coordinate_list suggested_targets;
		/// <summary>
		/// 探索済点
		/// </summary>
		std::unordered_set<coordinate> measured_coordinates;
		/// <summary>
		/// 低い値ほど良いかどうか
		/// </summary>
		bool lower_is_better;
		/// <summary>
		/// 実測データの更新用バッファ (座標)
		/// </summary>
		std::vector<coordinate> measured_coordinate_buffer;
		/// <summary>
		/// 実測データの更新用バッファ (性能評価値)
		/// </summary>
		std::vector<double> measured_metric_value_buffer;
		/// <summary>
		/// 基準点を含む全点の中で最良点の座標
		/// </summary>
		coordinate best_judged_point_coordinate;
		/// <summary>
		/// この機構による実測済の中で最良点の座標
		/// </summary>
		// best_judged_point_coordinate に追従するが，完全未探索状態の場合のみ空 coordinate を返す特殊挙動になるため区別
		coordinate best_measured_point_coordinate;
		/// <summary>
		/// 最良点の値
		/// </summary>
		double best_point_value;
		/// <summary>
		/// 指定点の探索が終わったか
		/// </summary>
		bool is_finish_searching = false;
	public:
		/// <summary>
		/// 探索空間全体を全探索するコンストラクタ
		/// </summary>
		/// <param name="search_database">使用するデータベース</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		// TODO: 対象全列挙の実装が意外と面倒なのでいったんやめた
		//FullSearcher(std::shared_ptr<DatabaseStandardFunctions> search_database, bool low_value_is_better = true);
		
		/// <summary>
		/// 指定した座標一覧を全探索するコンストラクタ
		/// </summary>
		/// <param name="target_coordinates">全探索する座標一覧</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		FullSearcher(const coordinate_list& target_coordinates, bool low_value_is_better = true);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		FullSearcher(const FullSearcher& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~FullSearcher(void);
		/// <summary>
		/// 次に実測すべき性能パラメタ性能値 (座標) を取得
		/// </summary>
		/// <returns>次に実測すべき性能パラメタ性能値 (座標)</returns>
		const coordinate& getSuggested(void) const override;
		/// <summary>
		/// 実測すべき性能パラメタ性能値 (座標) のリストを取得
		/// </summary>
		/// <returns>実測すべき性能パラメタ性能値 (座標) のリスト</returns>
		const coordinate_list& getSuggestedList(void) const override;
		/// <summary>
		/// 実測データをデータベースと次回更新用バッファに記録
		/// </summary>
		/// <param name="measured_coordinate">実測した座標</param>
		/// <param name="metric_value">実測した値</param>
		void setMetricValue(const coordinate& measured_coordinate, double metric_value) override;
		/// <summary>
		/// 記録されている実測データを用いて状態を更新
		/// </summary>
		/// <returns>更新用バッファにデータがあり，更新されたら true</returns>
		bool updateState(void) override;
		/// <summary>
		/// 探索アルゴリズムが終了条件を満たしたかを取得．<br/>
		/// 終了条件を持たない探索アルゴリズムの場合，この関数の挙動は各実装クラス依存です．
		/// </summary>
		/// <returns>終了条件を満たしていれば true</returns>
		bool isSearchFinished(void) const override;
		/// <summary>
		/// 探索機構が探索対象とする座標リストを取得
		/// </summary>
		/// <returns>探索機構が探索対象とする座標リスト</returns>
		const coordinate_list& getTargetCoordinate(void) const override;
		/// <summary>
		/// 対象座標内で探索済の座標リストを取得
		/// </summary>
		/// <returns>対象座標内で探索済の座標リスト</returns>
		const std::unordered_set<coordinate>& getMeasuredCoordinates(void) const override;
		/// <summary>
		/// 探索アルゴリズムが最良と判定した性能パラメタ設定値 (座標) を取得
		/// </summary>
		/// <returns>探索アルゴリズムが最良と判定した性能パラメタ設定値 (座標)</returns>
		const coordinate& getBestJudgedCoordinate(void) const override;
		/// <summary>
		/// 探索アルゴリズムによって実測されたうち，性能評価値が最良だった性能パラメタ設定値 (座標) を取得．<br/>
		/// 対象内に事前に探索された点があった場合，その探索済点は反映されません．
		/// </summary>
		/// <returns>実測されたうち，性能評価値が最良だった性能パラメタ設定値 (座標)</returns>
		const coordinate& getBestMeasuredCoordinate(void) const override;
	};
}

#endif // !FULL_SEARCHER_HPP_
