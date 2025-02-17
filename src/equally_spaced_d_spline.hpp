#ifndef EQUALLY_SPACED_D_SPLINE_HPP_
#define EQUALLY_SPACED_D_SPLINE_HPP_

#include <vector>

#include "d_spline_core.hpp"
#include "d_spline.hpp"

namespace dsice {

	/// <summary>
	/// 等間隔データ点の d-Spline 関数
	/// </summary>
	class EquallySpacedDSpline final : public DSpline {
	private:
		/// <summary>
		/// 標本データにあたる点の数
		/// </summary>
		std::size_t samples_num;
		/// <summary>
		/// 離散関数全体の点の数
		/// </summary>
		std::size_t markers_num;
		/// <summary>
		/// 標本データ点間を埋め，滑らかな曲線を表現するための点の数
		/// </summary>
		std::size_t interpolated_num;
		/// <summary>
		/// d-Spline の本体
		/// </summary>
		DSplineCore d_spline;
		/// <summary>
		/// 離散関数の各離散点から最近傍の標本データ点インデックスを格納 (算出時の条件分岐が激しいので事前に計算しておく)
		/// </summary>
		std::vector<std::size_t> nearest_sample_idx;
		/// <summary>
		/// インデクス変換配列の生成
		/// </summary>
		void setupNearestSampleIdx(void);
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="sample_value_num">標本データ点の数</param>
		/// <param name="alpha">滑らかさを決定する重み係数</param>
		/// <param name="interpolated_point_num">標本データ点間を埋め，滑らかな曲線を表現するための点の数．d-Spline は 3 次式を用いているため，特別な理由が無ければデフォルトの 2 点が自然です．</param>
		EquallySpacedDSpline(std::size_t sample_value_num, double alpha = 0.1, std::size_t interpolated_point_num = 2);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		EquallySpacedDSpline(const EquallySpacedDSpline& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~EquallySpacedDSpline(void);
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

#endif // !EQUALLY_SPACED_D_SPLINE_HPP_
