#ifndef BASE_POINT_LOG_HPP_
#define BASE_POINT_LOG_HPP_

#include <vector>

#include "dsice_search_space.hpp"
#include "sampling_log.hpp"
#include "suggest_group_log.hpp"

namespace dsice {

	/// <summary>
	/// 探索基準点 1 点あたりの情報を記録
	/// </summary>
	class BasePointLog final {
	private:
		/// <summary>
		/// 探索基準点座標
		/// </summary>
		coordinate base_point;
		/// <summary>
		/// この基準点で提案された候補リストおよび実測データのログ
		/// </summary>
		std::vector<SuggestGroupLog> suggest_log_list;
	public:
		/// <summary>
		/// 候補リストを指定しない場合のコンストラクタ
		/// </summary>
		/// <param name="base_point">探索基準点の座標</param>
		BasePointLog(const coordinate& base_point);
		/// <summary>
		/// 候補リストを指定する場合のコンストラクタ
		/// </summary>
		/// <param name="base_point">探索基準点の座標</param>
		/// <param name="first_candidate_list">最初の実測候補リスト</param>
		BasePointLog(const coordinate& base_point, const coordinate_list& first_candidate_list);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		BasePointLog(const BasePointLog& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~BasePointLog(void);
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
		/// 同一の探索基準点に対して実測候補リストを更新
		/// </summary>
		/// <param name="candidate_list">新しい実測候補リスト</param>
		void updateCandidateList(const coordinate_list& candidate_list);
		/// <summary>
		/// 探索基準点を取得
		/// </summary>
		/// <returns>探索基準点を取得</returns>
		const coordinate& getBasePoint(void) const;
		/// <summary>
		/// この基準点で提案された候補リストおよび実測データのログを取得
		/// </summary>
		/// <returns>この基準点で提案された候補リストおよび実測データのログ</returns>
		const std::vector<SuggestGroupLog>& getSuggestLogList(void) const;
	};
}

#endif // !BASE_POINT_LOG_HPP_
