#include <stdexcept>
#include <memory>
#include <limits>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <iterator>

#include "dsice_search_space.hpp"
#include "dsice_operator_s_ippe.hpp"
#include "dsice_operator_s_2017.hpp"
#include "dsice_operator_s_2018.hpp"
#include "dsice_operator_p_2024b.hpp"
#include "log_printer.hpp"
#include "operation_integrator.hpp"

// MEMO:
//  新しい Operator は buildOperator() とコピーコンストラクタに登録
//  並列用の場合は forSequentialMode() にも登録
//  推奨アルゴリズムの場合は buildOperator() を編集
//  C ラッパーにも入れる場合は dsice_c_wrapper.hpp (および dsice.h) 内の DSICE_EXECUTION_MODE 列挙体と dsice_c_wrapper.cpp 内の DSICE_SELECT_MODE() を編集

namespace dsice {

	template <typename T>
	void OperationIntegrator<T>::buildOperator(void) {

		if (this->target_parameters.empty()) {
			throw std::runtime_error("Tuner needs one parameter at least.");
		}

		// 推奨モードでは設定内容に応じて適切な探索機構を自動選択
		if (this->mode == ExecutionMode::RECOMMENDED) {
			if (this->for_parallel) {
				this->mode = ExecutionMode::P_2024B;
			}
			else if (this->target_parameters.size() == 1) {
				this->mode = ExecutionMode::S_IPPE;
			}
			else {
				this->mode = ExecutionMode::S_2018;
			}
		}

		space_size space;
		for (const std::vector<T>& one_parameter : this->target_parameters) {
			space.push_back(one_parameter.size());
		}

		// 新しい Operator はここに登録
		switch (this->mode) {
		case ExecutionMode::S_IPPE:
			this->search_operator = std::make_shared<Operator_S_IPPE>(space, this->lower_is_better, this->is_logging_mode_on, this->dsp_alpha, this->database_metric_type);
			break;
		case ExecutionMode::S_2017:
			this->search_operator = std::make_shared<Operator_S_2017>(space, this->lower_is_better, this->is_logging_mode_on, this->dsp_alpha, this->database_metric_type);
			break;
		case ExecutionMode::S_2018:
			this->search_operator = std::make_shared<Operator_S_2018>(space, this->lower_is_better, this->is_logging_mode_on, this->operator_initialize_way, this->dsp_alpha, this->database_metric_type, this->initial_coordinate);
			break;
		case ExecutionMode::P_2024B:
			this->search_operator = std::make_shared<Operator_P_2024B>(space, this->lower_is_better, this->is_logging_mode_on, this->operator_initialize_way, this->dsp_alpha, this->database_metric_type, this->initial_coordinate);
			break;
		default:
			throw std::runtime_error("Failed to build the Search Operator : the specified mode is not found.\n");
		}
	}

	template <typename T>
	constexpr bool OperationIntegrator<T>::forSequentialMode(void) const {

		// 並列アルゴリズムはここにも登録
		switch (this->mode) {
		case ExecutionMode::P_2024B:
			return false;
		default:
			return true;
		}
	}

	template <typename T>
	OperationIntegrator<T>::OperationIntegrator(void) {
		// DO_NOTHING
	}

	template <typename T>
	OperationIntegrator<T>::OperationIntegrator(const std::vector<std::vector<T>>& parameters) {

		for (const std::vector<T>& p : parameters) {
			if (p.empty()) {
				throw std::invalid_argument("each parameter must have one value at least.");
			}
		}

		this->target_parameters = parameters;
	}

	template <typename T>
	OperationIntegrator<T>::OperationIntegrator(const OperationIntegrator& original) {

		this->target_parameters = original.target_parameters;
		this->suggest_parameters = original.suggest_parameters;
		this->mode = original.mode;
		this->timer_start_point = original.timer_start_point;
		this->lower_is_better = original.lower_is_better;
		this->is_logging_mode_on = original.is_logging_mode_on;
		this->operator_initialize_way = original.operator_initialize_way;
		this->for_parallel = original.for_parallel;
		this->dsp_alpha = original.dsp_alpha;
		this->database_metric_type = original.database_metric_type;

		if (original.search_operator == nullptr) {
			this->search_operator = nullptr;
		}
		else {
			// 新しい Operator はここにも登録
			switch (this->mode) {
			case ExecutionMode::S_IPPE:
				if (std::shared_ptr<Operator_S_IPPE> s = std::dynamic_pointer_cast<Operator_S_IPPE>(original.search_operator)) {
					this->search_operator = std::make_shared<Operator_S_IPPE>(*s);
				}
				break;
			case ExecutionMode::S_2017:
				if (std::shared_ptr<Operator_S_2017> s = std::dynamic_pointer_cast<Operator_S_2017>(original.search_operator)) {
					this->search_operator = std::make_shared<Operator_S_2017>(*s);
				}
				break;
			case ExecutionMode::S_2018:
				if (std::shared_ptr<Operator_S_2018> s = std::dynamic_pointer_cast<Operator_S_2018>(original.search_operator)) {
					this->search_operator = std::make_shared<Operator_S_2018>(*s);
				}
				break;
			case ExecutionMode::P_2024B:
				if (std::shared_ptr<Operator_P_2024B> s = std::dynamic_pointer_cast<Operator_P_2024B>(original.search_operator)) {
					this->search_operator = std::make_shared<Operator_P_2024B>(*s);
				}
				break;
			default:
				throw std::runtime_error("Failed to copy Tuner instance : the specified mode is not found.\n");
			}
		}
	}

