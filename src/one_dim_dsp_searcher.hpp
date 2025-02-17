#ifndef ONE_DIM_DSP_SEARCHER_HPP_
#define ONE_DIM_DSP_SEARCHER_HPP_

#include <memory>
#include <vector>
#include <unordered_set>

#include "dsice_search_space.hpp"
#include "coordinate_line.hpp"
#include "observed_d_spline.hpp"
#include "dsice_database_core.hpp"
#include "dsice_search_engine.hpp"

namespace dsice {

	// TODO: suggestBasedOnApproximation() のたびに推奨リストが変わるため，データベースにログを記録する必要がある

	/// <summary>
	/// 狭義の (3 分割点の実測を確認しない) 等間隔 1 次元 d-Spline 探索機構
	/// </summary>
	class OneDimDspSearcher final : public SearchEngine {
	private:
		/// <summary>
		/// 対象座標を保持し，探索空間と d-Spline インデックスを相互変換
		/// </summary>
		CoordinateLine line;
		/// <summary>
		/// 近似に使用する d-Spline
		/// </summary>
		std::shared_ptr<ObservedDSpline> d_spline;
		/// <summary>
		/// 探索すべき座標の一覧 (探索優先度順)
		/// </summary>
		coordinate_list suggest_target;
		/// <summary>
		/// 探索済点の座標
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
		/// d-Spline 近似結果にしたがって探索対象をリストに追加する
		/// </summary>
		void suggestBasedOnApproximation(void);
	public:
		/// <summary>
		/// 直線上の 1 点と方向を指定するコンストラクタ
		/// </summary>
		/// <param name="search_database">データベース</param>
		/// <param name="example_coordinate">直線上の 1 点</param>
		/// <param name="direction_info">方向情報</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		/// <param name="alpha">近似関数の滑らかさを決定する重み係数 (小さいほどデータ追随，値が大きいほど滑らか)</param>
		OneDimDspSearcher(std::shared_ptr<const DatabaseCore> search_database, const coordinate& example_coordinate, const DirectionLine& direction_info, bool low_value_is_better = true, double alpha = 0.1);
		/// <summary>
		/// 直線上の 2 点を指定するコンストラクタ
		/// </summary>
		/// <param name="search_database">データベース</param>
		/// <param name="coordinate1">直線上の 1 点</param>
		/// <param name="coordinate2">直線上で coordinate1 とは異なる 1 点</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		/// <param name="alpha">近似関数の滑らかさを決定する重み係数 (小さいほどデータ追随，値が大きいほど滑らか)</param>
		OneDimDspSearcher(std::shared_ptr<const DatabaseCore> search_database, const coordinate& coordinate1, const coordinate& coordinate2, bool low_value_is_better = true, double alpha = 0.1);
		/// <summary>
		/// 直線情報を直接指定するコンストラクタ
		/// </summary>
		/// <param name="search_database">データベース</param>
		/// <param name="line_info">直線情報</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		/// <param name="alpha">近似関数の滑らかさを決定する重み係数 (小さいほどデータ追随，値が大きいほど滑らか)</param>
		OneDimDspSearcher(std::shared_ptr<const DatabaseCore> search_database, const CoordinateLine& line_info, bool low_value_is_better = true, double alpha = 0.1);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		OneDimDspSearcher(const OneDimDspSearcher& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~OneDimDspSearcher(void);
		/// <summary>
		/// 直線の構成情報にアクセス
		/// </summary>
		/// <returns>直線の構成情報</returns>
		const CoordinateLine& getLineInfomation(void) const;
		/// <summary>
		/// 内部の d-Spline にアクセス
		/// </summary>
		/// <returns>d-Spline</returns>
		std::shared_ptr<const ObservedDSpline> getUsedDSpline(void) const;
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
		/// 対象内に事前に探索された点があった場合，その探索済点も反映されます．
		/// </summary>
		/// <returns>実測されたうち，性能評価値が最良だった性能パラメタ設定値 (座標)</returns>
		const coordinate& getBestMeasuredCoordinate(void) const override;
	};
}

#endif // !ONE_DIM_DSP_SEARCHER_HPP_
