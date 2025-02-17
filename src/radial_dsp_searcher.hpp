#ifndef RADIAL_DSP_SEARCHER_HPP_
#define RADIAL_DSP_SEARCHER_HPP_

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "dsice_search_space.hpp"
#include "direction_line.hpp"
#include "dsice_database_core.hpp"
#include "dsice_searchers_manager.hpp"

namespace dsice {

	/// <summary>
	/// 放射型 d-Spline 探索機構
	/// </summary>
	class RadialDspSearcher : public SearchersManager {
	private:
		/// <summary>
		/// データベースへのアクセス
		/// </summary>
		std::shared_ptr<const DatabaseCore> database;
		/// <summary>
		/// 探索基準点
		/// </summary>
		coordinate base_coordinate;
		/// <summary>
		/// 管理中探索機構 (同時変化軸数順)
		/// </summary>
		std::vector<std::vector<ManagedSearcher>> searchers;
		/// <summary>
		/// 探索機構と方向の対応関係
		/// </summary>
		std::unordered_map<DirectionLine, std::pair<std::size_t, std::size_t>> searchers_of_direction;
		/// <summary>
		/// 終了条件を満たした方向
		/// </summary>
		std::unordered_set<DirectionLine> finished_direction;
		/// <summary>
		/// 担当する探索対象座標
		/// </summary>
		coordinate_list target_coorinates;
		/// <summary>
		/// 実測すべき点
		/// </summary>
		coordinate_list suggested_coordinates;
		/// <summary>
		/// 実測済点
		/// </summary>
		std::unordered_set<coordinate> measured_coordinates;
		/// <summary>
		/// 低い値ほど良いかどうか
		/// </summary>
		bool lower_is_better;
		/// <summary>
		/// 内部の探索機構に指定する，d-Spline 関数の滑らかさを決定する重み係数
		/// </summary>
		double dsp_alpha;
		/// <summary>
		/// 実測データの更新用バッファ (座標)
		/// </summary>
		std::vector<coordinate> measured_coordinate_buffer;
		/// <summary>
		/// 実測データの更新用バッファ (性能評価値)
		/// </summary>
		std::vector<double> measured_metric_value_buffer;
		/// <summary>
		/// ログを記録するモードかどうか
		/// </summary>
		bool is_logging_mode_on;
		/// <summary>
		/// 探索対象点の中で最良と判定されている座標
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
		/// 探索基準点より良い値を見つけたかどうか
		/// </summary>
		bool is_found_better_point = false;
		/// <summary>
		/// 各 Searcher の結果にしたがって探索対象をリストに追加
		/// </summary>
		void updateSuggested(void);
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="search_database">データベース</param>
		/// <param name="central_coordinate">基点座標</param>
		/// <param name="max_changed_axis_num">同時変化させる軸数の最大値</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		/// <param name="alpha">近似関数の滑らかさを決定する重み係数 (小さいほどデータ追随，値が大きいほど滑らか)</param>
		RadialDspSearcher(std::shared_ptr<const DatabaseCore> search_database, const coordinate& central_coordinate, std::size_t max_changed_axis_num, bool low_value_is_better = true, double alpha = 0.1);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		RadialDspSearcher(const RadialDspSearcher& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~RadialDspSearcher(void);
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
		/// <summary>
		/// 管理中の内部探索機構の数を取得
		/// </summary>
		/// <returns>管理中の内部探索機構の数</returns>
		std::size_t getManagedSearchersNum(void) const override;
		/// <summary>
		/// 管理中内部探索機構のうち，まだ終了条件を満たさずに稼働しているものの数を取得
		/// </summary>
		/// <returns>終了条件をまだ満たしていない内部探索機構の数</returns>
		std::size_t getRunningSearchersNum(void) const override;
		/// <summary>
		/// 管理中内部探索機構のうち，終了条件を満たしたものの数を取得
		/// </summary>
		/// <returns>終了条件を満たした内部探索機構の数</returns>
		std::size_t getFinishedSearchersNum(void) const override;
		/// <summary>
		/// 探索基準点より良い点を見つけたかどうか
		/// </summary>
		/// <returns>良い点が見つかっていれば true</returns>
		bool isFoundBetterPoint(void) const;
		/// <summary>
		/// この機構により探索が完了した方向情報を取得
		/// </summary>
		/// <returns>この機構により探索が完了した方向情報</returns>
		const std::unordered_set<DirectionLine>& getFinishedDirections(void) const;
	};
}

#endif // !RADIAL_DSP_SEARCHER_HPP_