	template <typename T>
	OperationIntegrator<T>::~OperationIntegrator(void) {
		// DO_NOTHING
	}

	template <typename T>
	void OperationIntegrator<T>::appendParameter(const std::vector<T>& parameter) {

		if (parameter.empty()) {
			throw std::invalid_argument("The parameter must have one value at least.");
		}

		this->target_parameters.push_back(parameter);
	}

	template <typename T>
	void OperationIntegrator<T>::appendParameterByLinearSpace(const T& min, const T& max, const T& space_length) {

		if (max < min) {
			throw std::invalid_argument("The minimum value must be less than the maximum value.");
		}

		if (space_length < 0) {
			throw std::invalid_argument("It does not allow space_value to be less than zero.");
		}

		this->target_parameters.emplace_back();

		// space_length が 0 なら最小値のみ追加
		if (space_length == 0) {
			this->target_parameters.back().push_back(min);
			return;
		}

		std::size_t idx = 0;
		while (static_cast<T>(min + space_length * idx) <= max) {
			this->target_parameters.back().push_back(static_cast<T>(min + space_length * idx));
			idx++;
		}
	}

	template <typename T>
	void OperationIntegrator<T>::selectMode(ExecutionMode execution_mode) {

		// すでに探索が開始している場合は何もしない
		if (this->search_operator != nullptr) {
			return;
		}

		this->mode = execution_mode;
	}

	template <typename T>
	void OperationIntegrator<T>::config_doInitialSearch(bool do_init_search) {

		// すでに探索が開始している場合は何もしない
		if (this->search_operator != nullptr) {
			return;
		}

		if (do_init_search) {
			this->operator_initialize_way = OperatorInitializeWay::INITIAL_SEARCH;
		}
		else {
			if (this->initial_coordinate.empty()) {
				this->operator_initialize_way = OperatorInitializeWay::CENTER;
			}
			else {
				this->operator_initialize_way = OperatorInitializeWay::SPECIFIED;
			}
		}
	}

	template <typename T>
	void OperationIntegrator<T>::config_specifyInitialParameter(const std::vector<T>& initial_parameter) {

		// すでに探索が開始している場合は何もしない
		if (this->search_operator != nullptr) {
			return;
		}

		if (initial_parameter.size() != this->target_parameters.size()) {
			throw std::invalid_argument("The dimension of the specified initial parameter is not the same as the target parameter's.");
		}

		this->operator_initialize_way = OperatorInitializeWay::SPECIFIED;
		this->initial_coordinate = {};

		for (std::size_t i = 0; i < this->target_parameters.size(); i++) {

			auto it = std::find(this->target_parameters[i].begin(), this->target_parameters[i].end(), initial_parameter[i]);
			if (it == this->target_parameters[i].end()) {
				throw std::invalid_argument("One of the elements in the specified initial parameter is not found in the parameter.");
			}

			this->initial_coordinate.push_back(std::distance(this->target_parameters[i].begin(), it));
		}
	}

	template <typename T>
	void OperationIntegrator<T>::config_recordLog(bool record_log) {

		// すでに探索が開始している場合は何もしない
		if (this->search_operator != nullptr) {
			return;
		}

		this->is_logging_mode_on = record_log;
	}

	template <typename T>
	void OperationIntegrator<T>::config_lowerValueIsBetter(bool lower_value_is_better) {

		// すでに探索が開始している場合は何もしない
		if (this->search_operator != nullptr) {
			return;
		}

		this->lower_is_better = lower_value_is_better;
	}

	template <typename T>
	void OperationIntegrator<T>::config_higherValueIsBetter(bool higher_value_is_better) {

		// すでに探索が開始している場合は何もしない
		if (this->search_operator != nullptr) {
			return;
		}

		this->lower_is_better = !higher_value_is_better;
	}

	template <typename T>
	void OperationIntegrator<T>::config_setParallelMode(bool parallel_mode) {
		this->for_parallel = parallel_mode;
	}

	template <typename T>
	void OperationIntegrator<T>::config_setDSplineAlpha(double alpha) {
		this->dsp_alpha = alpha;
	}

