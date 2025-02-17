#ifndef DSICE_OPERATOR_P_2024B_HPP_
#define DSICE_OPERATOR_P_2024B_HPP_

#include <memory>
#include <string>

#include "dsice_search_space.hpp"
#include "dsice_database_metric_type.hpp"
#include "dsice_database_standard_functions.hpp"
#include "dsice_searcher.hpp"
#include "dsice_operator.hpp"

namespace dsice {

	// ID : P_2024B
	// 準拠発表 :  Proposal for A Parallel Automatic Tuning Using d-Spline According to The Operating State of The Computer System (URL : https://sc24.conference-program.com/presentation/?id=post207&sess=sess487)

	/// <summary>
	/// 反復放射型 d-Spline 並列探索法を実装した探索機構
	/// </summary>
	class Operator_P_2024B final : public Operator {
	private:
		/// <summary>
		/// データベース
		/// </summary>
		std::shared_ptr<DatabaseStandardFunctions> database;
		/// <summary>
		/// 実行中の探索アルゴリズム
		/// </summary>
		std::shared_ptr<Searcher> searcher;
		/// <summary>
		/// 探索フェーズ
		/// </summary>
		std::size_t searching_phase = 0;
		/// <summary>
		/// 低い値ほど良いかどうか
		/// </summary>
		bool lower_is_better;
		/// <summary>
		/// ログを記録するモードかどうか
		/// </summary>
		bool is_logging_mode_on;
		/// <summary>
		/// 近似関数の滑らかさを決定する重み係数
		/// </summary>
		double dsp_alpha;
		/// <summary>
		/// 探索基準点
		/// </summary>
		coordinate base_coordinate;
		/// <summary>
		/// 基準点の値
		/// </summary>
		double base_value;
		/// <summary>
		/// 何ループ目か (setMetricValue() が呼ばれた回数)
		/// </summary>
		std::size_t loop_count = 0;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="parameters">各性能パラメタが取りうる値の数リスト</param>
		/// <param name="low_value_is_better">性能評価値が低い値ほど良いのであれば true</param>
		/// <param name="logging_on">ログを取る場合は true</param>
		/// <param name="initialize_way">初期点の決定方法</param>
		/// <param name="alpha">近似関数の滑らかさを決定する重み係数 (小さいほどデータ追随，値が大きいほど滑らか)</param>
		/// <param name="metric_type">性能パラメタの同一設定値に複数の性能評価値が登録された際の挙動</param>
		/// <param name="initial_coordinate">初期点の座標</param>
		Operator_P_2024B(const space_size& parameters, bool low_value_is_better = true, bool logging_on = false, OperatorInitializeWay initialize_way = OperatorInitializeWay::CENTER, double alpha = 0.1, DatabaseMetricType metric_type = DatabaseMetricType::AverageDouble, coordinate initial_coordinate = {});
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		Operator_P_2024B(const Operator_P_2024B& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~Operator_P_2024B(void);
		/// <summary>
		/// 探索基準点を取得
		/// </summary>
		/// <returns>探索基準点</returns>
		const coordinate& getBaseCoordinate(void) const override;
		/// <summary>
		/// 次に実測すべき性能パラメタ性能値 (座標) を取得
		/// </summary>
		/// <returns>次に実測すべき性能パラメタ性能値 (座標)</returns>
		const coordinate& getSuggested(void) override;
		/// <summary>
		/// 実測すべき性能パラメタ性能値 (座標) のリストを取得
		/// </summary>
		/// <returns>実測すべき性能パラメタ性能値 (座標) のリスト</returns>
		const coordinate_list& getSuggestedList(void) override;
		/// <summary>
		/// 実測データを記録
		/// </summary>
		/// <param name="measured_coordinate">実測した座標</param>
		/// <param name="metric_value">実測した値</param>
		void setMetricValue(const coordinate& measured_coordinate, double metric_value) override;
		/// <summary>
		/// 状態を更新
		/// </summary>
		void updateState(void) override;
		/// <summary>
		/// 探索アルゴリズムが終了条件を満たしたかを取得．<br/>
		/// 終了条件を持たない探索アルゴリズムの場合，この関数の挙動は各実装クラス依存です．
		/// </summary>
		/// <returns>終了条件を満たしていれば true</returns>
		bool isSearchFinished(void) override;
		/// <summary>
		/// その時点で探索アルゴリズムが最良と判定している性能パラメタ設定値 (座標) を取得
		/// </summary>
		/// <returns>探索アルゴリズムが最良と判定している性能パラメタ設定値 (座標)</returns>
		const coordinate& getBestJudgedCoordinate(void) override;
		/// <summary>
		/// 実測された中で性能評価値が最良だった性能パラメタ設定値 (座標) を取得
		/// </summary>
		/// <returns>実測された中で性能評価値が最良だった性能パラメタ設定値 (座標)</returns>
		const coordinate& getBestMeasuredCoordinate(void) override;
		/// <summary>
		/// アルゴリズムセットの ID 文字列を取得
		/// </summary>
		/// <returns>アルゴリズムセットの ID 文字列</returns>
		std::string getAlgorithmId(void) const override;
		/// <summary>
		/// 使用しているデータベースを取得
		/// </summary>
		/// <returns>使用しているデータベース</returns>
		std::shared_ptr<DatabaseCore> getDatabase(void) const override;
		/// <summary>
		/// これまでのループ回数 (updateState() が呼ばれた回数) を取得
		/// </summary>
		/// <returns>これまでのループ回数</returns>
		std::size_t getLoopCount(void) const override;
		/// <summary>
		/// 実行中の探索モード名を取得
		/// </summary>
		/// <returns>実行中の探索モード名</returns>
		std::string getSearchModeName(void) const override;
	};
}

#endif // !DSICE_OPERATOR_P_2024B_HPP_
