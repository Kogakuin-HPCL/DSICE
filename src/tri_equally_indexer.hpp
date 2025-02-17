#ifndef TRI_EQUALLY_INDEXER_HPP_
#define TRI_EQUALLY_INDEXER_HPP_

#include <vector>

namespace dsice {

	/// <summary>
	/// 指定されたインデックス (つまり離散値) 範囲を繰り返し 3 等分する場合の分割点を算出する機構
	/// </summary>
	class TriEquallyIndexer final {
	private:
		/// <summary>
		/// 等分インデックスリスト
		/// </summary>
		std::vector<std::size_t> indexes;
	public:
		/// <summary>
		/// 3 分割時の分割点 2 点のみを算出
		/// </summary>
		/// <param name="left">分割範囲の左端</param>
		/// <param name="right"></param>
		/// <returns>3 分割時の分割点 pair</returns>
		static std::pair<std::size_t, std::size_t> getTriPoints(std::size_t left, std::size_t right);
		/// <summary>
		/// コンストラクタ (生成される等分インデックスの範囲は [left, right]，つまり left と right の両方を含みます)
		/// </summary>
		/// <param name="left>等分する範囲の最左インデックス</param>
		/// <param name="right">等分する範囲の最右インデックス</param>
		TriEquallyIndexer(std::size_t left, std::size_t right);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		TriEquallyIndexer(const TriEquallyIndexer& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~TriEquallyIndexer(void);
		/// <summary>
		/// インデックス点の数を取得
		/// </summary>
		/// <returns>インデックス点の数</returns>
		std::size_t getNum(void) const;
		/// <summary>
		/// 等分インデックスリストを取得
		/// </summary>
		/// <returns>等分インデックスリスト</returns>
		const std::vector<std::size_t>& getIndexes(void) const;
	};
}

#endif // !TRI_EQUALLY_INDEXER_HPP_
