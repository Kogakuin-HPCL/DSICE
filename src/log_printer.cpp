#include <memory>
#include <string>
#include <ostream>
#include <iostream>
#include <fstream>
#include <limits>

#include "dsice_database_latest_sample_extension.hpp"
#include "dsice_database_logging_extension.hpp"
#include "operation_integrator.hpp"
#include "dsice_operator.hpp"
#include "log_printer.hpp"

namespace dsice {

	// static 変数の実体定義
	template <typename T>
	std::unordered_set<std::string> LogPrinter<T>::appending_log_files;

	template <typename T>
	std::shared_ptr<std::ostream> LogPrinter<T>::makeOutput(const std::string& file_path) {

		if (file_path.empty()) {
			return std::shared_ptr<std::ostream>(&std::cout, [](std::ostream*) {
				// DO_NOTHING
				});
		}
		else {

			// 初めてアクセスするファイルは初期化
			if (!LogPrinter<T>::appending_log_files.contains(file_path)) {

				std::ofstream ofs(file_path, std::ios_base::out);
				if (!ofs) {
					throw std::runtime_error(std::string("Failed to open file : ") + std::string(file_path));
				}
				ofs.close();

				LogPrinter<T>::appending_log_files.insert(file_path);
			}

			std::shared_ptr<std::ostream> output = std::static_pointer_cast<std::ostream>(std::make_shared<std::ofstream>(file_path, std::ios_base::app));

			if (output == nullptr) {
				throw std::runtime_error(std::string("Failed to open file : ") + std::string(file_path));
			}

			return output;
		}
	}

	template <typename T>
	void LogPrinter<T>::scanningAndOutoutSpace(std::shared_ptr<std::ostream> output, std::shared_ptr<DatabaseCore> database, double default_value, std::vector<std::size_t> coordinate_fragment) {

		if (coordinate_fragment.size() == database->getSpaceDimension()) {
			
			for (std::size_t c : coordinate_fragment) {
				*output << c << " ";
			}

			if (database->hasSample(coordinate_fragment)) {
				*output << database->getSampleMetricValue(coordinate_fragment);
			}
			else {
				*output << default_value;
			}

			*output << std::endl;

			return;
		}

		for (std::size_t i = 0; i < database->getSpaceSize()[coordinate_fragment.size()]; i++) {

			std::vector<std::size_t> next_fragment = coordinate_fragment;
			next_fragment.push_back(i);

			LogPrinter<T>::scanningAndOutoutSpace(output, database, default_value, next_fragment);
		}
	}

	template <typename T>
	void LogPrinter<T>::result(std::shared_ptr<Operator> dsice_operator, const std::vector<std::vector<T>>& parameter_space, bool lower_is_better, const std::string& file_path) {

		std::size_t all_parameter_num = 1;
		for (const std::vector<T>& p : parameter_space) {
			all_parameter_num *= p.size();
		}
		std::size_t measured_parameter_num = dsice_operator->getDatabase()->getExistSamplesNum();
		const coordinate& result_coordinate = dsice_operator->getBestMeasuredCoordinate();
		double metric_value;
		if (dsice_operator->getDatabase()->hasSample(result_coordinate)) {
			metric_value = dsice_operator->getDatabase()->getSampleMetricValue(result_coordinate);
		}
		else {
			if (lower_is_better) {
				metric_value = std::numeric_limits<double>::max();
			}
			else {
				metric_value = std::numeric_limits<double>::lowest();
			}
		}

		std::shared_ptr<std::ostream> output = LogPrinter<T>::makeOutput(file_path);

		*output << "[Tuning Result]" << std::endl;
		*output << "\tSearching Status : ";
		if (dsice_operator->isSearchFinished()) {
			*output << "Finish" << std::endl;
		}
		else {
			*output << "Tentative (Not Finish)" << std::endl;
		}
		*output << "\tMeasured Points : " << measured_parameter_num << " / " << all_parameter_num << " (" << static_cast<std::size_t>((static_cast<double>(measured_parameter_num)*100)/static_cast<double>(all_parameter_num)) << "%)" << std::endl;
		*output << "\tParameter :";
		for (std::size_t i = 0; i < result_coordinate.size(); i++) {
			*output << " " << parameter_space[i][result_coordinate[i]];
		}
		*output << " (Coordinate :";
		for (std::size_t c : result_coordinate) {
			*output << " " << c;
		}
		*output << ")" << std::endl;
		*output << "\tPerformance Value : " << metric_value << std::endl;
	}

