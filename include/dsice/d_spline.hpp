#ifndef D_SPLINE_HPP_
#define D_SPLINE_HPP_

#include <vector>

namespace dsice {

	/// <summary>
	/// d-Spline (離散スプライン) 関数
	/// </summary>
	class DSpline {
	public:
		/// <summary>
		/// 標本データを追加
		/// </summary>
		/// <param name="sample_idx">標本データのインデックス</param>
		/// <param name="sample_value">標本データの値</param>
		virtual void update(std::size_t sample_idx, double sample_value) = 0;
		/// <summary>
		/// 標本データを追加
		/// </summary>
		/// <param name="sample_data">標本データのインデックスと値の組</param>
		virtual void update(const std::pair<std::size_t, double>& sample_data) = 0;
		/// <summary>
		/// 複数の標本データを一括追加
		/// </summary>
		/// <param name="sample_data">標本データのインデックスと値の組の一覧</param>
		virtual void update(const std::vector<std::pair<std::size_t, double>>& sample_data) = 0;
		/// <summary>
		/// 離散関数の値を 1 つ取得 (離散点インデックス指定)
		/// </summary>
		/// <param name="marker_idx">離散関数における点インデックス</param>
		/// <returns>離散関数の値</returns>
		virtual double getMarkerValue(std::size_t marker_idx) const = 0;
		/// <summary>
		/// 離散関数の値を 1 つ取得 (標本データインデックス指定)
		/// </summary>
		/// <param name="sample_idx">取得する標本データのインデックス</param>
		/// <returns>離散関数の値</returns>
		virtual double getSampleValue(std::size_t sample_idx) const = 0;
		/// <summary>
		/// 離散関数の値一覧を取得
		/// </summary>
		/// <returns>離散関数の値一覧 (補間点を含む)</returns>
		virtual const std::vector<double>& getMarkerValues(void) const = 0;
		/// <summary>
		/// 標本データの値一覧を生成し，取得
		/// </summary>
		/// <returns>標本データの値一覧 (補間点を含まない)</returns>
		virtual std::vector<double> getSampleValues(void) const = 0;
		/// <summary>
		/// 離散関数における点インデックスを標本データインデックスに変換
		/// </summary>
		/// <param name="marker_idx">離散関数における点インデックス</param>
		/// <returns>標本データインデックス</returns>
		virtual std::size_t markerIdx2SampleIdx(std::size_t marker_idx) const = 0;
		/// <summary>
		/// 標本データインデックスを離散関数における点インデックスに変換
		/// </summary>
		/// <param name="sample_idx">標本データインデックス</param>
		/// <returns>離散関数における点インデックス</returns>
		virtual std::size_t sampleIdx2MarkerIdx(std::size_t sample_idx) const = 0;
		/// <summary>
		/// 離散関数における点の数を取得
		/// </summary>
		/// <returns>離散関数における点の数</returns>
		virtual std::size_t getMarkersNum(void) const = 0;
		/// <summary>
		/// 標本データの数を取得
		/// </summary>
		/// <returns>標本データの数</returns>
		virtual std::size_t getSamplesNum(void) const = 0;
		/// <summary>
		/// 関係性行列 Z を取得．<br/>
		/// 非ゼロ要素である，3 * marker_num 要素のみであることに注意．
		/// </summary>
		/// <returns>関係性行列 Z</returns>
		virtual const std::vector<double>& getZ(void) const = 0;
		/// <summary>
		/// 理想値ベクトル b を取得
		/// </summary>
		/// <returns>理想値ベクトル b</returns>
		virtual const std::vector<double>& getB(void) const = 0;
		/// <summary>
		/// 離散関数値 f を取得．<br/>
		/// getMarkerValues() と同等です．
		/// </summary>
		/// <returns>離散関数値 f</returns>
		virtual const std::vector<double>& getF(void) const = 0;
	};
}

#endif // !D_SPLINE_HPP_
