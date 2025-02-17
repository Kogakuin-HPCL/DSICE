#ifndef DSICE_PROCESS_LAUNCHER_HPP_
#define DSICE_PROCESS_LAUNCHER_HPP_

#include <memory>
#include <string>
#include <vector>
#include <future>

namespace dsice {

	/// <summary>
	/// コマンドライン引数経由でパラメタ設定値を指定し，プロセスを起動 (パラメタ設定値がない場合はコマンドライン引数は指定しない)
	/// </summary>
	/// <param name="command">プロセスを起動するコマンド (コマンドそのものに引数を渡す場合はここで設定してください)</param>
	/// <param name="parameters">コマンドライン引数で渡すパラメタ設定値</param>
	/// <param name="process_id">管理用 ID (DSICE 用．通常は 0．パラメタ設定値がない場合は無視されます)</param>
	template <typename T = double>
	void launchProcess(std::string command, const std::vector<T>& parameters, std::size_t process_id = 0);

	/// <summary>
	/// ファイル経由でパラメタ設定値を指定し，プロセスを起動
	/// </summary>
	/// <param name="command">プロセスを起動するコマンド (コマンドそのものに引数を渡す場合はここで設定してください)</param>
	/// <param name="parameters">ファイル経由で渡すパラメタ設定値</param>
	/// <param name="process_id">管理用 ID (DSICE 用．通常は 0)</param>
	template <typename T = double>
	void launchProcessWithFile(std::string command, const std::vector<T>& parameters, std::size_t process_id = 0);
}

#endif // !DSICE_PROCESS_LAUNCHER_HPP_
