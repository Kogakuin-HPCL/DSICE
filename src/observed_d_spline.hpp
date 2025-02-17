#ifndef OBSERVED_D_SPLINE_HPP_
#define OBSERVED_D_SPLINE_HPP_

#include <vector>
#include <memory>
#include <limits>

#include "d_spline.hpp"

namespace dsice {

	/// <summary>
	/// 監視モードの種類
	/// </summary>
	enum class DSplineObservationMode {
		/// <summary>
		/// 曲率，実測最大値，実測最小値，最小推定連続回数を記録
		/// </summary>
		STANDARD,
		/// <summary>
		/// 曲率，実測最大値，実測最小値，最小推定連続回数に加え，実測済ゾーン (推定値に応じて実測最大値と実測最小値で区切られるゾーン) の算出を行う
		/// </summary>
		MEASURED_ZONE
	};

	/// <summary>
	/// d-Spline の曲率などを監視，保持する機構．<br/>
	/// そのまま d-Spline として扱えるように継承していますが，役割は機能拡張であり，本体を別途用意する必要があります．
	/// </summary>
	class ObservedDSpline final : DSpline {
	private:
		/// <summary>
		/// 監視対象の d-Spline
		/// </summary>
		std::shared_ptr<DSpline> d_spline;
		/// <summary>
		/// 監視モード
		/// </summary>
		DSplineObservationMode mode;
		/// <summary>
		/// 全点の曲率
		/// </summary>
		std::vector<double> all_curvatures;
		/// <summary>
		/// 各標本データ点の曲率
		/// </summary>
		std::vector<double> sample_curvatures;
		/// <summary>
		/// 実測された中での最大値
		/// </summary>
		double measured_max = std::numeric_limits<double>::lowest();
		/// <summary>
		/// 実測された中での最小値
		/// </summary>
		double measured_min = std::numeric_limits<double>::max();
		/// <summary>
		/// 最低推定点インデックス
		/// </summary>
		std::size_t lowest_estimated_index = std::numeric_limits<std::size_t>::max();
		/// <summary>
		/// 最高推定点インデックス
		/// </summary>
		std::size_t highest_estimated_index = std::numeric_limits<std::size_t>::max();
		/// <summary>
		/// 最低推定点が何回連続での指定か記録するカウンタ
		/// </summary>
		std::size_t consecutive_lowest_count = 0;
		/// <summary>
		/// 最高推定点が何回連続での指定か記録するカウンタ
		/// </summary>
		std::size_t consecutive_highest_count = 0;
		/// <summary>
		/// 実測された最大値よりも高く推定されているマーカ位置のインデックス
		/// </summary>
		std::vector<std::size_t> higher_zone_markers;
		/// <summary>
		/// 実測された最大値と最小値の間の値で推定されているマーカ位置のインデックス
		/// </summary>
		std::vector<std::size_t> measured_zone_markers;
		/// <summary>
		/// 実測された最小値よりも低く推定されているマーカ位置のインデックス
		/// </summary>
		std::vector<std::size_t> lower_zone_markers;
		/// <summary>
		/// 実測された最大値よりも高く推定されている標本位置のインデックス
		/// </summary>
		std::vector<std::size_t> higher_zone_samples;
		/// <summary>
		/// 実測された最大値と最小値の間の値で推定されている標本位置のインデックス
		/// </summary>
		std::vector<std::size_t> measured_zone_samples;
		/// <summary>
		/// 実測された最小値よりも低く推定されている標本位置のインデックス
		/// </summary>
		std::vector<std::size_t> lower_zone_samples;
		/// <summary>
		/// 実測ゾーン系 (higher_zone, measured_zone, lower_zone) を更新
		/// </summary>
		void updateMeasuredZone(void);
		/// <summary>
		/// 曲率を計算する
		/// </summary>
		void updateCurvatures(void);
		/// <summary>
		/// 最低/最高推定点がどこかを調べる (マーカ点から最良推定位置を探し，その最近傍標本位置を求める)
		/// </summary>
		/// <param name="used_samples_num">前回最低/最高推定点を調べたときから標本データをいくつ追加したか</param>
		void checkBestEstimated(std::size_t used_samples_num = 1);
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="target_d_spline">監視対象の d-Spline オブジェクト</param>
		/// <param name="observation_mode">監視モード</param>
		ObservedDSpline(std::shared_ptr<DSpline> target_d_spline, DSplineObservationMode observation_mode = DSplineObservationMode::STANDARD);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		ObservedDSpline(const ObservedDSpline& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~ObservedDSpline(void);
		/// <summary>
		/// 全点の曲率を取得
		/// </summary>
		/// <returns>全点の曲率</returns>
		const std::vector<double>& getAllCurvatures(void) const;
		/// <summary>
		/// 各標本データ位置の曲率を取得
		/// </summary>
		/// <returns>各標本データ位置の曲率 (曲率は d-Spline を構成する全点に対して計算され，そのうち該当するものだけがピックアップされます．つまり，曲率は非標本データ点を含めて計算されます)</returns>
		const std::vector<double>& getSampleCurvatures(void) const;
		/// <summary>
		/// 同じ点が何回連続で最低推定点になっているかを取得．<br/>
		/// ここで，回数は update に渡された標本データ数を意味します．<br/>
		/// また，前回の最低推定点と異なる点が最低と推定されると「1」にリセットされます
		/// </summary>
		/// <returns>同じ点が最低推定点に何回連続で該当しているか</returns>
		std::size_t getConsecutiveLowestCount(void) const;
		/// <summary>
		/// 同じ点が何回連続で最高推定点になっているかを取得．<br/>
		/// ここで，回数は update に渡された標本データ数を意味します．
		/// また，前回の最高推定点と異なる点が最高と推定されると「1」にリセットされます
		/// </summary>
		/// <returns>同じ点が最高推定点に何回連続で該当しているか</returns>
		std::size_t getConsecutiveHighestCount(void) const;
		/// <summary>
		/// 実測された最大値を取得
		/// </summary>
		/// <returns>実測された最大値</returns>
		double getMaxMeasuredValue(void) const;
		/// <summary>
		/// 実測された最小値を取得
		/// </summary>
		/// <returns>実測された最小値</returns>
		double getMinMeasuredValue(void) const;
		/// <summary>
		/// 実測最大値より高い推定値のマーカ点インデックスリストを取得
		/// </summary>
		/// <returns>実測最大値より高い推定値のマーカ点インデックスリスト (大きい順，インデックス 0 が最大推定値)</returns>
		const std::vector<std::size_t>& getHigherZoneMarkerIndexes(void) const;
		/// <summary>
		/// 推定値が実測最大値と実測最小値の間であるマーカ点インデックスリストを取得
		/// </summary>
		/// <returns>推定値が実測最大値と実測最小値の間であるマーカ点インデックスリスト (リスト内の順序は未定義)</returns>
		const std::vector<std::size_t>& getMeasuredZoneMarkerIndexes(void) const;
		/// <summary>
		/// 実測最小値より低い推定値のマーカ点インデックスリストを取得
		/// </summary>
		/// <returns>実測最小値より低い推定値のマーカ点インデックスリスト (小さい順，インデックス 0 が最小推定値)</returns>
		const std::vector<std::size_t>& getLowerZoneMarkerIndexes(void) const;
		/// <summary>
		/// 実測最大値より高い推定値の標本位置インデックスリストを取得
		/// </summary>
		/// <returns>実測最大値より高い推定値の標本位置インデックスリスト (大きい順，インデックス 0 が最大推定値)</returns>
		const std::vector<std::size_t>& getHigherZoneSampleIndexes(void) const;
		/// <summary>
		/// 推定値が実測最大値と実測最小値の間である標本位置インデックスリストを取得
		/// </summary>
		/// <returns>推定値が実測最大値と実測最小値の間である標本位置インデックスリスト (リスト内の順序は未定義)</returns>
		const std::vector<std::size_t>& getMeasuredZoneSampleIndexes(void) const;
		/// <summary>
		/// 実測最小値より低い推定値の標本位置インデックスリストを取得
		/// </summary>
		/// <returns>実測最小値より低い推定値の標本位置インデックスリスト (小さい順，インデックス 0 が最小推定値)</returns>
		const std::vector<std::size_t>& getLowerZoneSampleIndexes(void) const;
		/// <summary>
		/// 標本データを追加
		/// </summary>
		/// <param name="sample_idx">標本データのインデックス</param>
		/// <param name="sample_value">標本データの値</param>
		void update(std::size_t sample_idx, double sample_value) override;
		/// <summary>
		/// 標本データを追加
		/// </summary>
		/// <param name="sample_data">標本データのインデックスと値の組</param>
		void update(const std::pair<std::size_t, double>& sample_data) override;
		/// <summary>
		/// 複数の標本データを一括追加
		/// </summary>
		/// <param name="sample_data">標本データのインデックスと値の組の一覧</param>
		void update(const std::vector<std::pair<std::size_t, double>>& sample_data) override;
		/// <summary>
		/// 離散関数の値を 1 つ取得 (離散点インデックス指定)
		/// </summary>
		/// <param name="marker_idx">離散関数における点インデックス</param>
		/// <returns>離散関数の値</returns>
		double getMarkerValue(std::size_t marker_idx) const override;
		/// <summary>
		/// 離散関数の値を 1 つ取得 (標本データインデックス指定)
		/// </summary>
		/// <param name="sample_idx">取得する標本データのインデックス</param>
		/// <returns>離散関数の値</returns>
		double getSampleValue(std::size_t sample_idx) const override;
		/// <summary>
		/// 離散関数の値一覧を取得
		/// </summary>
		/// <returns>離散関数の値一覧 (補間点を含む)</returns>
		const std::vector<double>& getMarkerValues(void) const override;
		/// <summary>
		/// 標本データの値一覧を生成し，取得
		/// </summary>
		/// <returns>標本データの値一覧 (補間点を含まない)</returns>
		std::vector<double> getSampleValues(void) const override;
		/// <summary>
		/// 離散関数における点インデックスを標本データインデックスに変換
		/// </summary>
		/// <param name="marker_idx">離散関数における点インデックス</param>
		/// <returns>標本データインデックス</returns>
		std::size_t markerIdx2SampleIdx(std::size_t marker_idx) const override;
		/// <summary>
		/// 標本データインデックスを離散関数における点インデックスに変換
		/// </summary>
		/// <param name="sample_idx">標本データインデックス</param>
		/// <returns>離散関数における点インデックス</returns>
		std::size_t sampleIdx2MarkerIdx(std::size_t sample_idx) const override;
		/// <summary>
		/// 離散関数における点の数を取得
		/// </summary>
		/// <returns>離散関数における点の数</returns>
		std::size_t getMarkersNum(void) const override;
		/// <summary>
		/// 標本データの数を取得
		/// </summary>
		/// <returns>標本データの数</returns>
		std::size_t getSamplesNum(void) const override;
		/// <summary>
		/// 関係性行列 Z を取得．<br/>
		/// 非ゼロ要素である，3 * marker_num 要素のみであることに注意．
		/// </summary>
		/// <returns>関係性行列 Z</returns>
		const std::vector<double>& getZ(void) const override;
		/// <summary>
		/// 理想値ベクトル b を取得
		/// </summary>
		/// <returns>理想値ベクトル b</returns>
		const std::vector<double>& getB(void) const override;
		/// <summary>
		/// 離散関数値 f を取得．<br/>
		/// getMarkerValues() と同等です．
		/// </summary>
		/// <returns>離散関数値 f</returns>
		const std::vector<double>& getF(void) const override;
	};
}

#endif // !OBSERVED_D_SPLINE_HPP_
