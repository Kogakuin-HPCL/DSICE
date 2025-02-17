#ifndef OVERWRITTEN_DOUBLE_HPP_
#define OVERWRITTEN_DOUBLE_HPP_

#include <cstddef>
#include "dsice_metric.hpp"

namespace dsice {

	/// <summary>
	/// 倍精度浮動小数点数について最新の値のみを保持
	/// </summary>
	class OverwrittenDouble final : public Metric {
	private:
		double value = 0;
		std::size_t count = 0;
	public:
		/// <summary>
		/// 値無しの場合のコンストラクタ．<br/>
		/// 初期値は 0 ですが，保存回数にはカウントされません．
		/// </summary>
		OverwrittenDouble(void);
		/// <summary>
		/// 初期値を指定するコンストラクタ
		/// </summary>
		/// <param name="init_value">初期値</param>
		OverwrittenDouble(double init_value);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		OverwrittenDouble(const OverwrittenDouble& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~OverwrittenDouble(void);
		/// <summary>
		/// double 型への型変換 (getValuee() の省略) を許可
		/// </summary>
		operator double() const override;
		/// <summary>
		/// 性能評価値を取得
		/// </summary>
		/// <returns>性能評価値</returns>
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

#endif // !OVERWRITTEN_DOUBLE_HPP_
