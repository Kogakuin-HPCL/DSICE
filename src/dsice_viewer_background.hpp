#ifndef DSICE_VIEWER_BACKGROUND_HPP_
#define DSICE_VIEWER_BACKGROUND_HPP_

#include "dsice_search_space.hpp"

namespace dsice {

#ifdef __cplusplus
	extern "C" {
#endif

		/// <summary>
		/// 2 点を結ぶ直線上の点の数を求める
		/// </summary>
		/// <param name="parameter_num">性能パラメタの数</param>
		/// <param name="space">探索空間</param>
		/// <param name="coordinate1">座標1</param>
		/// <param name="coordinate2">座標2</param>
		/// <returns>2 点を結ぶ直線上の点の数</returns>
		size_t DSICE_VIEWER_GET_LINE_POINTS_NUM(size_t parameter_num, const size_t* space, const size_t* coordinate1, const size_t* coordinate2);

		/// <summary>
		/// 2 点から直線を求める
		/// </summary>
		/// <param name="parameter_num">性能パラメタの数</param>
		/// <param name="space">探索空間</param>
		/// <param name="coordinate1">座標1</param>
		/// <param name="coordinate2">座標2</param>
		/// <returns>直線上の点座標リスト</returns>
		const size_t** DSICE_VIEWER_GET_LINE_POINTS(size_t parameter_num, const size_t* space, const size_t* coordinate1, const size_t* coordinate2);

#ifdef __cplusplus
	}
#endif

}

#endif // !DSICE_VIEWER_BACKGROUND_HPP_
