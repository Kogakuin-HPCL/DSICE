#ifndef AVERAGE_DOUBLE_HPP_
#define AVERAGE_DOUBLE_HPP_

#include <cstddef>
#include "dsice_metric.hpp"

namespace dsice {

	/// <summary>
	/// 倍精度浮動小数について常に平均をとり続ける蓄積数値型．<br/>
	/// 過去の生データを保持しない分軽量な実装を採用
	/// </summary>
	class AverageDouble final : public Metric {
	private:
		/// <summary>
		/// 性能評価値 (の過去の平均値)
		/// </summary>
		double value = 0;
		/// <summary>
		/// 性能評価値を登録した回数
		/// </summary>
		std::size_t count = 0;
	public:
		/// <summary>
		/// 値無しの場合のコンストラクタ．<br/>
		/// 初期値は 0 ですが，値は以降の平均値計算に用いられません．
		/// </summary>
		AverageDouble(void);
		/// <summary>
		/// 初期値を指定するコンストラクタ
		/// </summary>
		/// <param name="init_value">初期値</param>
		AverageDouble(double init_value);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		AverageDouble(const AverageDouble& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~AverageDouble(void);
		/// <summary>
		/// double 型への型変換 (getValue() の省略) を許可
		/// </summary>
		operator double() const override;
		/// <summary>
		/// 倍精度浮動小数点数表現を取得
		/// </summary>
		/// <returns>倍精度浮動小数点数で表現した場合の値</returns>
		double getValue(void) const override;
		/// <summary>
		/// 性能評価値を登録
		/// </summary>
		/// <param name="value">性能評価値</param>
		void setValue(double metric_value) override;
		/// <summary>
		/// 過去に保存されたデータの数を取得
		/// </summary>
		/// <returns>過去に保存されたデータの数</returns>
		std::size_t getRecordedCount(void) const override;
		/// <summary>
		/// コピーされた新しいインスタンスを Metric 型として作成．コピーコンストラクタ用．
		/// </summary>
		/// <returns>コピーされたインスタンス</returns>
		std::shared_ptr<Metric> makeCopy(void) const override;
	};
}

#endif // !AVERAGE_DOUBLE_HPP_
