#ifndef DSICE_SUPERCOMPUTERS_HPP_
#define DSICE_SUPERCOMPUTERS_HPP_

namespace dsice {

	//
	// 注: dsice_submit_job コマンドの数値と合わせてください
	//

	/// <summary>
	///	プロセス起動で実行させるマシンの種類
	/// </summary>
	enum class SuperComputer {
		/// <summary>
		/// スパコン以外
		/// </summary>
		PLAIN = 0,
		/// <summary>
		/// Wisteria/BDEC-01 (東京大学, 2021)
		/// </summary>
		TOKYO_UNIV_WISTERIA_01,
		/// <summary>
		/// 不老 (名古屋大学, 2020)
		/// </summary>
		NAGOYA_UNIV_FLOW_01,
		/// <summary>
		/// 玄界 (九州大学, 2024)
		/// </summary>
		KYUSHU_UNIV_GENKAI
	};
}

#endif // !DSICE_SUPERCOMPUTERS_HPP_
