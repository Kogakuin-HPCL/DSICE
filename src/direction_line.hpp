#ifndef DIRECTION_LINE_HPP_
#define DIRECTION_LINE_HPP_

#include <memory>
#include <vector>

#include "dsice_search_space.hpp"

namespace dsice {

	/// <summary>
	/// 探索空間における 2 点を結んだ直線の方向 (角度) 情報を表現
	/// </summary>
	class DirectionLine final {
	private:
		/// <summary>
		/// 順方向情報を保持
		/// </summary>
		std::vector<long long> direction;
		/// <summary>
		/// 逆方向情報を保持 (direction の符号反転)
		/// </summary>
		std::vector<long long> reverse;
		/// <summary>
		/// 方向情報の次元数 (0 以外の数)
		/// </summary>
		std::size_t dimension = 0;
	public:
		/// <summary>
		/// 方向情報を利用するコンストラクタ
		/// </summary>
		/// <param name="direction">各軸に対する重みつき方向情報を格納した vector</param>
		DirectionLine(const std::vector<char>& direction);
		/// <summary>
		/// 方向情報を利用するコンストラクタ
		/// </summary>
		/// <param name="direction">各軸に対する重みつき方向情報を格納した vector</param>
		DirectionLine(const std::vector<short>& direction);
		/// <summary>
		/// 方向情報を利用するコンストラクタ
		/// </summary>
		/// <param name="direction">各軸に対する重みつき方向情報を格納した vector</param>
		DirectionLine(const std::vector<int>& direction);
		/// <summary>
		/// 方向情報を利用するコンストラクタ
		/// </summary>
		/// <param name="direction">各軸に対する重みつき方向情報を格納した vector</param>
		DirectionLine(const std::vector<long>& direction);
		/// <summary>
		/// 方向情報を利用するコンストラクタ
		/// </summary>
		/// <param name="direction">各軸に対する重みつき方向情報を格納した vector</param>
		DirectionLine(const std::vector<long long>& direction);
		/// <summary>
		/// 2 点の座標から方向情報を取得するコンストラクタ
		/// </summary>
		/// <param name="coordinate1">座標1</param>
		/// <param name="coordinate2">座標2</param>
		DirectionLine(const coordinate& coordinate1, const coordinate& coordinate2);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		DirectionLine(const DirectionLine& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~DirectionLine(void);
		/// <summary>
		/// 方向の一致を判定
		/// </summary>
		/// <param name="other">比較対象</param>
		/// <returns>同一方向なら true</returns>
		bool operator ==(const DirectionLine& other) const;
		/// <summary>
		/// 方向の不一致を判定
		/// </summary>
		/// <param name="other">比較対象</param>
		/// <returns>同一方向でない場合に true</returns>
		bool operator !=(const DirectionLine& other) const;
		/// <summary>
		/// 順方向情報を取得．<br/>
		/// ここで，順方向とは影響する最小インデックス軸の符号が正である方向を指します．<br/>
		/// (例) (+, +) や (+, -)，(+, 0)，(0, +)
		/// </summary>
		/// <returns>順方向情報の vector</returns>
		const std::vector<long long>& getDirection(void) const;
		/// <summary>
		/// 逆方向情報を取得．<br/>
		/// ここで，逆方向とは影響する最小インデックス軸の符号が負である方向を指します．<br/>
		/// (例) (-, -) や (-, +)，(-, 0)，(0, -)
		/// </summary>
		/// <returns>逆方向情報の vector</returns>
		const std::vector<long long>& getReverse(void) const;
		/// <summary>
		/// 1 次元以上の方向情報を持つかどうかを取得
		/// </summary>
		/// <returns>要素無し，または全要素 0 で false．それ以外は true</returns>
		bool hasDimension(void) const;
		/// <summary>
		/// 方向情報の次元数 (方向情報が持つ 0 以外の数) を取得
		/// </summary>
		/// <returns>方向情報の次元数</returns>
		std::size_t getDirectionDimension(void) const;
	};
}

namespace std {

	/// <summary>
	/// dsice::DirectionLine 型を検索キーで使うためのハッシュ定義
	/// </summary>
	template<>
	class hash<dsice::DirectionLine> {
	public:
		size_t operator()(const dsice::DirectionLine& dir) const {
			std::hash<std::size_t> hasher;
			size_t seed = 0;
			for (long long i : dir.getDirection()) {
				seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};
}

#endif // !DIRECTION_LINE_HPP_
