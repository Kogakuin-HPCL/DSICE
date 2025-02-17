#include <stdexcept>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <limits>
#include <chrono>
#include <thread>
#include <iostream>

#include "dsice_symbols.hpp"
#include "dsice_process_launcher.hpp"
#include "dsice_supercomputers.hpp"
#include "dsice_process_manager.hpp"

namespace dsice {

	template <typename T>
	std::size_t ProcessManager<T>::getPID(std::filesystem::path pid_recorded_path) {

		std::ifstream ifs(pid_recorded_path);
		if (!ifs) {
			throw std::runtime_error("Failed to open the PID File.\n");
		}

		std::size_t tmp;
		ifs >> tmp;

		return tmp;
	}

	template <typename T>
	ProcessManager<T>::ProcessManager(void) {
		this->current_path = std::filesystem::current_path();
	}

	template <typename T>
	void ProcessManager<T>::setJobScript(SuperComputer machine, const std::string& script_path) {

		if (machine == SuperComputer::PLAIN) {
			throw std::invalid_argument("Specified machine is not supercomputer.\n");
		}

		this->execute_machine = machine;
		this->kill_process_command = "pjdel";
		this->command_in_bin = false;

		try {
			this->execution_command = std::filesystem::canonical(script_path);
		}
		catch (std::filesystem::filesystem_error& e) {
			std::cerr << e.what() << std::endl;
			throw std::runtime_error("Failed to set a job script : script file does not exist.\n");
		}
	}

	template <typename T>
	void ProcessManager<T>::setLaunchCommand(const std::string& command, std::string options) {

		this->execute_machine = SuperComputer::PLAIN;
		this->kill_process_command = "dsice_del";

		// パス形式かどうかをチェック
		std::filesystem::path tmp = command;
		tmp.remove_filename();
		if (tmp.empty()) {
			this->command_in_bin = true;
			this->execution_command = command;
		}
		else {
			this->command_in_bin = false;
			this->execution_command = std::filesystem::absolute(command);
		}

		this->command_options = options;
	}

	template <typename T>
	void ProcessManager<T>::setLaunchCommand(const std::string& command, const std::vector<std::string>& options) {

		this->execute_machine = SuperComputer::PLAIN;
		this->kill_process_command = "dsice_del";

		// パス形式かどうかをチェック
		std::filesystem::path tmp = command;
		tmp.remove_filename();
		if (tmp.empty()) {
			this->command_in_bin = true;
			this->execution_command = command;
		}
		else {
			this->command_in_bin = false;
			this->execution_command = std::filesystem::absolute(command);
		}

		if (options.empty()) {
			return;
		}

		this->command_options = options[0];
		for (std::size_t i = 1; i < options.size(); i++) {
			this->command_options += " " + options[i];
		}
	}

	template <typename T>
	void ProcessManager<T>::setCopiedDirectory(std::string directory_path) {

		this->do_copy = true;

		std::filesystem::path tmp_dir;
		try {
			tmp_dir = std::filesystem::canonical(directory_path);
		}
		catch (std::filesystem::filesystem_error& e) {
			std::cerr << e.what() << std::endl;
			throw std::runtime_error("Failed to copy directory : directory_path does not exist.\n");
		}

		this->work_dir_prefix.clear();
		this->executed_file_path_suffix.clear();

		for (auto p = tmp_dir.begin(); p != tmp_dir.end(); p++) {

			// 仮で進めて最終要素か確認
			p++;
			if (p == tmp_dir.end()) {
				p--;
				this->original_work_dir_name = *p;
			}
			else {
				p--;
				this->work_dir_prefix /= *p;
			}
		}

		// PATH 経由のコマンド実行の場合はここで終了
		if (this->command_in_bin) {
			return;
		}

		// 実行ファイルがワーキングディレクトリにあるかどうかをチェック
		this->is_command_in_copied_dir = true;
		auto comm_itr = this->execution_command.begin();
		auto dir_itr = tmp_dir.begin();
		while (dir_itr != tmp_dir.end()) {

			if (*comm_itr != *dir_itr) {
				this->is_command_in_copied_dir = false;
				break;
			}

			comm_itr++;
			dir_itr++;
		}

		// コピー後の実行ファイル位置を示す相対パスを取得
		while (comm_itr != this->execution_command.end()) {

			comm_itr++;
			if (comm_itr == this->execution_command.end()) {
				comm_itr--;
				this->executed_file_name = *comm_itr;
			}
			else {
				comm_itr--;
				this->executed_file_path_suffix /= *comm_itr;
			}

			comm_itr++;
		}
	}

