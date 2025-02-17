#ifndef DSICE_LOGGING_DATABASE_HPP_
#define DSICE_LOGGING_DATABASE_HPP_

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "dsice_search_space.hpp"
#include "dsice_database_logging_extension.hpp"
#include "sampling_log.hpp"
#include "suggest_group_log.hpp"
#include "base_point_log.hpp"

namespace dsice {

	/// <summary>
	/// ログを記録する拡張データベース
	/// </summary>
	class LoggingDatabase final : public DatabaseLoggingExtension {
	private:
		/// <summary>
		/// 探索空間の大きさ (座標定義)
		/// </summary>
		space_size search_space_size;
		/// <summary>
		/// 実測標本データ
		/// </summary>
		std::unordered_map<coordinate, std::shared_ptr<Metric>> samples;
		/// <summary>
		/// 探索時の基準点データ (基準点と探索済方向の組)
		/// </summary>
		std::unordered_map<coordinate, std::unordered_set<DirectionLine>> base_points;
		/// <summary>
		/// データベース内部で性能評価値を保持する際の型
		/// </summary>
		DatabaseMetricType metric_type = DatabaseMetricType::AverageDouble;
		/// <summary>
		/// 実測のログ．コンストラクタでダミー基準点を保持
		/// </summary>
		std::vector<BasePointLog> log;
		/// <summary>
		/// 直近で基準点が登録された際，異なる基準点になったかどうか
		/// </summary>
		bool has_base_coordinate_changed = false;
		/// <summary>
		/// 直近ループで登録された標本データの座標一覧 (※ 標本登録なしで基準点移動が繰り返されると逆順走査のコストが大きくなるため，ログと重複して持つ)
		/// </summary>
		coordinate_list latest_sample_coordinates;
		/// <summary>
		/// 直近ループで登録された性能評価値一覧 (※ 標本登録なしで基準点移動が繰り返されると逆順走査のコストが大きくなるため，ログと重複して持つ)
		/// </summary>
		std::vector<double> latest_measured_values;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="search_space">性能パラメタが取り得る値の数リスト</param>
		/// <param name="database_metric_type">データベース内部で性能評価値を保持する際の型</param>
		LoggingDatabase(const space_size& search_space, DatabaseMetricType database_metric_type = DatabaseMetricType::AverageDouble);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		LoggingDatabase(const LoggingDatabase& original);
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name=""></param>
		~LoggingDatabase(void);
		/// <summary>
		/// 探索空間の次元数 (探索対象のパラメタ数) を取得
		/// </summary>
		/// <returns>探索空間の次元数 (探索対象のパラメタ数)</returns>
		dimension_size getSpaceDimension(void) const override;
		/// <summary>
		/// 探索空間の大きさ (各性能パラメタの取りうる値の数) を取得
		/// </summary>
		/// <returns>探索空間の大きさ (各性能パラメタの取りうる値の数)</returns>
		const space_size& getSpaceSize(void) const override;
		/// <summary>
		/// 標本データを登録
		/// </summary>
		/// <param name="target_coordinate">標本データの座標</param>
		/// <param name="metric_value">性能評価値</param>
		void setSampleMetricValue(const coordinate& target_coordinate, double metric_value) override;
		/// <summary>
		/// 標本点が登録済かどうか
		/// </summary>
		/// <param name="target_coordinates">標本点の座標</param>
		/// <returns>登録済なら true</returns>
		bool hasSample(const coordinate& target_coordinate) const override;
		/// <summary>
		/// 登録済標本点の数を取得
		/// </summary>
		/// <returns>登録済標本点の数</returns>
		std::size_t getExistSamplesNum(void) const override;
		/// <summary>
		/// 登録済の標本の性能評価値を取得
		/// </summary>
		/// <param name="target_coordinate">取得したい標本データの座標</param>
		/// <returns>当該標本の性能評価値</returns>
		double getSampleMetricValue(const coordinate& target_coordinate) const override;
		/// <summary>
		/// 指定座標を探索基準点として登録
		/// </summary>
		/// <param name="target_coordinates">探索基準点の座標</param>
		void setBasePoint(const coordinate& target_coordinate) override;
		/// <summary>
		/// 探索候補リストを更新
		/// </summary>
		/// <param name="candidates_list">新しい探索候補リスト</param>
		void updateCandidateList(const coordinate_list candidates_list);
		/// <summary>
		/// 基準点およびその探索済方向の一覧を取得
		/// </summary>
		/// <returns>基準点およびその探索済方向の一覧</returns>
		const std::unordered_map<coordinate, std::unordered_set<DirectionLine>>& getBasePoints(void) const override;
		/// <summary>
		/// 指定座標が探索基準点になったことがあるか
		/// </summary>
		/// <param name="target_coordinates">調べる座標</param>
		/// <returns>過去に探索基準点となったことがあれば true</returns>
		bool hasBeenBasePoint(const coordinate& target_coordinate) const override;
		/// <summary>
		/// 指定座標を通る直線のうち，指定直線を探索済として登録．<br/>
		/// 過去に基準点になっていない座標を指定した場合でも通常通り動作しますが，ログには基準点情報は残りません．
		/// </summary>
		/// <param name="base_coordinates">基準となる探索基準点</param>
		/// <param name="line_info">探索した直線</param>
		void recordSearchedDirection(const coordinate& base_coordinate, const DirectionLine& line_info) override;
		/// <summary>
		/// 指定座標を通る直線のうち，指定直線が探索済かどうかを判定
		/// </summary>
		/// <param name="base_coordinates">基準となる探索基準点</param>
		/// <param name="line_info">判定する直線</param>
		/// <returns>指定直線が探索済なら true</returns>
		bool isSearchedDirection(const coordinate& base_coordinate, const DirectionLine& line_info) const override;
		/// <summary>
		/// ループの終わりを記録
		/// </summary>
		void setLoopEnd(void) override;
		/// <summary>
		/// 直近に登録した標本データの座標を取得
		/// </summary>
		/// <returns>直近に登録した標本データの値</returns>
		const std::vector<coordinate>& getLatestSampleCoordinate(void) const override;
		/// <summary>
		/// 直近に登録した性能評価値を取得
		/// </summary>
		/// <returns>直近に登録した性能評価値</returns>
		const std::vector<double>& getLatestMeasuredValue(void) const override;
		/// <summary>
		/// 現在の基準点の座標を取得
		/// </summary>
		/// <returns>現在の基準点の座標</returns>
		const coordinate& getLatestBaseCoordinate(void) const override;
		/// <summary>
		/// 直近で基準点が登録された際，基準点は変化していたかを取得
		/// </summary>
		/// <returns>変化した (直近基準点がその前の基準点と異なる座標の) 場合は true</returns>
		bool hasBaseCoordinateChanged(void) const override;
		/// <summary>
		/// 歴代探索基準点の一覧を取得
		/// </summary>
		/// <returns>歴代探索基準点の一覧</returns>
		coordinate_list getBaseCoordinateListLog(void) const override;
		/// <summary>
		/// 歴代の探索候補リストの一覧を取得
		/// </summary>
		/// <returns>歴代の探索候補リストの一覧</returns>
		std::vector<coordinate_list> getSuggestedCoordinateListsLog(void) const override;
		/// <summary>
		/// 実測データ (パラメタ設定値と性能評価値の組) の一覧を取得
		/// </summary>
		/// <returns>実測データの一覧</returns>
		std::vector<dsice_measure_data> getMeasuredDataLog(void) const override;
		/// <summary>
		/// 基準点，実測候補，実測データの全ログデータを取得
		/// </summary>
		/// <returns>全ログデータ</returns>
		const std::vector<BasePointLog>& getAllMeasuringLog(void) const override;
	};
}

#endif // !DSICE_LOGGING_DATABASE_HPP_
