#ifndef D_SPLINE_C_WRAPPER_HPP_
#define D_SPLINE_C_WRAPPER_HPP_

#include <memory>
#include <unordered_map>

#include "d_spline.hpp"
#include "equally_spaced_d_spline.hpp"

namespace dsice {

	/// <summary>
	/// d-Spline の識別子を格納する型
	/// </summary>
	typedef size_t d_spline_id;

	/// <summary>
	/// DSpline の，純粋 C で利用するインスタンスを保持
	/// </summary>
	std::unordered_map<d_spline_id, std::shared_ptr<DSpline>>& getDSplinesMap(void);

	/// <summary>
	/// DSpline が生成する標本データ点の値一覧を，純粋 C で利用するためのバッファを保持
	/// </summary>
	std::unordered_map<d_spline_id, std::vector<double>>& getDSplineSampleValuesBuffers(void);

	/// <summary>
	/// 指定 ID の d-Spline インスタンスを取得
	/// </summary>
	/// <param name="id">d-Spline インスタンスを指定する ID</param>
	/// <returns>d-Spline インスタンス</returns>
	std::shared_ptr<DSpline> getDSpline(d_spline_id id);

	/// <summary>
	/// 新しい d-Spline 識別子を生成，取得
	/// </summary>
	/// <returns>新しい d-Spline 識別子</returns>
	d_spline_id createNewDSplineId(void);

#ifdef __cplusplus
	extern "C" {
#endif

		/// <summary>
		/// 等間隔 d-Spline インスタンスを生成
		/// </summary>
		/// <param name="sample_num">標本点の数</param>
		/// <param name="alpha">滑らかさを決定する重み係数</param>
		/// <param name="interpolated_point_num">標本データ点間を埋め，滑らかな曲線を表現するための点の数．d-Spline は 3 次式を用いているため，特別な理由が無ければデフォルトの 2 点が自然です．</param>
		/// <returns>生成された d-Spline インスタンスを表す ID</returns>
		d_spline_id DSPLINE_EQ_CREATE(size_t sample_num, double alpha, size_t interpolated_point_num);

		/// <summary>
		/// 等間隔 d-Spline インスタンスを生成 (標準的な設定を利用 : alpha = 0.1, interpolated_point_num = 2)
		/// </summary>
		/// <param name="sample_num">標本点の数</param>
		/// <returns>生成された d-Spline インスタンスを表す ID</returns>
		d_spline_id DSPLINE_EQ_CREATE_STANDARD(size_t sample_num);

		/// <summary>
		/// d-Spline インスタンスを削除
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		void DSPLINE_DELETE(d_spline_id id);

		/// <summary>
		/// d-Spline に標本データを 1 つ追加
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <param name="sample_idx">標本データのインデックス</param>
		/// <param name="sample_value">標本データの値</param>
		void DSPLINE_UPDATE(d_spline_id id, size_t sample_idx, double sample_value);

		/// <summary>
		/// d-Spline に標本データを一括で追加
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <param name="sample_num">標本データの数</param>
		/// <param name="sample_idx_list">標本データのインデックス一覧</param>
		/// <param name="sample_value_list">標本データの値一覧</param>
		void DSPLINE_UPDATE_MANY(d_spline_id id, size_t sample_num, size_t* sample_idx_list, double* sample_value_list);

		/// <summary>
		/// 離散関数の値を 1 つ取得 (離散点インデックス指定)
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <param name="marker_idx">離散関数における点インデックス</param>
		/// <returns>離散関数の値</returns>
		double DSPLINE_GET_MARKER_VALUE(d_spline_id id, size_t marker_idx);

		/// <summary>
		/// 離散関数の値を 1 つ取得 (標本データインデックス指定)
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <param name="sample_idx">取得する標本データのインデックス</param>
		/// <returns>離散関数の値</returns>
		double DSPLINE_GET_SAMPLE_VALUE(d_spline_id id, size_t sample_idx);

		/// <summary>
		/// 離散関数の値一覧を取得．<br/>
		/// 一覧に含まれる値の個数は DSPLINE_GET_MARKERS_NUM 関数を呼び出して取得してください．
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <returns>離散関数の値一覧</returns>
		const double* DSPLINE_GET_MARKER_VALUES(d_spline_id id);

		/// <summary>
		/// 標本データの値一覧を取得．<br/>
		/// 一覧に含まれる値の個数は DSPLINE_GET_SAMPLES_NUM 関数を呼び出して取得してください．
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <returns>標本データの値一覧</returns>
		const double* DSPLINE_GET_SAMPLE_VALUES(d_spline_id id);

		/// <summary>
		/// 離散関数における点インデックスを標本データインデックスに変換
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <param name="marker_idx">離散関数における点インデックス</param>
		/// <returns>標本データインデックス</returns>
		size_t DSPLINE_MARKER_IDX_2_SAMPLE_IDX(d_spline_id id, size_t marker_idx);

		/// <summary>
		/// 標本データインデックスを離散関数における点インデックスに変換
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <param name="sample_idx">標本データインデックス</param>
		/// <returns>離散関数における点インデックス</returns>
		size_t DSPLINE_SAMPLE_IDX_2_MARKER_IDX(d_spline_id id, size_t sample_idx);

		/// <summary>
		/// 離散関数を構成する離散点の個数を取得
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <returns>離散関数を構成する離散点の個数</returns>
		size_t DSPLINE_GET_MARKERS_NUM(d_spline_id id);

		/// <summary>
		/// 標本データの数を取得
		/// </summary>
		/// <param name="id">d-Spline インスタンスを指定する ID</param>
		/// <returns>標本データの数</returns>
		size_t DSPLINE_GET_SAMPLES_NUM(d_spline_id id);

#ifdef __cplusplus
	}
#endif
}

#endif // !D_SPLINE_C_WRAPPER_HPP_