	template <typename T>
	void ProcessManager<T>::config_switchWaitLimit(bool limit_on) {
		this->process_kill_reservation_on = limit_on;
	}

	template <typename T>
	void ProcessManager<T>::setWatchDurationOfLaunch(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds) {
		this->watch_duration_of_launch_h = hours;
		this->watch_duration_of_launch_m = minutes;
		this->watch_duration_of_launch_s = seconds;
	}

	template <typename T>
	void ProcessManager<T>::setWatchDurationOfLaunch(std::size_t hours, std::size_t minutes, std::size_t seconds) {
		this->watch_duration_of_launch_h = std::chrono::hours(hours);
		this->watch_duration_of_launch_m = std::chrono::minutes(minutes);
		this->watch_duration_of_launch_s = std::chrono::seconds(seconds);
	}

	template <typename T>
	void ProcessManager<T>::setWaitDurationOfLaunch(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds) {
		this->wait_duration_of_launch_h = hours;
		this->wait_duration_of_launch_m = minutes;
		this->wait_duration_of_launch_s = seconds;
	}

	template <typename T>
	void ProcessManager<T>::setWaitDurationOfLaunch(std::size_t hours, std::size_t minutes, std::size_t seconds) {
		this->wait_duration_of_launch_h = std::chrono::hours(hours);
		this->wait_duration_of_launch_m = std::chrono::minutes(minutes);
		this->wait_duration_of_launch_s = std::chrono::seconds(seconds);
	}

	template <typename T>
	void ProcessManager<T>::setWatchDurationOfRunning(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds) {
		this->watch_duration_of_running_h = hours;
		this->watch_duration_of_running_m = minutes;
		this->watch_duration_of_running_s = seconds;
	}

	template <typename T>
	void ProcessManager<T>::setWatchDurationOfRunning(std::size_t hours, std::size_t minutes, std::size_t seconds) {
		this->watch_duration_of_running_h = std::chrono::hours(hours);
		this->watch_duration_of_running_m = std::chrono::minutes(minutes);
		this->watch_duration_of_running_s = std::chrono::seconds(seconds);
	}

	template <typename T>
	void ProcessManager<T>::setWaitDurationOfRunning(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds) {
		this->wait_duration_of_running_h = hours;
		this->wait_duration_of_running_m = minutes;
		this->wait_duration_of_running_s = seconds;
	}

	template <typename T>
	void ProcessManager<T>::setWaitDurationOfRunning(std::size_t hours, std::size_t minutes, std::size_t seconds) {
		this->wait_duration_of_running_h = std::chrono::hours(hours);
		this->wait_duration_of_running_m = std::chrono::minutes(minutes);
		this->wait_duration_of_running_s = std::chrono::seconds(seconds);
	}

