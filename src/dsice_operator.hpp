#ifndef DSICE_OPERATOR_HPP_
#define DSICE_OPERATOR_HPP_

#include <memory>
#include <string>
#include <ios>

#include "dsice_database_core.hpp"

namespace dsice {

	/// <summary>
	/// 初期点の決定方法を定義
	/// </summary>
	enum class OperatorInitializeWay {
		/// <summary>
		/// 中心点
		/// </summary>
		CENTER,
		/// <summary>
		/// ユーザによる指定点
		/// </summary>
		SPECIFIED,
		/// <summary>
		/// 初期点探索
		/// </summary>
		INITIAL_SEARCH
	};

	/// <summary>
	/// DSICE の探索機構の本体．データベースの実体を保有
	/// </summary>
	class Operator {
	public:
		/// <summary>
		/// 探索基準点を取得
		/// </summary>
		/// <returns>探索基準点</returns>
		virtual const coordinate& getBaseCoordinate(void) const = 0;
		/// <summary>
		/// 次に実測すべき性能パラメタ性能値 (座標) を取得
		/// </summary>
		/// <returns>次に実測すべき性能パラメタ性能値 (座標)</returns>
		virtual const coordinate& getSuggested(void) = 0;
		/// <summary>
		/// 実測すべき性能パラメタ性能値 (座標) のリストを取得
		/// </summary>
		/// <returns>実測すべき性能パラメタ性能値 (座標) のリスト</returns>
		virtual const coordinate_list& getSuggestedList(void) = 0;
		/// <summary>
		/// 実測データを記録
		/// </summary>
		/// <param name="measured_coordinate">実測した座標</param>
		/// <param name="metric_value">実測した値</param>
		virtual void setMetricValue(const coordinate& measured_coordinate, double metric_value) = 0;
		/// <summary>
		/// 状態を更新
		/// </summary>
		virtual void updateState(void) = 0;
		/// <summary>
		/// 探索アルゴリズムが終了条件を満たしたかを取得．<br/>
		/// 終了条件を持たない探索アルゴリズムの場合，この関数の挙動は各実装クラス依存です．
		/// </summary>
		/// <returns>終了条件を満たしていれば true</returns>
		virtual bool isSearchFinished(void) = 0;
		/// <summary>
		/// その時点で探索アルゴリズムが最良と判定している性能パラメタ設定値 (座標) を取得
		/// </summary>
		/// <returns>探索アルゴリズムが最良と判定している性能パラメタ設定値 (座標)</returns>
		virtual const coordinate& getBestJudgedCoordinate(void) = 0;
		/// <summary>
		/// 実測された中で性能評価値が最良だった性能パラメタ設定値 (座標) を取得
		/// </summary>
		/// <returns>実測された中で性能評価値が最良だった性能パラメタ設定値 (座標)</returns>
		virtual const coordinate& getBestMeasuredCoordinate(void) = 0;
		/// <summary>
		/// アルゴリズムセットの ID 文字列を取得
		/// </summary>
		/// <returns>アルゴリズムセットの ID 文字列</returns>
		virtual std::string getAlgorithmId(void) const = 0;
		/// <summary>
		/// 使用しているデータベースを取得
		/// </summary>
		/// <returns>使用しているデータベース</returns>
		virtual std::shared_ptr<DatabaseCore> getDatabase(void) const = 0;
		/// <summary>
		/// これまでのループ回数 (updateState() が呼ばれた回数) を取得
		/// </summary>
		/// <returns>これまでのループ回数</returns>
		virtual std::size_t getLoopCount(void) const = 0;
		/// <summary>
		/// 実行中の探索モード名を取得
		/// </summary>
		/// <returns>実行中の探索モード名</returns>
		virtual std::string getSearchModeName(void) const = 0;
	};
}

#endif // !DSICE_OPERATOR_HPP_
