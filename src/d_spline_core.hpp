#ifndef D_SPLINE_CORE_HPP_
#define D_SPLINE_CORE_HPP_

#include <vector>

namespace dsice {

	/// <summary>
	/// d-Spline 関数の算出理論部分 (拡張性を持たせるため，このクラスにおいては点と点の間に補助点を設けない)．
	/// </summary>
	class DSplineCore final {
	private:
		/// <summary>
		/// 離散関数の点の数
		/// </summary>
		std::size_t marker_num = 0;
		/// <summary>
		/// 離散点同士の関係性を表現する行列 (実際には QR 分解した後の R の帯幅 3 部分のみ)
		/// </summary>
		std::vector<double> Z;
		/// <summary>
		/// 離散点同士の各関係性の理想値を並べたベクトル (実際には Q^T * b)
		/// </summary>
		std::vector<double> b;
		/// <summary>
		/// 近似後の値を並べたベクトル (d-Spline そのもの)
		/// </summary>
		std::vector<double> f;
		/// <summary>
		/// 関係性行列 Z に 1 行追加したものに対する Givens 変換
		/// </summary>
		/// <param name="target_col">0 にしたい要素の列インデックス</param>
		/// <param name="appended_row1">追加した行の 1 要素目</param>
		/// <param name="appended_row2">追加した行の 2 要素目</param>
		/// <param name="appended_b">追加した行の分のベクトル b の要素</param>
		void givensRotate(std::size_t target_col, double& appended_row1, double& appended_row2, double& appended_b);
		/// <summary>
		/// 新しいデータを追加し，Z に QR 分解を実施 (実際にはその後両辺に Q^T を左からかけた結果が残る)
		/// </summary>
		/// <param name="index">新しいデータの離散点インデックス</param>
		/// <param name="value">新しいデータの値</param>
		void updateZAndB(std::size_t index, double value);
		/// <summary>
		/// Z (理論上の Q^T * Z) と b (理論上の Q^T * b) のデータから近似関数値のベクトル f を求める
		/// </summary>
		void updateF(void);
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="value_num">離散点の数</param>
		/// <param name="alpha">滑らかさを決定する重み係数</param>
		DSplineCore(std::size_t value_num, double alpha = 0.1);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		DSplineCore(const DSplineCore& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~DSplineCore(void);
		/// <summary>
		/// データを追加
		/// </summary>
		/// <param name="target_index">追加するデータの離散点インデックス</param>
		/// <param name="value">追加するデータの値</param>
		void update(std::size_t target_index, double value);
		/// <summary>
		/// データを追加
		/// </summary>
		/// <param name="data">追加するデータの離散点インデックスと値の組</param>
		void update(const std::pair<std::size_t, double>& data);
		/// <summary>
		/// 複数データを一括で追加
		/// </summary>
		/// <param name="data">追加するデータの離散点インデックスと値の組リスト</param>
		void update(const std::vector<std::pair<std::size_t, double>>& data);
		/// <summary>
		/// 離散関数の点の数を取得
		/// </summary>
		/// <returns>離散関数の点の数</returns>
		std::size_t getMarkerNum(void) const;
		/// <summary>
		/// 関係性行列 Z を取得．<br/>
		/// 非ゼロ要素である，3 * marker_num 要素のみであることに注意．
		/// </summary>
		/// <returns>関係性行列 Z</returns>
		const std::vector<double>& getZ(void) const;
		/// <summary>
		/// 理想値ベクトル b を取得
		/// </summary>
		/// <returns>理想値ベクトル b</returns>
		const std::vector<double>& getB(void) const;
		/// <summary>
		/// 離散関数値 f を取得
		/// </summary>
		/// <returns>離散関数値 f</returns>
		const std::vector<double>& getF(void) const;
	};
}

#endif // !D_SPLINE_CORE_HPP_
