#ifndef DSICE_METRIC_HPP_
#define DSICE_METRIC_HPP_

#include <memory>
#include <cstddef>

namespace dsice {

	/// <summary>
	/// 性能評価値を格納するための型．値が複数集まった場合の挙動を定義
	/// </summary>
	class Metric {
	public:
		/// <summary>
		/// double 型への型変換 (getValuee() の省略) を許可
		/// </summary>
		virtual operator double() const = 0;
		/// <summary>
		/// 性能評価値を取得
		/// </summary>
		/// <returns>性能評価値</returns>
		virtual double getValue(void) const = 0;
		/// <summary>
		/// 性能評価値を登録
		/// </summary>
		/// <param name="value">性能評価値</param>
		virtual void setValue(double value) = 0;
		/// <summary>
		/// 過去に保存されたデータの数を取得
		/// </summary>
		/// <returns>過去に保存されたデータの数</returns>
		virtual std::size_t getRecordedCount(void) const = 0;
		/// <summary>
		/// コピーされた新しいインスタンスを Metric 型として作成．コピーコンストラクタ用．
		/// </summary>
		/// <returns>コピーされたインスタンス</returns>
		virtual std::shared_ptr<Metric> makeCopy(void) const = 0;
	};
}

#endif