	template <typename T>
	bool ProcessManager<T>::launchProcess(std::vector<T> parameter, bool with_file) {

		if (this->reserved_update) {
			this->running_processes.clear();
			this->running_pid.clear();
			this->running_dir.clear();
			this->reserved_update = false;
		}

		this->dsice_process_id++;

		std::filesystem::path process_state_file_dir;

		if (this->do_copy) {

			std::filesystem::path original = this->work_dir_prefix / this->original_work_dir_name;
			std::filesystem::path new_dir = this->work_dir_prefix / std::filesystem::path(std::string(".dsice_process_") + std::to_string(this->dsice_process_id));

			std::filesystem::copy(original, new_dir, std::filesystem::copy_options::recursive);

			// プロセス起動
			if (this->command_in_bin) {

				std::string comm;
				switch (this->execute_machine) {
				case SuperComputer::PLAIN:
					comm = std::string("dsice_launch ") + this->execution_command.string() + std::string(" ") + this->command_options;
					break;
				default:
					comm = std::string("dsice_submit_job --target ") + std::to_string(static_cast<std::size_t>(this->execute_machine)) + std::string(" --script-file ") + this->execution_command.string();
					break;
				}

				if (with_file) {
					dsice::launchProcessWithFile(comm, parameter, this->dsice_process_id);
				}
				else {
					dsice::launchProcess(comm, parameter, this->dsice_process_id);
				}

				process_state_file_dir = std::filesystem::current_path();
			}
			else {

				std::filesystem::path run_dir_path = new_dir / this->executed_file_path_suffix;
				std::filesystem::current_path(run_dir_path);

				std::string comm;
				switch (this->execute_machine) {
				case SuperComputer::PLAIN:
					comm = std::string("dsice_launch ") + ("." / this->executed_file_name).string() + std::string(" ") + this->command_options;
					break;
				default:
					comm = std::string("dsice_submit_job --target ") + std::to_string(static_cast<std::size_t>(this->execute_machine)) + std::string(" --script-file ") + this->execution_command.string();
					break;
				}

				if (with_file) {
					dsice::launchProcessWithFile(comm, parameter, this->dsice_process_id);
				}
				else {
					dsice::launchProcess(comm, parameter, this->dsice_process_id);
				}

				std::filesystem::current_path(this->current_path);

				process_state_file_dir = run_dir_path;
			}
		}
		else {

			// プロセス起動
			std::string comm;
			switch (this->execute_machine) {
			case SuperComputer::PLAIN:
				if (this->command_in_bin) {
					comm = std::string("dsice_launch ") + this->execution_command.string() + " " + this->command_options;
				}
				else {
					comm = std::string("dsice_launch ") + std::filesystem::canonical(this->execution_command).string() + " " + this->command_options;
				}
				break;
			default:
				comm = std::string("dsice_submit_job --target ") + std::to_string(static_cast<std::size_t>(this->execute_machine)) + std::string(" --script-file ") + this->execution_command.string();
				break;
			}

			if (with_file) {
				dsice::launchProcessWithFile(comm, parameter, this->dsice_process_id);
			}
			else {
				dsice::launchProcess(comm, parameter, this->dsice_process_id);
			}

			process_state_file_dir = std::filesystem::current_path();
		}

		std::size_t pid = this->getPID(process_state_file_dir / TMP_PID_FILE);
		this->running_pid.emplace(this->dsice_process_id, pid);

		std::chrono::system_clock::time_point wait_limit = std::chrono::system_clock::now() + this->wait_duration_of_launch_h + this->wait_duration_of_launch_m + this->wait_duration_of_launch_s;

		// プロセス開始待機
		std::string start_notice_str = std::to_string(this->dsice_process_id) + ", START";
		while (true) {

			if (std::filesystem::is_regular_file(process_state_file_dir / TMP_PROCESS_STATE_FILE)) {

				std::ifstream ifs(process_state_file_dir / TMP_PROCESS_STATE_FILE);
				std::string file_str = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

				if (file_str.find(start_notice_str) != std::string::npos) {
					this->running_processes.push_back(this->dsice_process_id);
					this->running_dir.push_back(process_state_file_dir);
					break;
				}
			}

			// 待機時間を超えたらプロセス起動をキャンセル
			// TODO: ジョブキャンセルはできない？
			if (std::chrono::system_clock::now() > wait_limit) {

				// プロセスキル
				if (this->process_kill_reservation_on && this->running_pid.contains(this->dsice_process_id)) {
					
					int kill_result = std::system((this->kill_process_command + " " + std::to_string(this->running_pid.at(this->dsice_process_id))).c_str());

					if (kill_result != 0) {
						std::cerr << "Failed to cancel process execution.\n" << std::endl;
					}
				}

				return false;
			}
			
			std::chrono::system_clock::time_point wait_next = std::chrono::system_clock::now() + this->watch_duration_of_launch_h + this->watch_duration_of_launch_m + this->watch_duration_of_launch_s;
			std::this_thread::sleep_until(wait_next);
		}

		return true;
	}

