#ifndef SUGGEST_GROUP_LOG_HPP_
#define SUGGEST_GROUP_LOG_HPP_

#include <vector>

#include "dsice_search_space.hpp"
#include "sampling_log.hpp"

namespace dsice {

	/// <summary>
	/// DSICE が実測提案する際の候補リストごとのロググループ
	/// </summary>
	class SuggestGroupLog final {
	private:
		/// <summary>
		/// 提案した候補リスト
		/// </summary>
		coordinate_list candidate_list;
		/// <summary>
		/// 実際の実測記録
		/// </summary>
		std::vector<SamplingLog> measured_data_list;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="candidate_list">探索候補の座標リスト</param>
		SuggestGroupLog(const coordinate_list& candidate_list);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		SuggestGroupLog(const SuggestGroupLog& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~SuggestGroupLog(void);
		/// <summary>
		/// 実測 1 回分の情報を記録 (インスタンス登録)
		/// </summary>
		/// <param name="measured_data">実測記録</param>
		void appendMeasuredData(const SamplingLog& measured_data);
		/// <summary>
		/// 実測 1 回分の情報を記録 (パラメタ登録，実測時刻なし)
		/// </summary>
		/// <param name="measured_coordinate">標本データの座標</param>
		/// <param name="metric_value">標本データの性能評価値</param>
		void appendMeasuredData(const coordinate& measured_coordinate, double metric_value);
		/// <summary>
		/// 実測 1 回分の情報を記録 (パラメタ登録，実測時刻あり)
		/// </summary>
		/// <param name="measured_coordinate">標本データの座標</param>
		/// <param name="metric_value">標本データの性能評価値</param>
		/// <param name="measure_start_time">実測開始時刻</param>
		/// <param name="measure_end_time">実測終了時刻</param>
		void appendMeasuredData(const coordinate& measured_coordinate, double metric_value, std::chrono::system_clock::time_point measure_start_time, std::chrono::system_clock::time_point measure_end_time);
		/// <summary>
		/// 実測候補リストを取得
		/// </summary>
		/// <returns>実測候補リスト</returns>
		const coordinate_list& getCandidateList(void) const;
		/// <summary>
		/// 実測 1 回分の情報を取得
		/// </summary>
		/// <returns>実測 1 回分の情報を</returns>
		const std::vector<SamplingLog>& getMeasuredDataList(void) const;
	};
}

#endif // !SUGGEST_GROUP_LOG_HPP_
