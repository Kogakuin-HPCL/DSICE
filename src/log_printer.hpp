#ifndef LOG_PRINTER_HPP_
#define LOG_PRINTER_HPP_

#include <memory>
#include <vector>
#include <unordered_set>
#include <string>
#include <ostream>

#include "dsice_search_space.hpp"
#include "dsice_operator.hpp"
#include "operation_integrator.hpp"

namespace dsice {

	/// <summary>
	/// Operator のログメッセージを生成，出力する機構
	/// </summary>
	template <typename T = double>
	class LogPrinter final {
	private:
		/// <summary>
		/// すでに追記を開始している，ログ出力先ファイル
		/// </summary>
		static std::unordered_set<std::string> appending_log_files;
		/// <summary>
		/// 出力先ストリームを生成
		/// </summary>
		/// <param name="file_path">出力先 (空文字で標準出力)</param>
		/// <returns>出力先ストリーム</returns>
		static std::shared_ptr<std::ostream> makeOutput(const std::string& file_path);
		/// <summary>
		/// 探索空間全域を走査し，各点の値を出力
		/// </summary>
		/// <param name="output">出力先ストリーム</param>
		/// <param name="database">対象データベース</param>
		/// <param name="default_value">データベースがデータを保有していない場合に代わりに使用する固定値</param>
		/// <param name="coordinate_fragment">再帰呼び出しで使用する，呼び出し元までに生成された座標の断片 (初回呼び出し時は空 vector を指定)</param>
		static void scanningAndOutoutSpace(std::shared_ptr<std::ostream> output, std::shared_ptr<DatabaseCore> database, double default_value, std::vector<std::size_t> coordinate_fragment = {});
	public:
		/// <summary>
		/// チューニング結果の通知文を生成，出力
		/// </summary>
		/// <param name="dsice_operator">性能パラメタを認識しない，探索機構側の統合機構</param>
		/// <param name="parameter_space">性能パラメタの最良設定値の座標</param>
		/// <param name="lower_is_better">低いほど良いか</param>
		/// <param name="file_path">出力先 (空文字で標準出力)</param>
		static void result(std::shared_ptr<Operator> dsice_operator, const std::vector<std::vector<T>>& parameter_space, bool lower_is_better, const std::string& file_path = "");
		/// <summary>
		/// ループごとの簡易的なログを生成，出力
		/// </summary>
		/// <param name="target_operator">ログ対象の Operator</param>
		/// <param name="file_path">出力先 (空文字で標準出力)</param>
		static void loop_simple(std::shared_ptr<Operator> target_operator, const std::string& file_path = "", bool for_sequential = true);
		/// <summary>
		/// DSICE Viewer 向けに DSICE 挙動ログファイルを出力
		/// </summary>
		/// <param name="target_operator">ログ対象の Operator</param>
		/// <param name="file_path">出力先 (空文字で標準出力)</param>
		static void dsiceLogForViewer(std::shared_ptr<Operator> target_operator, const std::string& file_path = "");
		/// <summary>
		/// DSICE Viewer 向けに探索空間全点のデータを出力．<br/>
		/// 未実測点の値は固定値で埋められます
		/// </summary>
		/// <param name="target_operator">ログ対象の Operator</param>
		/// <param name="file_path">出力先 (空文字で標準出力)</param>
		static void searchSpaceForViewer(std::shared_ptr<Operator> target_operator, const std::string& file_path = "", double default_value = 0);
	};
}

#endif // !LOG_PRINTER_HPP_
