#ifndef DSICE_TUNER_HPP_
#define DSICE_TUNER_HPP_

#include <memory>
#include <vector>
#include <string>

#include "dsice_execution_mode.hpp"
#include "dsice_database_metric_type.hpp"

namespace dsice {

	/// <summary>
	/// DSICE のチューニング機構
	/// </summary>
	/// <typeparam name="T">性能パラメタの値型</typeparam>
	template <typename T = double>
	class Tuner final {
	private:
		class TunerPrivateMembers;
		std::shared_ptr<TunerPrivateMembers> private_members = nullptr;
	public:
		/// <summary>
		/// 性能パラメタを後から指定するコンストラクタ
		/// </summary>
		Tuner(void);
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="parameters">性能パラメタの取りうる値リスト</param>
		Tuner(const std::vector<std::vector<T>>& parameters);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		Tuner(const Tuner& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~Tuner(void);
		/// <summary>
		/// 性能パラメタが取り得る値を指定して性能パラメタを追加
		/// </summary>
		/// <param name="parameter">性能パラメタが取り得る値リスト</param>
		void appendParameter(const std::vector<T>& parameter);
		/// <summary>
		/// 閉区間と間隔を指定し，性能パラメタを 1 つ追加
		/// </summary>
		/// <param name="min">閉区間の最小値</param>
		/// <param name="max">閉区間の最大値</param>
		/// <param name="space_length">性能パラメタが取り得る値同士の間隔</param>
		void appendParameterByLinearSpace(const T& min, const T& max, const T& space_length);
		/// <summary>
		/// 実行する探索機構を直接指定
		/// </summary>
		/// <param name="mode">実行する探索機構の識別子</param>
		void selectMode(ExecutionMode execution_mode);
		/// <summary>
		/// 初期点探索の実行を設定
		/// </summary>
		/// <param name="do_init_search">初期点探索を行う場合 true</param>
		void config_doInitialSearch(bool do_init_search = true);
		/// <summary>
		/// 初期点となる性能パラメタ設定値を指定
		/// </summary>
		/// <param name="initial_parameter">初期点となる性能パラメタ設定値</param>
		void config_specifyInitialParameter(const std::vector<T>& initial_parameter);
		/// <summary>
		/// ログの記録を設定
		/// </summary>
		/// <param name="record_log">ログをとる場合 true</param>
		void config_recordLog(bool record_log = true);
		/// <summary>
		/// 低い値の探索を設定
		/// </summary>
		/// <param name="lower_value_is_better">低い値ほど良い場合 true</param>
		void config_lowerValueIsBetter(bool lower_value_is_better = true);
		/// <summary>
		/// 高い値の探索を設定
		/// </summary>
		/// <param name="higher_value_is_better">高い値ほど良い場合 true</param>
		void config_higherValueIsBetter(bool higher_value_is_better = true);
		/// <summary>
		/// 並列探索用アルゴリズムの使用を設定
		/// </summary>
		/// <param name="parallel_mode">並列探索用アルゴリズムの場合 true</param>
		void config_setParallelMode(bool parallel_mode = true);
		/// <summary>
		/// 近似関数 d-Spline の滑らかさを決定する重み係数を指定
		/// </summary>
		/// <param name="alpha">近似関数の滑らかさを決定する重み係数 (小さいほどデータ追随，値が大きいほど滑らか)</param>
		void config_setDSplineAlpha(double alpha);
		/// <summary>
		/// DSICE のデータベースが保有する性能評価値のふるまいを指定
		/// </summary>
		/// <param name="metric_type">DSICE のデータベースが保有する性能評価値のふるまい</param>
		void config_setDatabaseMetricType(DatabaseMetricType metric_type);
		/// <summary>
		/// 性能パラメタに設定すべき値を取得
		/// </summary>
		/// <returns>性能パラメタに設定すべき値</returns>
		const std::vector<T>& getSuggestedNext(void);
		/// <summary>
		/// 性能パラメタに設定すべき値の一覧を取得
		/// </summary>
		/// <returns>性能パラメタに設定すべき値の一覧</returns>
		const std::vector<std::vector<T>>& getSuggestedList(void);
		/// <summary>
		/// 実測した性能評価値を登録
		/// </summary>
		/// <param name="metric_value">実測した性能評価値</param>
		void setMetricValue(double metric_value);
		/// <summary>
		/// getSuggestedNext() からの経過時間を計測し，性能評価値として登録
		/// </summary>
		void setTimePerformance(void);
		/// <summary>
		/// 実測した性能評価値の一覧を登録．<br/>
		/// 実測候補すべてのデータを揃える必要はありません．実測優先度順に，与えられたデータ数分のみ記録されます．
		/// </summary>
		/// <param name="metric_values_list">実測した性能評価値の一覧</param>
		void setMetricValuesList(const std::vector<double>& metric_values_list);
		/// <summary>
		/// 登録済性能パラメタの数を取得
		/// </summary>
		/// <returns>登録済性能パラメタの数</returns>
		std::size_t getParametersNum(void) const;
		/// <summary>
		/// 指定された性能パラメタの取りうる値の数を取得
		/// </summary>
		/// <param name="parameter_idx">性能パラメタの登録順インデックス</param>
		/// <returns>指定された性能パラメタの取りうる値の数</returns>
		std::size_t getParameterLength(std::size_t parameter_idx) const;
		/// <summary>
		/// 登録済の性能パラメタリストを取得
		/// </summary>
		/// <returns>登録済の性能パラメタリスト</returns>
		const std::vector<std::vector<T>>& getTargetParameters(void) const;
		/// <summary>
		/// 暫定最良設定値 (探索基準点に対応する性能パラメタ設定値) を取得
		/// </summary>
		/// <returns>性能パラメタの暫定最良設定値</returns>
		std::vector<T> getTentativeBestParameter(void) const;
		/// <summary>
		/// 探索アルゴリズムが終了条件を満たしたかを取得
		/// </summary>
		/// <returns>終了条件を満たしていれば true</returns>
		bool isSearchFinished(void) const;
		/// <summary>
		/// チューニング結果を表示
		/// </summary>
		/// <param name="file_path">出力先ファイル名 (空文字で標準出力)</param>
		void printTuningResult(std::string file_path = "") const;
		/// <summary>
		/// 簡易的なログを出力
		/// </summary>
		/// <param name="file_path">出力先ファイル名 (空文字で標準出力)</param>
		void printSimpleLoopLog(std::string file_path = "") const;
		/// <summary>
		/// DSICE Viewer 向けの探索空間ファイルを出力
		/// </summary>
		/// <param name="file_path">出力先ファイル名</param>
		/// <param name="default_value">未実測点で代わりに出力する値</param>
		void outputSearchSpaceForViewer(std::string file_path = "search_space.log", double default_value = 0) const;
		/// <summary>
		/// DSICE Viewer 向けのログファイルを出力
		/// </summary>
		/// <param name="file_path">出力先ファイル名</param>
		void outputLogForViewer(std::string file_path = "dsice_sampling.log") const;
	};
}

#endif // !DSICE_TUNER_HPP_
