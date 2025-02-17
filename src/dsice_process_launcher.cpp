#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <future>

#include "dsice_symbols.hpp"
#include "dsice_process_launcher.hpp"

namespace dsice {

	template <typename T>
	void launchProcess(std::string command, const std::vector<T>& parameters, std::size_t process_id) {

		std::string run_command = command;
		for (T p : parameters) {
			run_command += " " + std::to_string(p);
		}
		if (parameters.size() != 0) {
			run_command += " " + std::to_string(parameters.size());
			run_command += " " + std::to_string(process_id);
		}

		int execution_result = std::system(run_command.c_str());
		if (execution_result != 0) {
			throw std::runtime_error("Failed to launch process.\n");
		}
	}

	template <typename T>
	void launchProcessWithFile(std::string command, const std::vector<T>& parameters, std::size_t process_id) {

		std::ofstream ofs(TMP_PARAM_SPECIFY_FILE, std::ios::out);
		if (!ofs) {
			throw std::runtime_error("Failed to open the Parameters Specify File.\n");
		}

		ofs << process_id;
		ofs << " " << parameters.size();
		for (T p : parameters) {
			ofs << " " << p;
		}

		ofs.close();

		int execution_result = std::system(command.c_str());
		if (execution_result != 0) {
			throw std::runtime_error("Failed to launch process.\n");
		}
	}

	//
	// テンプレート関数の実態を定義
	//

	template void launchProcess(std::string, const std::vector<int>&, std::size_t);
	template void launchProcess(std::string, const std::vector<long long>&, std::size_t);
	template void launchProcess(std::string, const std::vector<std::size_t>&, std::size_t);
	template void launchProcess(std::string, const std::vector<float>&, std::size_t);
	template void launchProcess(std::string, const std::vector<double>&, std::size_t);

	template void launchProcessWithFile(std::string, const std::vector<int>&, std::size_t);
	template void launchProcessWithFile(std::string, const std::vector<long long>&, std::size_t);
	template void launchProcessWithFile(std::string, const std::vector<std::size_t>&, std::size_t);
	template void launchProcessWithFile(std::string, const std::vector<float>&, std::size_t);
	template void launchProcessWithFile(std::string, const std::vector<double>&, std::size_t);
}