	template <typename T>
	bool ProcessManager<T>::waitAll(void) const {

		for (std::size_t i = 0; i < this->running_processes.size(); i++) {

			std::size_t id = this->running_processes[i];
			std::filesystem::path state_file = this->running_dir[i] / TMP_PROCESS_STATE_FILE;

			std::string end_notice_str = std::to_string(id) + ", END";
			std::chrono::system_clock::time_point wait_limit = std::chrono::system_clock::now() + this->wait_duration_of_running_h + this->wait_duration_of_running_m + this->wait_duration_of_running_s;

			while (true) {

				if (std::filesystem::is_regular_file(state_file)) {

					std::ifstream ifs(state_file);
					std::string file_str = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

					if (file_str.find(end_notice_str) != std::string::npos) {
						break;
					}
				}

				// 待機制限時間で終了
				if (std::chrono::system_clock::now() > wait_limit) {
					return false;
				}

				std::chrono::system_clock::time_point wait_next = std::chrono::system_clock::now() + this->watch_duration_of_running_h + this->watch_duration_of_running_m + this->watch_duration_of_running_s;
				std::this_thread::sleep_until(wait_next);
			}
		}

		return true;
	}

	template <typename T>
	std::vector<double> ProcessManager<T>::aggregateResults(void) {

		std::vector<double> performance_values;

		for (std::size_t i = 0; i < this->running_processes.size(); i++) {

			std::filesystem::path performance_file_path = this->running_dir[i] / std::filesystem::path(TMP_PERFORMANCE_FILE_PREFIX + std::to_string(this->running_processes[i]));

			std::ifstream ifs(performance_file_path);
			if (ifs) {
				double tmp;
				ifs >> tmp;
				performance_values.push_back(tmp);
			}
			else {
				
				performance_values.push_back(std::numeric_limits<double>::quiet_NaN());

				// プロセスキル
				if (this->process_kill_reservation_on && this->running_pid.contains(this->running_processes[i])) {
					
					int kill_result = std::system((this->kill_process_command + " " + std::to_string(this->running_pid.at(this->running_processes[i]))).c_str());

					if (kill_result != 0) {
						std::cerr << "Failed to cancel process execution.\n" << std::endl;
					}
				}
			}
		}

		this->reserved_update = true;

		return performance_values;
	}

	template <typename T>
	void ProcessManager<T>::resetEnvironment(void) const {

		for (std::size_t i = 0; i < this->running_processes.size(); i++) {

			std::filesystem::path parameter_file_path = this->running_dir[i] / TMP_PARAM_SPECIFY_FILE;
			std::filesystem::path state_file_path = this->running_dir[i] / TMP_PROCESS_STATE_FILE;
			std::filesystem::path performance_file_path = this->running_dir[i] / std::filesystem::path(TMP_PERFORMANCE_FILE_PREFIX + std::to_string(this->running_processes[i]));

			if (std::filesystem::is_regular_file(parameter_file_path)) {
				std::filesystem::remove(parameter_file_path);
			}

			if (std::filesystem::is_regular_file(state_file_path)) {
				std::filesystem::remove(state_file_path);
			}

			if (std::filesystem::is_regular_file(performance_file_path)) {
				std::filesystem::remove(performance_file_path);
			}
		}
	}

	// コンパイルさせるための実態を記述
	template class ProcessManager<int>;
	template class ProcessManager<long long>;
	template class ProcessManager<std::size_t>;
	template class ProcessManager<float>;
	template class ProcessManager<double>;
}