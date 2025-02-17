#ifndef SAMPLING_LOG_HPP_
#define SAMPLING_LOG_HPP_

#include <chrono>

#include "dsice_search_space.hpp"

namespace dsice {
	/// <summary>
	/// 実測 1 回あたりの情報を記録
	/// </summary>
	class SamplingLog final {
	private:
		/// <summary>
		/// 実測標本データそのもの
		/// </summary>
		dsice_measure_data measure_data;
		/// <summary>
		/// 実測開始時刻
		/// </summary>
		std::chrono::system_clock::time_point measure_start_time;
		/// <summary>
		/// 実測終了時刻
		/// </summary>
		std::chrono::system_clock::time_point measure_end_time;
	public:
		/// <summary>
		/// 時間を記録しない場合のコンストラクタ
		/// </summary>
		/// <param name="sample_coordinate">標本データの座標</param>
		/// <param name="metric_value">標本データの性能評価値</param>
		SamplingLog(const coordinate& sample_coordinate, double metric_value);
		/// <summary>
		/// 時間を記録する場合のコンストラクタ
		/// </summary>
		/// <param name="sample_coordinate">標本データの座標</param>
		/// <param name="metric_value">標本データの性能評価値</param>
		/// <param name="measure_start_time">実測開始時刻</param>
		/// <param name="measure_end_time">実測終了時刻</param>
		SamplingLog(const coordinate& sample_coordinate, double metric_value, std::chrono::system_clock::time_point measure_start_time, std::chrono::system_clock::time_point measure_end_time);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		SamplingLog(const SamplingLog& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~SamplingLog(void);
		/// <summary>
		/// 標本データの座標を取得
		/// </summary>
		/// <returns>標本データの座標</returns>
		const coordinate& getSampleCoordinate(void) const;
		/// <summary>
		/// 標本データの性能評価値を取得
		/// </summary>
		/// <returns>標本データの性能評価値</returns>
		double getMetricValue(void) const;
		/// <summary>
		/// 実測開始時刻を取得
		/// </summary>
		/// <returns>実測開始時刻．ただし，未登録時はインスタンス作成時刻になります．</returns>
		const std::chrono::system_clock::time_point& getMeasureStartTime(void) const;
		/// <summary>
		/// 実測終了時刻を取得
		/// </summary>
		/// <returns>実測終了時刻．ただし，未登録時はインスタンス作成時刻になります．</returns>
		const std::chrono::system_clock::time_point& getMeasureEndTime(void) const;
		/// <summary>
		/// 実測期間の間隔を取得
		/// </summary>
		/// <returns>実測時間．ただし，実測時刻の未登録時は同一時刻同士の差になります．</returns>
		std::chrono::system_clock::duration getMeasureDuration(void) const;
	};
}

#endif // !SAMPLING_LOG_HPP_
