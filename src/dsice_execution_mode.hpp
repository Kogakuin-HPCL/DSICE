#ifndef DSICE_EXECUTION_MODE_HPP_
#define DSICE_EXECUTION_MODE_HPP_

namespace dsice {

	/// <summary>
	/// 探索機構を指定する識別子
	/// </summary>
	enum class ExecutionMode {
		/// <summary>
		/// そのバージョンで推奨されるモードを自動選択
		/// </summary>
		RECOMMENDED,
		/// <summary>
		/// IPPE 法による，1 パラメタ限定の逐次探索
		/// </summary>
		S_IPPE,
		/// <summary>
		/// 反復一次元 d-Spline 探索法による逐次探索
		/// </summary>
		S_2017,
		/// <summary>
		/// multi-steps search 方式の反復一次元 d-Spline 探索法による逐次探索
		/// </summary>
		S_2018,
		/// <summary>
		/// 反復放射型 d-Spline 探索法による並列探索
		/// </summary>
		P_2024B
	};
}

#endif // !DSICE_EXECUTION_MODE_HPP_
