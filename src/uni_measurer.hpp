#ifndef UNI_MEASURER_HPP_
#define UNI_MEASURER_HPP_

#include <memory>
#include <unordered_set>

#include "dsice_search_space.hpp"
#include "dsice_search_engine.hpp"

namespace dsice {

	/// <summary>
	/// 1 点のみ指定して実測させる機構．他の Searcher と同じインタフェースで実測させるためのラッパ
	/// </summary>
	class UniMeasurer final : public SearchEngine {
	private:
		/// <summary>
		/// 実測対象座標 (メンバメソッドでリストを返すために一覧として持つ)
		/// </summary>
		coordinate_list target;
		/// <summary>
		/// 実測データの更新用バッファ (座標)
		/// </summary>
		std::unordered_set<coordinate> measured_coordinate_buffer;
		/// <summary>
		/// 探索済点 (メンバメソッドで返すために set を持つ)
		/// </summary>
		std::unordered_set<coordinate> measured_coordinates;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="target_coordinate">実測対象座標</param>
		UniMeasurer(const coordinate& target_coordinate);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		UniMeasurer(const UniMeasurer& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~UniMeasurer(void);
		/// <summary>
		/// 次に実測すべき性能パラメタ性能値 (座標) を取得
		/// </summary>
		/// <returns>次に実測すべき性能パラメタ性能値 (座標)</returns>
		const coordinate& getSuggested(void) const override;
		/// <summary>
		/// 実測すべき性能パラメタ性能値 (座標) のリストを取得
		/// </summary>
		/// <returns>実測すべき性能パラメタ性能値 (座標) のリスト</returns>
		const coordinate_list& getSuggestedList(void) const override;
		/// <summary>
		/// 実測データを記録
		/// </summary>
		/// <param name="measured_coordinate">実測した座標</param>
		/// <param name="metric_value">実測した値</param>
		void setMetricValue(const coordinate& measured_coordinate, double metric_value) override;
		/// <summary>
		/// 記録されている実測データを用いて状態を更新
		/// </summary>
		/// <returns>更新用バッファにデータがあり，更新されたら true</returns>
		bool updateState(void) override;
		/// <summary>
		/// 指定座標が探索されたかどうか
		/// </summary>
		/// <returns>指定座標が探索されたら true</returns>
		bool isSearchFinished(void) const override;
		/// <summary>
		/// 探索機構が探索対象とする座標リストを取得
		/// </summary>
		/// <returns>探索機構が探索対象とする座標リスト</returns>
		const coordinate_list& getTargetCoordinate(void) const override;
		/// <summary>
		/// 対象座標内で探索済の座標リストを取得
		/// </summary>
		/// <returns>対象座標内で探索済の座標リスト</returns>
		const std::unordered_set<coordinate>& getMeasuredCoordinates(void) const override;
		/// <summary>
		/// 探索アルゴリズムが最良と判定した性能パラメタ設定値 (座標) を取得．<br/>
		/// この機構においては常に実測対象座標を返します．
		/// </summary>
		/// <returns>実測対象座標</returns>
		const coordinate& getBestJudgedCoordinate(void) const override;
		/// <summary>
		/// 探索アルゴリズムによって実測されたうち，性能評価値が最良だった性能パラメタ設定値 (座標) を取得．<br/>
		/// この機構においては常に実測対象座標を返します．
		/// </summary>
		/// <returns>実測対象座標</returns>
		const coordinate& getBestMeasuredCoordinate(void) const override;
	};
}

#endif // !UNI_MEASURER_HPP_