	template <typename T>
	void OperationIntegrator<T>::config_setDatabaseMetricType(DatabaseMetricType metric_type) {
		this->database_metric_type = metric_type;
	}

	template <typename T>
	const std::vector<T>& OperationIntegrator<T>::getSuggestedNext(void) {

		if (this->search_operator == nullptr) {
			this->buildOperator();
		}

		this->suggest_parameters.clear();

		const coordinate& next_coordinate = this->search_operator->getSuggested();
		this->suggest_parameters.emplace_back();
		for (std::size_t i = 0; i < next_coordinate.size(); i++) {
			this->suggest_parameters.back().push_back(this->target_parameters[i][next_coordinate[i]]);
		}

		this->timer_start_point = std::chrono::system_clock::now();

		return this->suggest_parameters.back();
	}

	template <typename T>
	const std::vector<std::vector<T>>& OperationIntegrator<T>::getSuggestedList(void) {

		if (this->search_operator == nullptr) {
			this->buildOperator();
		}

		this->suggest_parameters.clear();

		const coordinate_list& suggested_coordinates = this->search_operator->getSuggestedList();
		for (const coordinate& one_coordinate : suggested_coordinates) {
			this->suggest_parameters.emplace_back();
			for (std::size_t i = 0; i < one_coordinate.size(); i++) {
				this->suggest_parameters.back().push_back(this->target_parameters[i][one_coordinate[i]]);
			}
		}

		return this->suggest_parameters;
	}

	template <typename T>
	void OperationIntegrator<T>::setMetricValue(double metric_value) {
		this->search_operator->setMetricValue(this->search_operator->getSuggested(), metric_value);
	}

	template <typename T>
	std::size_t OperationIntegrator<T>::getParametersNum(void) const {
		return this->target_parameters.size();
	}

	template <typename T>
	std::size_t OperationIntegrator<T>::getParameterLength(std::size_t parameter_idx) const {
		return this->target_parameters[parameter_idx].size();
	}

	template <typename T>
	const std::vector<std::vector<T>>& OperationIntegrator<T>::getTargetParameters(void) const {
		return this->target_parameters;
	}

	template <typename T>
	std::vector<T> OperationIntegrator<T>::getTentativeBestParameter(void) const {

		const coordinate& base_coordinate = this->search_operator->getBaseCoordinate();

		std::vector<T> tmp;
		for (std::size_t i = 0; i < base_coordinate.size(); i++) {
			tmp.push_back(this->target_parameters[i][base_coordinate[i]]);
		}

		return tmp;
	}

	template <typename T>
	bool OperationIntegrator<T>::isSearchFinished(void) const {
		return this->search_operator->isSearchFinished();
	}

	template <typename T>
	void OperationIntegrator<T>::setTimePerformance(void) {

		// TODO: どれくらいの時間でループする？
		std::chrono::nanoseconds elapsed_time = std::chrono::system_clock::now() - this->timer_start_point;
		double nano_sec_double = static_cast<double>(elapsed_time.count());

		this->search_operator->setMetricValue(this->search_operator->getSuggested(), nano_sec_double);
	}

	template <typename T>
	void OperationIntegrator<T>::setMetricValuesList(const std::vector<double>& metric_values_list) {

		const coordinate_list& suggested_list = this->search_operator->getSuggestedList();

		if (suggested_list.size() < metric_values_list.size()) {
			throw std::invalid_argument("There are too many metric values.");
		}

		for (std::size_t i = 0; i < metric_values_list.size(); i++) {
			this->search_operator->setMetricValue(suggested_list[i], metric_values_list[i]);
		}
	}

	template <typename T>
	void OperationIntegrator<T>::printTuningResult(std::string file_path) const {

		coordinate tmp_param_coordinate = this->search_operator->getBestMeasuredCoordinate();
		
		LogPrinter<T>::result(this->search_operator, this->target_parameters, this->lower_is_better, file_path);
	}

	template <typename T>
	void OperationIntegrator<T>::printSimpleLoopLog(std::string file_path) const {
		LogPrinter<T>::loop_simple(this->search_operator, file_path, this->forSequentialMode());
	}

	template <typename T>
	void OperationIntegrator<T>::outputSearchSpaceForViewer(std::string file_path, double default_value) const {
		LogPrinter<T>::searchSpaceForViewer(this->search_operator, file_path, default_value);
	}

	template <typename T>
	void OperationIntegrator<T>::outputLogForViewer(std::string file_path) const {
		LogPrinter<T>::dsiceLogForViewer(this->search_operator, file_path);
	}

	// コンパイルさせるための実態を記述
	template class OperationIntegrator<int>;
	template class OperationIntegrator<long long>;
	template class OperationIntegrator<std::size_t>;
	template class OperationIntegrator<float>;
	template class OperationIntegrator<double>;
}