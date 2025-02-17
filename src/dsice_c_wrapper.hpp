#ifndef DSICE_C_WRAPPER_HPP_
#define DSICE_C_WRAPPER_HPP_

#include <memory>
#include <unordered_map>

#include "dsice_tuner.hpp"

namespace dsice {

	/// <summary>
	/// チューナの識別子を格納する型
	/// </summary>
	typedef size_t dsice_tuner_id;

	/// <summary>
	/// 探索機構を指定する識別子
	/// </summary>
	enum DSICE_EXECUTION_MODE {
		DSICE_MODE_RECOMMENDED,
		DSICE_MODE_S_IPPE,
		DSICE_MODE_S_2017,
		DSICE_MODE_S_2018,
		DSICE_MODE_P_2024B
	};

	/// <summary>
	/// DSICE のデータベースが保持する性能評価値のふるまいを指定する列挙子
	/// </summary>
	enum DSICE_DATABASE_METRIC_TYPE {
		DSICE_METRIC_TYPE_OVERWRITTEN,
		DSICE_METRIC_TYPE_AVERAGE
	};

	/// <summary>
	/// Tuner の，純粋 C で利用するインスタンスを保持
	/// </summary>
	std::unordered_map<dsice_tuner_id, std::shared_ptr<Tuner<double>>>& getDsiceTuners(void);

	/// <summary>
	/// Tuner が持つ提案設定値一覧 (2 次元 vector) を C で使うため，vector の実データアドレスを格納．<br/>
	/// 領域は Tuner が保持するため，free などの操作は不要です (してはいけません)
	/// </summary>
	std::unordered_map<dsice_tuner_id, std::vector<const double*>>& getTunerSuggestBuffers(void);

	/// <summary>
	/// 指定 ID の登録済性能パラメタバッファにアクセス<br/>
	/// 領域は Tuner が保持するため，free などの操作は不要です (してはいけません)
	/// </summary>
	/// <param name="tuner_id">チューナを指定する ID</param>
	/// <returns>登録済性能パラメタバッファ</returns>
	std::vector<const double*>& getTargetParamBuffer(dsice_tuner_id tuner_id);

	/// <summary>
	/// Tuner による暫定最良パラメタ設定値を C で使うためのインスタンスを保持
	/// </summary>
	std::unordered_map<dsice_tuner_id, std::vector<double>>& getTentativeBestParameterBuffers(void);

	/// <summary>
	/// 指定 ID の Tuner インスタンスを取得
	/// </summary>
	/// <param name="tuner_id">チューナを指定する ID</param>
	/// <returns>Tuner インスタンス</returns>
	std::shared_ptr<Tuner<double>> getTuner(dsice_tuner_id tuner_id);

	/// <summary>
	/// 指定 ID の提案値バッファにアクセス<br/>
	/// 領域は Tuner が保持するため，free などの操作は不要です (してはいけません)
	/// </summary>
	/// <param name="tuner_id">チューナを指定する ID</param>
	/// <returns>提案値バッファ</returns>
	std::vector<const double*>& getSuggestBuffer(dsice_tuner_id tuner_id);

