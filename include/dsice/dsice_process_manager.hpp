#ifndef DSICE_PROCESS_MANAGER_HPP_
#define DSICE_PROCESS_MANAGER_HPP_

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <chrono>

#include "dsice_supercomputers.hpp"

namespace dsice {

	/// <summary>
	/// ユーザプログラムをプロセス形式で実行させる実測機構
	/// </summary>
	template <typename T = double>
	class ProcessManager final {
	private:
		/// <summary>
		/// この機構が動作するディレクトリのパス
		/// </summary>
		std::filesystem::path current_path;
		/// <summary>
		/// 前回起動されたプロセスの識別子
		/// </summary>
		std::size_t dsice_process_id = 0;
		/// <summary>
		/// 監視中プロセスの識別子一覧
		/// </summary>
		std::vector<std::size_t> running_processes;
		/// <summary>
		/// DSICE プロセスに対応する，OS 指定の PID
		/// </summary>
		std::unordered_map<std::size_t, std::size_t> running_pid;
		/// <summary>
		/// 監視中プロセスの実行ディレクトリ一覧
		/// </summary>
		std::vector<std::filesystem::path> running_dir;
		/// <summary>
		/// ユーザプログラムを実行するコマンドパス (絶対パスまたは PATH 依存パス)
		/// </summary>
		std::filesystem::path execution_command;
		/// <summary>
		/// ユーザプログラムを実行する際のコマンドオプション
		/// </summary>
		std::string command_options;
		/// <summary>
		/// 実行ごとにコピーされるディレクトリの親ディレクトリ
		/// </summary>
		std::filesystem::path work_dir_prefix;
		/// <summary>
		/// 実測ごとにコピーされるディレクトリの名前
		/// </summary>
		std::filesystem::path original_work_dir_name;
		/// <summary>
		/// 実行ごとにコピーされるディレクトリの内側で実行ファイルがどこにあるか
		/// </summary>
		std::filesystem::path executed_file_path_suffix;
		/// <summary>
		/// 実行ファイルの名前
		/// </summary>
		std::filesystem::path executed_file_name;
		/// <summary>
		/// 対象が PATH によるコマンドかどうか
		/// </summary>
		bool command_in_bin = false;
		/// <summary>
		/// 実行ごとに作業ディレクトリをコピーするか
		/// </summary>
		bool do_copy = false;
		/// <summary>
		/// 時間制限などによるプロセス監視放棄を設定するか
		/// </summary>
		bool process_kill_reservation_on = false;
		/// <summary>
		/// 実行される実行ファイルが，実行ごとにコピーされるディレクトリの中にあるかどうか
		/// </summary>
		bool is_command_in_copied_dir = false;
		/// <summary>
		/// 次に launchProcess を呼び出した際に状態をアップデートするか
		/// </summary>
		bool reserved_update = true;
		/// <summary>
		/// 実行するマシン
		/// </summary>
		SuperComputer execute_machine = SuperComputer::PLAIN;
		/// <summary>
		/// プロセスの監視を中断する際に実行するコマンド
		/// </summary>
		std::string kill_process_command = "dsice_del";
		/// <summary>
		/// プロセス起動時にプロセス状態を監視する時間間隔 (秒)
		/// </summary>
		std::chrono::seconds watch_duration_of_launch_s = std::chrono::seconds(2);
		/// <summary>
		/// プロセス起動時にプロセス状態を監視する時間間隔 (分)
		/// </summary>
		std::chrono::minutes watch_duration_of_launch_m = std::chrono::minutes(0);
		/// <summary>
		/// プロセス起動時にプロセス状態を監視する時間間隔 (時)
		/// </summary>
		std::chrono::hours watch_duration_of_launch_h = std::chrono::hours(0);
		/// <summary>
		/// プロセス起動時に許容する待機時間 (秒)
		/// </summary>
		std::chrono::seconds wait_duration_of_launch_s = std::chrono::seconds(30);
		/// <summary>
		/// プロセス起動時に許容する待機時間 (分)
		/// </summary>
		std::chrono::minutes wait_duration_of_launch_m = std::chrono::minutes(0);
		/// <summary>
		/// プロセス起動時に許容する待機時間 (時)
		/// </summary>
		std::chrono::hours wait_duration_of_launch_h = std::chrono::hours(0);
		/// <summary>
		/// プロセス実行中にプロセス状態を監視する時間間隔 (秒)
		/// </summary>
		std::chrono::seconds watch_duration_of_running_s = std::chrono::seconds(0);
		/// <summary>
		/// プロセス実行中にプロセス状態を監視する時間間隔 (分)
		/// </summary>
		std::chrono::minutes watch_duration_of_running_m = std::chrono::minutes(1);
		/// <summary>
		/// プロセス実行中にプロセス状態を監視する時間間隔 (時)
		/// </summary>
		std::chrono::hours watch_duration_of_running_h = std::chrono::hours(0);
		/// <summary>
		/// プロセス実行中に許容する待機時間 (秒)
		/// </summary>
		std::chrono::seconds wait_duration_of_running_s = std::chrono::seconds(0);
		/// <summary>
		/// プロセス実行中に許容する待機時間 (分)
		/// </summary>
		std::chrono::minutes wait_duration_of_running_m = std::chrono::minutes(30);
		/// <summary>
		/// プロセス実行中に許容する待機時間 (時)
		/// </summary>
		std::chrono::hours wait_duration_of_running_h = std::chrono::hours(0);
		/// <summary>
		/// 一時ファイルから PID を取得
		/// </summary>
		/// <param name="pid_recorded_path">一時ファイルの位置</param>
		/// <return>取得した PID</return>
		std::size_t getPID(std::filesystem::path pid_recorded_path);
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		ProcessManager(void);
		/// <summary>
		/// スパコンでの実行を指定し，ジョブスクリプトを指定する
		/// </summary>
		/// <param name="machine">実行するスーパーコンピュータ</param>
		/// <param name="script_path">投入するジョブスクリプトファイル</param>
		void setJobScript(SuperComputer machine, const std::string& script_path);
		/// <summary>
		/// 一般的なマシンでの実行を指定し，実行されるコマンドとそのオプションを指定する
		/// </summary>
		/// <param name="command">ユーザプログラムを実行するコマンド</param>
		/// <param name="options">ユーザプログラムを実行する際のコマンドオプション</param>
		void setLaunchCommand(const std::string& command, std::string options = "");
		/// <summary>
		/// 一般的なマシンでの実行を指定し，実行されるコマンドとそのオプションを指定する
		/// </summary>
		/// <param name="command">ユーザプログラムを実行するコマンド</param>
		/// <param name="options">ユーザプログラムを実行する際のコマンドオプション</param>
		void setLaunchCommand(const std::string& command, const std::vector<std::string>& options);
		/// <summary>
		/// ユーザプログラムを実行する際に実行環境を破壊しないよう，毎回ディレクトリをコピーするように指定
		/// </summary>
		/// <param name="directory_path">実行ごとに毎回コピーされるディレクトリのパス</param>
		void setCopiedDirectory(std::string directory_path);
		/// <summary>
		/// 時間制限などによるプロセス監視放棄を指定
		/// </summary>
		/// <param name="limit_on">指定する場合 true</param>
		void config_switchWaitLimit(bool limit_on = true);
		/// <summary>
		/// プロセス起動時に行う状態監視の時間間隔を設定
		/// </summary>
		/// <param name="hours">時</param>
		/// <param name="minutes">分</param>
		/// <param name="seconds">秒</param>
		void setWatchDurationOfLaunch(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds);
		/// <summary>
		/// プロセス起動時に行う状態監視の時間間隔を設定
		/// </summary>
		/// <param name="hours">時</param>
		/// <param name="minutes">分</param>
		/// <param name="seconds">秒</param>
		void setWatchDurationOfLaunch(std::size_t hours, std::size_t minutes, std::size_t seconds);
		/// <summary>
		/// プロセス起動時に行う状態監視の時間制限を設定
		/// </summary>
		/// <param name="hours">時</param>
		/// <param name="minutes">分</param>
		/// <param name="seconds">秒</param>
		void setWaitDurationOfLaunch(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds);
		/// <summary>
		/// プロセス起動時に行う状態監視の時間制限を設定
		/// </summary>
		/// <param name="hours">時</param>
		/// <param name="minutes">分</param>
		/// <param name="seconds">秒</param>
		void setWaitDurationOfLaunch(std::size_t hours, std::size_t minutes, std::size_t seconds);
		/// <summary>
		/// プロセス実行中に行う状態監視の時間間隔を設定
		/// </summary>
		/// <param name="hours">時</param>
		/// <param name="minutes">分</param>
		/// <param name="seconds">秒</param>
		void setWatchDurationOfRunning(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds);
		/// <summary>
		/// プロセス実行中に行う状態監視の時間間隔を設定
		/// </summary>
		/// <param name="hours">時</param>
		/// <param name="minutes">分</param>
		/// <param name="seconds">秒</param>
		void setWatchDurationOfRunning(std::size_t hours, std::size_t minutes, std::size_t seconds);
		/// <summary>
		/// プロセス実行中に行う状態監視の時間制限を設定
		/// </summary>
		/// <param name="hours">時</param>
		/// <param name="minutes">分</param>
		/// <param name="seconds">秒</param>
		void setWaitDurationOfRunning(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds);
		/// <summary>
		/// プロセス実行中に行う状態監視の時間制限を設定
		/// </summary>
		/// <param name="hours">時</param>
		/// <param name="minutes">分</param>
		/// <param name="seconds">秒</param>
		void setWaitDurationOfRunning(std::size_t hours, std::size_t minutes, std::size_t seconds);
		/// <summary>
		/// パラメタ設定値を変化させながらユーザプログラムを起動
		/// </summary>
		/// <param name="parameter">パラメタ設定値</param>
		/// <param name="with_file">パラメタ設定値をファイルで受け渡すかどうか</param>
		/// <returns>時間制限などでプログラムが起動しなかった場合に false</returns>
		bool launchProcess(std::vector<T> parameter, bool with_file = true);
		/// <summary>
		/// 実行中の全プロセスが終了するか，事前に設定された制限時間が経過するまで待機
		/// </summary>
		/// <returns>全プロセス終了で true，制限時間到達で false</returns>
		bool waitAll(void) const;
		/// <summary>
		/// 監視中プロセスの結果を確認
		/// </summary>
		/// <returns>結果の性能値一覧 (終了できていない場合は NaN)</returns>
		std::vector<double> aggregateResults(void);
		/// <summary>
		/// DSICE の一時ファイルとコピーされたディレクトリをすべて削除し，実行前の状態に戻す
		/// </summary>
		void resetEnvironment(void) const;
	};
}

#endif // !DSICE_PROCESS_MANAGER_HPP_
