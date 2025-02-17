#include <stdexcept>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

#include "dsice_symbols.hpp"
#include "dsice_process_agent.hpp"

namespace dsice {

	void recordStartProcess(std::size_t dsice_process_id) {

		std::ofstream ofs(TMP_PROCESS_STATE_FILE, std::ios_base::app);
		if (!ofs) {
			throw std::runtime_error("Failed to open the DSICE Process State File.\n");
		}

		ofs << dsice_process_id << ", START" << std::endl;
	}

	void recordEndProcess(std::size_t dsice_process_id) {

		std::ofstream ofs(TMP_PROCESS_STATE_FILE, std::ios_base::app);
		if (!ofs) {
			throw std::runtime_error("Failed to open the DSICE Process State File.\n");
		}

		ofs << dsice_process_id << ", END" << std::endl;
	}

	template <>
	int ProcessAgent<int>::stoT(const std::string& str, std::size_t* idx) {
		return std::stoi(str, idx);
	}

	template <>
	long long ProcessAgent<long long>::stoT(const std::string& str, std::size_t* idx) {
		return std::stoll(str, idx);
	}

	template <>
	std::size_t ProcessAgent<std::size_t>::stoT(const std::string& str, std::size_t* idx) {
		return std::stoull(str, idx);
	}

	template <>
	float ProcessAgent<float>::stoT(const std::string& str, std::size_t* idx) {
		return std::stof(str, idx);
	}

	template <>
	double ProcessAgent<double>::stoT(const std::string& str, std::size_t* idx) {
		return std::stod(str, idx);
	}

	template <typename T>
	ProcessAgent<T>::ProcessAgent(void) {

		if (!std::filesystem::is_regular_file(TMP_PARAM_SPECIFY_FILE)) {
			throw std::runtime_error("Parameters specify file is not found.\n");
		}

		std::ifstream ifs(TMP_PARAM_SPECIFY_FILE);
		if (!ifs) {
			throw std::runtime_error("Failed to open the Parameters Specify File.\n");
		}

		ifs >> this->process_id;

		std::size_t param_num;
		ifs >> param_num;

		for (std::size_t i = 0; i < param_num; i++) {
			T tmp;
			ifs >> tmp;
			this->parameter.push_back(tmp);
		}

		recordStartProcess(this->process_id);
	}

	template <typename T>
	ProcessAgent<T>::ProcessAgent(int argc, char** argv) {

		if (argc < 4) {
			throw std::invalid_argument("The number of command-line arguments is too low.\n");
		}

		this->process_id = std::stoull(argv[argc - 1]);
		std::size_t param_num = std::stoull(argv[argc - 2]);

		if (param_num > static_cast<std::size_t>(argc) - 3) {
			throw std::invalid_argument("Parameter information is invalid.\n");
		}

		for (std::size_t i = 0; i < param_num; i++) {
			
			this->parameter.push_back(this->stoT(argv[argc - i - 3]));
		}

		recordStartProcess(this->process_id);
	}

	template <typename T>
	std::size_t ProcessAgent<T>::getProcessId(void) const {
		return this->process_id;
	}

	template <typename T>
	const std::vector<T> ProcessAgent<T>::getParameter(void) const {
		return this->parameter;
	}

	template <typename T>
	void ProcessAgent<T>::recordPerformanceValue(double performance_value) const {

		std::string performance_file_path = TMP_PERFORMANCE_FILE_PREFIX + std::to_string(this->process_id);

		std::ofstream ofs(performance_file_path, std::ios::out);
		if (!ofs) {
			throw std::runtime_error("Failed to open the Performance File.\n");
		}

		ofs << performance_value << std::endl;

		recordEndProcess(this->process_id);
	}

	template class ProcessAgent<int>;
	template class ProcessAgent<long long>;
	template class ProcessAgent<std::size_t>;
	template class ProcessAgent<float>;
	template class ProcessAgent<double>;
}