	/// <summary>
	/// 指定 ID の最良推定パラメタ設定値のバッファインスタンスを取得
	/// </summary>
	/// <param name="tuner_id">チューナを指定する ID</param>
	/// <returns>最良推定パラメタ設定値のバッファ</returns>
	std::vector<double>& getBestParamBuffer(dsice_tuner_id tuner_id);

#ifdef __cplusplus
	extern "C" {
#endif

		/// <summary>
		/// チューナを生成
		/// </summary>
		/// <returns>生成されたチューナを表す ID</returns>
		dsice_tuner_id DSICE_CREATE(void);

		/// <summary>
		/// チューナを削除
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		void DSICE_DELETE(dsice_tuner_id tuner_id);

		/// <summary>
		/// 性能パラメタが取り得る値を指定して性能パラメタを追加
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="parameter_length">性能パラメタが取り得る値の数</param>
		/// <param name="parameter">性能パラメタが取り得る値リスト</param>
		void DSICE_APPEND_PARAMETER(dsice_tuner_id tuner_id, size_t parameter_length, double* parameter);

		/// <summary>
		/// 閉区間と間隔を指定し，性能パラメタを 1 つ追加
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="min">閉区間の最小値</param>
		/// <param name="max">閉区間の最大値</param>
		/// <param name="space_length">性能パラメタが取り得る値同士の間隔</param>
		void DSICE_APPEND_PARAMETER_BY_LINEAR_SPACE(dsice_tuner_id tuner_id, double min, double max, double space_length);

		/// <summary>
		/// 実行する探索機構を直接指定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="mode">実行する探索機構の識別子</param>
		void DSICE_SELECT_MODE(dsice_tuner_id tuner_id, DSICE_EXECUTION_MODE mode);

		/// <summary>
		/// 初期点探索の実行を設定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="do_init_search">初期点探索を行う場合 true</param>
		void DSICE_CONFIG_DO_INITIAL_SEARCH(dsice_tuner_id tuner_id, bool do_init_search);

		/// <summary>
		/// 初期点となる性能パラメタ設定値を指定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="initial_parameter">初期点となる性能パラメタ設定値</param>
		void DSICE_CONFIG_SPECIFY_INITIAL_PARAMETER(dsice_tuner_id tuner_id, double* initial_parameter);

		/// <summary>
		/// ログの記録を設定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="record_log">ログをとる場合 true</param>
		void DSICE_CONFIG_RECORD_LOG(dsice_tuner_id tuner_id, bool record_log);

		/// <summary>
		/// 低い値の探索を設定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="lower_value_is_better">低い値ほど良い場合 true</param>
		void DSICE_CONFIG_LOWER_VALUE_IS_BETTER(dsice_tuner_id tuner_id, bool lower_value_is_better);

		/// <summary>
		/// 高い値の探索を設定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="higher_value_is_better">高い値ほど良い場合 true</param>
		void DSICE_CONFIG_HIGHER_VALUE_IS_BETTER(dsice_tuner_id tuner_id, bool higher_value_is_better);

		/// <summary>
		/// 並列探索用アルゴリズムの使用を設定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="parallel_mode">並列探索用アルゴリズムの場合 true</param>
		void DSICE_CONFIG_SET_PARALLEL_MODE(dsice_tuner_id tuner_id, bool parallel_mode);

		/// <summary>
		/// 近似関数 d-Spline の滑らかさを決定する重み係数を指定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="alpha">近似関数の滑らかさを決定する重み係数 (小さいほどデータ追随，値が大きいほど滑らか)</param>
		void DSICE_CONFIG_SET_DSPLINE_ALPHA(dsice_tuner_id tuner_id, double alpha);

		/// <summary>
		/// DSICE のデータベースが保有する性能評価値のふるまいを指定
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="metric_type">DSICE のデータベースが保有する性能評価値のふるまい</param>
		void DSICE_CONFIG_SET_DATABASE_METRIC_TYPE(dsice_tuner_id tuner_id, DSICE_DATABASE_METRIC_TYPE metric_type);

		/// <summary>
		/// 性能パラメタに設定すべき値を取得
		/// </summary>
		/// <param name="id">チューナを指定する ID</param>
		/// <returns>性能パラメタに設定すべき値</returns>
		const double* DSICE_BEGIN(dsice_tuner_id tuner_id);

		/// <summary>
		/// 性能パラメタに設定すべき値の一覧を取得．<br/>
		/// 一覧に含まれる値の個数は，この関数の呼び出し後に DSICE_GET_SUGGESTED_NUM 関数を呼び出して取得してください
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <returns>性能パラメタに設定すべき値の一覧</returns>
		const double** DSICE_BEGIN_PARALLEL(dsice_tuner_id tuner_id);

		/// <summary>
		/// 性能パラメタに設定すべき値として提案されている数を取得．<br/>
		/// この関数は DSICE_BEGIN_PARALLEL の後に呼び出してください．
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <returns>性能パラメタに設定すべき値として提案されている数</returns>
		size_t DSICE_GET_SUGGESTED_NUM(dsice_tuner_id tuner_id);

		/// <summary>
		/// 実測した性能評価値を登録
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="performance_value">実測した性能評価値</param>
		void DSICE_END(dsice_tuner_id tuner_id, double performance_value);

		/// <summary>
		/// 登録済性能パラメタの数を取得
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <returns>登録済性能パラメタの数</returns>
		size_t DSICE_GET_PARAM_NUM(dsice_tuner_id tuner_id);

		/// <summary>
		/// 指定された性能パラメタの取りうる値の数を取得
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="parameter_idx">性能パラメタの登録順インデックス</param>
		/// <returns>指定された性能パラメタの取りうる値の数</returns>
		size_t DSICE_GET_PARAM_LENGTH(dsice_tuner_id tuner_id, size_t parameter_idx);

		/// <summary>
		/// 登録済の性能パラメタリストを取得
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <returns>登録済の性能パラメタリスト</returns>
		const double** DSICE_GET_TARGET_PARAMETERS(dsice_tuner_id tuner_id);

		/// <summary>
		/// 暫定最良設定値 (探索基準点に対応する性能パラメタ設定値) を取得
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <returns>性能パラメタの暫定最良設定値</returns>
		const double* DSICE_GET_TENTATIVE_BEST_PARAMETER(dsice_tuner_id tuner_id);

		/// <summary>
		/// 探索アルゴリズムが終了条件を満たしたかを取得
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <returns>終了条件を満たしていれば true</returns>
		bool DSICE_IS_SEARCH_FINISHED(dsice_tuner_id tuner_id);

		/// <summary>
		/// DSICE_BEGIN からの経過時間を計測し，性能評価値として登録
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		void DSICE_END_TIME(dsice_tuner_id tuner_id);

		/// <summary>
		/// 実測した性能評価値の一覧を登録
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="value_num">登録する性能評価値の数</param>
		/// <param name="performance_value">登録する性能評価値の一覧</param>
		void DSICE_END_PARALLEL(dsice_tuner_id tuner_id, size_t value_num, const double* performance_value);

		/// <summary>
		/// チューニング結果を表示
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		void DSICE_PRINT_TUNING_RESULT_STD(dsice_tuner_id tuner_id);

		/// <summary>
		/// チューニング結果をファイル出力
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="file_path">出力先ファイル名 (空文字で標準出力)</param>
		void DSICE_PRINT_TUNING_RESULT_FILE(dsice_tuner_id tuner_id, const char* file_path);

		/// <summary>
		/// 簡易的なログを標準出力
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		void DSICE_PRINT_SIMPLE_LOOP_LOG_STD(dsice_tuner_id tuner_id);

		/// <summary>
		/// 簡易的なログをファイル出力
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="file_path">出力先ファイル名 (空文字で標準出力)</param>
		void DSICE_PRINT_SIMPLE_LOOP_LOG_FILE(dsice_tuner_id tuner_id, const char* file_path);

		/// <summary>
		/// DSICE Viewer 向けの探索空間ファイルを出力
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="file_path">出力先ファイル名</param>
		/// <param name="default_value">未実測点で代わりに出力する値</param>
		void DSICE_OUTPUT_SEARCH_SPACE_FOR_VIEWER(dsice_tuner_id tuner_id, const char* file_path, double default_value);

		/// <summary>
		/// DSICE Viewer 向けのログファイルを出力
		/// </summary>
		/// <param name="tuner_id">チューナを指定する ID</param>
		/// <param name="file_path">出力先ファイル名</param>
		void DSICE_OUTPUT_TUNER_LOG_FOR_VIEWER(dsice_tuner_id tuner_id, const char* file_path);

#ifdef __cplusplus
	}
#endif
}

#endif // !DSICE_C_WRAPPER_HPP_
