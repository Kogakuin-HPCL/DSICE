#ifndef LINE_SINGLE_TRI_SEARCHER_HPP_
#define LINE_SINGLE_TRI_SEARCHER_HPP_

#include <memory>
#include <vector>

#include "dsice_search_space.hpp"
#include "coordinate_line.hpp"
#include "dsice_search_engine.hpp"
#include "dsice_database_core.hpp"
#include "line_iterative_tri_searcher.hpp"

namespace dsice {

	/// <summary>
	/// 直線上の点を繰り返し 3 分割した点を探索するが，直線全体の 3 分割点が探索されたら終了条件を満たす機構．<br/>
	/// d-Spline 近似の準備機構であり，LineIterativeTriSearcher に対して終了条件のみを変化させたラッパとして実装される
	/// </summary>
	class LineSingleTriSearcher final : public SearchEngine {
	private:
		/// <summary>
		/// 本体
		/// </summary>
		LineIterativeTriSearcher substance;
		/// <summary>
		/// 終了条件となる 4 座標
		/// </summary>
		coordinate_list tri_points;
		/// <summary>
		/// 実測データの更新用バッファ (座標)
		/// </summary>
		std::vector<coordinate> measured_coordinate_buffer;
		/// <summary>
		/// 終了条件 4 座標を算出
		/// </summary>
		void listupTriPoints(std::shared_ptr<const DatabaseCore> database);
	public:
		/// <summary>
		/// 直線上の 1 点と方向情報から探索対象を決めるコンストラクタ
		/// </summary>
		/// <param name="database">データベース</param>
		/// <param name="example_coordinate">直線上のどれか 1 点</param>
		/// <param name="direction_info">方向情報</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		LineSingleTriSearcher(std::shared_ptr<const DatabaseCore> database, const coordinate& example_coordinate, const DirectionLine& direction_info, bool low_value_is_better = true);
		/// <summary>
		/// 直線上の 2 点から探索対象を決めるコンストラクタ
		/// </summary>
		/// <param name="database">データベース</param>
		/// <param name="coordinate1">直線上のどれか 1 点</param>
		/// <param name="coordinate2">直線上で coordinate1 と異なるどれか 1 点</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		LineSingleTriSearcher(std::shared_ptr<const DatabaseCore> database, const coordinate& coordinate1, const coordinate& coordinate2, bool low_value_is_better = true);
		/// <summary>
		/// 探索対象情報を直接指定するコンストラクタ
		/// </summary>
		/// <param name="database">データベース</param>
		/// <param name="line_info">探索対象直線</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		LineSingleTriSearcher(std::shared_ptr<const DatabaseCore> database, const CoordinateLine& line_info, bool low_value_is_better = true);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		LineSingleTriSearcher(const LineSingleTriSearcher& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~LineSingleTriSearcher(void);
		/// <summary>
		/// 直線の構成情報にアクセス
		/// </summary>
		/// <returns>直線の構成情報</returns>
		const CoordinateLine& getLineInfomation(void) const;
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
		/// 探索アルゴリズムが終了条件を満たしたかを取得
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

#endif // !LINE_SINGLE_TRI_SEARCHER_HPP_