	template <typename T>
	void LogPrinter<T>::loop_simple(std::shared_ptr<Operator> target_operator, const std::string& file_path, bool for_sequential) {

		std::shared_ptr<std::ostream> output = LogPrinter<T>::makeOutput(file_path);

		if (std::shared_ptr<DatabaseLatestSampleExtension> database = std::dynamic_pointer_cast<DatabaseLatestSampleExtension>(target_operator->getDatabase())) {

			const coordinate_list& measured_coordinates = database->getLatestSampleCoordinate();
			const std::vector<double>& metric_values = database->getLatestMeasuredValue();

			for (std::size_t i = 0; i < measured_coordinates.size(); i++) {

				*output << "Loop = " << (target_operator->getLoopCount() + 1);

				if (!for_sequential) {
					*output << ", Measured = " << (i + 1);
				}

				*output << std::endl;

				*output << "\tSearching Mode : " << target_operator->getSearchModeName() << std::endl;

				*output << "\tBase Point : ";
				for (std::size_t c : target_operator->getBaseCoordinate()) {
					*output << c << " ";
				}
				*output << std::endl;

				*output << "\tMeasured Point : ";
				for (std::size_t c : measured_coordinates[i]) {
					*output << c << " ";
				}
				*output << std::endl;

				*output << "\tPerformance : " << metric_values[i] << std::endl;
			}
		}
		else {
			*output << "database have to be DatabaseLatestSampleExtension" << std::endl;;
		}
	}

	template <typename T>
	void LogPrinter<T>::dsiceLogForViewer(std::shared_ptr<Operator> target_operator, const std::string& file_path) {

		std::shared_ptr<std::ostream> output = LogPrinter<T>::makeOutput(file_path);

		if (std::shared_ptr<DatabaseLoggingExtension> database = std::dynamic_pointer_cast<DatabaseLoggingExtension>(target_operator->getDatabase())) {

			const space_size& space = target_operator->getDatabase()->getSpaceSize();
			*output << space[0];
			for (std::size_t i = 1; i < space.size(); i++) {
				*output << "," << space[i];
			}
			*output << std::endl;

			std::size_t loop_cnt = 1;

			const std::vector<BasePointLog>& log = database->getAllMeasuringLog();
			for (const BasePointLog& each_base : log) {

				const coordinate& base_point = each_base.getBasePoint();
				const std::vector<SuggestGroupLog>& each_base_log = each_base.getSuggestLogList();
				for (const SuggestGroupLog& each_suggest : each_base_log) {

					// 実測点のないループは Viewer が対応しておらずループ数がずれるためスキップ
					if (each_suggest.getMeasuredDataList().empty()) {
						continue;
					}

					*output << "loop " << loop_cnt << " start" << std::endl;

					*output << "base start" << std::endl;
					for (std::size_t c : base_point) {
						*output << c << " ";
					}
					*output << std::endl;
					*output << "base end" << std::endl;

					// TODO: approximate

					*output << "suggested start" << std::endl;
					for (const coordinate& suggested : each_suggest.getCandidateList()) {
						for (std::size_t c : suggested) {
							*output << c << " ";
						}
						*output << std::endl;
					}
					*output << "suggested end" << std::endl;

					*output << "measured start" << std::endl;
					for (const SamplingLog& sample : each_suggest.getMeasuredDataList()) {
						for (std::size_t c : sample.getSampleCoordinate()) {
							*output << c << " ";
						}
						*output << std::endl;
					}
					*output << "measured end" << std::endl;

					*output << "loop " << loop_cnt << " end" << std::endl;

					loop_cnt++;
				}
			}
		}
		else {
			*output << "database have to be DatabaseLoggingExtension" << std::endl;;
		}
	}

	template <typename T>
	void LogPrinter<T>::searchSpaceForViewer(std::shared_ptr<Operator> target_operator, const std::string& file_path, double default_value) {

		std::shared_ptr<std::ostream> output = LogPrinter<T>::makeOutput(file_path);

		std::shared_ptr<DatabaseCore> database = target_operator->getDatabase();

		const space_size& space = database->getSpaceSize();
		*output << space[0];
		for (std::size_t i = 1; i < space.size(); i++) {
			*output << "," << space[i];
		}
		*output << std::endl;

		LogPrinter<T>::scanningAndOutoutSpace(output, database, default_value);
	}

	// コンパイルさせるための実態を記述
	template class LogPrinter<int>;
	template class LogPrinter<long long>;
	template class LogPrinter<std::size_t>;
	template class LogPrinter<float>;
	template class LogPrinter<double>;
}