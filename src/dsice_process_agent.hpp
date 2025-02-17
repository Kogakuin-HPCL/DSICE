#ifndef DSICE_PROCESS_AGENT_HPP_
#define DSICE_PROCESS_AGENT_HPP_

#include <string>
#include <vector>

namespace dsice {

	/// <summary>
	/// DSICE 本体への伝達ファイルにプロセスの開始を記述
	/// </summary>
	/// <param name="process_id">DSICE が管理するプロセス ID (不明な場合は 0)</param>
	void recordStartProcess(std::size_t dsice_process_id = 0);

	/// <summary>
	/// DSICE 本来への伝達ファイルにプロセスの終了を記述
	/// </summary>
	/// <param name="dsice_process_id">DSICE が管理するプロセス ID (不明な場合は 0)</param>
	void recordEndProcess(std::size_t dsice_process_id = 0);

	template <typename T = double>
	class ProcessAgent final {
	private:
		/// <summary>
		/// プロセス識別子
		/// </summary>
		std::size_t process_id = 0;
		/// <summary>
		/// 取得したパラメタ
		/// </summary>
		std::vector<T> parameter;
		/// <summary>
		/// 文字列 str を型 T の値に変換する (std::stod ファミリのテンプレート抽象化)
		/// </summary>
		/// <param name="str">変換対象の文字列</param>
		/// <param name="idx">文字列中で変換された箇所の直後を指すインデックス</param>
		/// <returns>変換後の数値</returns>
		inline T stoT(const std::string& str, std::size_t* idx = nullptr);
	public:
		/// <summary>
		/// DSICE が生成する一時ファイルから性能パラメタ設定値を取得するコンストラクタ
		/// </summary>
		ProcessAgent(void);
		/// <summary>
		/// コマンドライン引数から性能パラメタ設定値を取得するコンストラクタ
		/// </summary>
		/// <param name="argc">コマンドライン引数の数 (main 関数の第一引数)</param>
		/// <param name="argv">コマンドライン引数の中身 (main 関数の第二引数)</param>
		ProcessAgent(int argc, char** argv);
		/// <summary>
		/// プロセス識別子を取得
		/// </summary>
		/// <returns>プロセス識別子</returns>
		std::size_t getProcessId(void) const;
		/// <summary>
		/// パラメタ設定値を取得
		/// </summary>
		/// <returns>パラメタ設定値</returns>
		const std::vector<T> getParameter(void) const;
		/// <summary>
		/// DSICE に渡すために一時ファイルへ性能値を記録
		/// </summary>
		/// <param name="performance_value">性能値</param>
		void recordPerformanceValue(double performance_value) const;
	};
}

#endif // !DSICE_PROCESS_AGENT_HPP_
