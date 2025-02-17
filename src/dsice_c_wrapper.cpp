#include <stdexcept>
#include <memory>
#include <vector>
#include <unordered_map>

#include "dsice_tuner.hpp"
#include "dsice_c_wrapper.hpp"

namespace dsice {

	std::unordered_map<dsice_tuner_id, std::shared_ptr<Tuner<double>>>& getDsiceTuners(void) {
		
		static std::unordered_map<dsice_tuner_id, std::shared_ptr<Tuner<double>>> tuners;

		return tuners;
	}

	std::unordered_map<dsice_tuner_id, std::vector<const double*>>& getTunerSuggestBuffers(void) {

		static std::unordered_map<dsice_tuner_id, std::vector<const double*>> buffers;

		return buffers;
	}

	std::vector<const double*>& getTargetParamBuffer(dsice_tuner_id tuner_id) {

		static std::unordered_map<dsice_tuner_id, std::vector<const double*>> buffers;

		if (!buffers.contains(tuner_id)) {
			buffers.emplace(tuner_id, std::vector<const double*>());
		}

		return buffers.at(tuner_id);
	}

	std::unordered_map<dsice_tuner_id, std::vector<double>>& getTentativeBestParameterBuffers(void) {

		static std::unordered_map<dsice_tuner_id, std::vector<double>> buffers;

		return buffers;
	}

	std::shared_ptr<Tuner<double>> getTuner(dsice_tuner_id tuner_id) {

		std::unordered_map<dsice_tuner_id, std::shared_ptr<Tuner<double>>>& tuners = getDsiceTuners();

		if (!tuners.contains(tuner_id)) {
			throw std::invalid_argument("Specified tuner is not found.\n");
		}

		return tuners.at(tuner_id);
	}

	std::vector<const double*>& getSuggestBuffer(dsice_tuner_id tuner_id) {

		std::unordered_map<dsice_tuner_id, std::vector<const double*>>& buffers = getTunerSuggestBuffers();

		if (!buffers.contains(tuner_id)) {
			buffers.emplace(tuner_id, std::vector<const double*>());
		}

		return buffers.at(tuner_id);
	}

	std::vector<double>& getBestParamBuffer(dsice_tuner_id tuner_id) {

		std::unordered_map<dsice_tuner_id, std::vector<double>>& buffers = getTentativeBestParameterBuffers();

		if (!buffers.contains(tuner_id)) {
			buffers.emplace(tuner_id, std::vector<double>());
		}

		return buffers.at(tuner_id);
	}

	dsice_tuner_id DSICE_CREATE(void) {

		static dsice_tuner_id tuners_count = 0;
		std::unordered_map<std::size_t, std::shared_ptr<Tuner<double>>>& tuners = getDsiceTuners();

		tuners_count++;
		tuners.emplace(tuners_count, std::make_shared<Tuner<double>>());

		return tuners_count;
	}

	void DSICE_DELETE(dsice_tuner_id tuner_id) {

		std::unordered_map<std::size_t, std::shared_ptr<Tuner<double>>>& tuners = getDsiceTuners();
		std::unordered_map<std::size_t, std::vector<const double*>> buffers = getTunerSuggestBuffers();

		if (tuners.contains(tuner_id)) {
			tuners.erase(tuner_id);
		}

		if (buffers.contains(tuner_id)) {
			buffers.erase(tuner_id);
		}
	}

	void DSICE_APPEND_PARAMETER(dsice_tuner_id tuner_id, size_t parameter_length, double* parameter) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		// Memo: paramter + parameter_length は範囲外アドレスだが，コンストラクタがこのアドレスにアクセスすることがないために安全
		std::vector<double> parameter_vector(parameter, parameter + parameter_length);

		target_tuner->appendParameter(parameter_vector);
	}

	void DSICE_APPEND_PARAMETER_BY_LINEAR_SPACE(dsice_tuner_id tuner_id, double min, double max, double space_length) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->appendParameterByLinearSpace(min, max, space_length);
	}

	void DSICE_SELECT_MODE(dsice_tuner_id tuner_id, DSICE_EXECUTION_MODE mode) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		switch (mode) {
		case DSICE_MODE_RECOMMENDED:
			target_tuner->selectMode(ExecutionMode::RECOMMENDED);
			break;
		case DSICE_MODE_S_IPPE:
			target_tuner->selectMode(ExecutionMode::S_IPPE);
			break;
		case DSICE_MODE_S_2017:
			target_tuner->selectMode(ExecutionMode::S_2017);
			break;
		case DSICE_MODE_S_2018:
			target_tuner->selectMode(ExecutionMode::S_2018);
			break;
		case DSICE_MODE_P_2024B:
			target_tuner->selectMode(ExecutionMode::P_2024B);
		}
	}

	void DSICE_CONFIG_DO_INITIAL_SEARCH(dsice_tuner_id tuner_id, bool do_init_search) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->config_doInitialSearch(do_init_search);
	}

	void DSICE_CONFIG_SPECIFY_INITIAL_PARAMETER(dsice_tuner_id tuner_id, double* initial_parameter) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		std::vector<double> initial_parameter_vec;
		for (std::size_t i = 0; i < target_tuner->getTargetParameters().size(); i++) {
			initial_parameter_vec.push_back(initial_parameter[i]);
		}

		target_tuner->config_specifyInitialParameter(initial_parameter_vec);
	}

	void DSICE_CONFIG_RECORD_LOG(dsice_tuner_id tuner_id, bool record_log) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->config_recordLog(record_log);
	}

	void DSICE_CONFIG_LOWER_VALUE_IS_BETTER(dsice_tuner_id tuner_id, bool lower_value_is_better) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->config_lowerValueIsBetter(lower_value_is_better);
	}

	void DSICE_CONFIG_HIGHER_VALUE_IS_BETTER(dsice_tuner_id tuner_id, bool higher_value_is_better) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->config_higherValueIsBetter(higher_value_is_better);
	}

	void DSICE_CONFIG_SET_PARALLEL_MODE(dsice_tuner_id tuner_id, bool parallel_mode) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->config_setParallelMode(parallel_mode);
	}

	void DSICE_CONFIG_SET_DSPLINE_ALPHA(dsice_tuner_id tuner_id, double alpha) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->config_setDSplineAlpha(alpha);
	}

	void DSICE_CONFIG_SET_DATABASE_METRIC_TYPE(dsice_tuner_id tuner_id, DSICE_DATABASE_METRIC_TYPE metric_type) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		switch (metric_type) {
		case DSICE_METRIC_TYPE_OVERWRITTEN:
			target_tuner->config_setDatabaseMetricType(DatabaseMetricType::OverwrittenDouble);
			break;
		case DSICE_METRIC_TYPE_AVERAGE:
			target_tuner->config_setDatabaseMetricType(DatabaseMetricType::AverageDouble);
			break;
		}
	}

	const double* DSICE_BEGIN(dsice_tuner_id tuner_id) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		return target_tuner->getSuggestedNext().data();
	}

	const double** DSICE_BEGIN_PARALLEL(dsice_tuner_id tuner_id) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);
		std::vector<const double*>& suggested_c_buffer = getSuggestBuffer(tuner_id);

		suggested_c_buffer.clear();

		const std::vector<std::vector<double>>& suggested = target_tuner->getSuggestedList();
		for (std::size_t i = 0; i < suggested.size(); i++) {
			suggested_c_buffer.push_back(suggested[i].data());
		}

		return suggested_c_buffer.data();
	}

	size_t DSICE_GET_SUGGESTED_NUM(dsice_tuner_id tuner_id) {
		return getSuggestBuffer(tuner_id).size();
	}

	void DSICE_END(dsice_tuner_id tuner_id, double performance_value) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->setMetricValue(performance_value);
	}

	size_t DSICE_GET_PARAM_NUM(dsice_tuner_id tuner_id) {
		
		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		return target_tuner->getParametersNum();
	}

	size_t DSICE_GET_PARAM_LENGTH(dsice_tuner_id tuner_id, size_t parameter_idx) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		return target_tuner->getParameterLength(parameter_idx);
	}

	const double** DSICE_GET_TARGET_PARAMETERS(dsice_tuner_id tuner_id) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);
		std::vector<const double*>& parameter_c_buffer = getTargetParamBuffer(tuner_id);
		parameter_c_buffer.clear();

		std::vector<std::vector<double>> target_parameters = target_tuner->getTargetParameters();
		for (const std::vector<double>& param : target_parameters) {
			parameter_c_buffer.push_back(param.data());
		}

		return parameter_c_buffer.data();
	}

	const double* DSICE_GET_TENTATIVE_BEST_PARAMETER(dsice_tuner_id tuner_id) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);
		std::vector<double>& buffers = getBestParamBuffer(tuner_id);

		buffers = target_tuner->getTentativeBestParameter();

		return buffers.data();
	}

	bool DSICE_IS_SEARCH_FINISHED(dsice_tuner_id tuner_id) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		return target_tuner->isSearchFinished();
	}

	void DSICE_END_TIME(dsice_tuner_id tuner_id) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->setTimePerformance();
	}

	void DSICE_END_PARALLEL(dsice_tuner_id tuner_id, size_t value_num, const double* performance_value) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		std::vector<double> tmp_performances;
		for (std::size_t i = 0; i < value_num; i++) {
			tmp_performances.push_back(performance_value[i]);
		}

		target_tuner->setMetricValuesList(tmp_performances);
	}

	void DSICE_PRINT_TUNING_RESULT_STD(dsice_tuner_id tuner_id) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->printTuningResult();
	}

	void DSICE_PRINT_TUNING_RESULT_FILE(dsice_tuner_id tuner_id, const char* file_path) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->printTuningResult(file_path);
	}

	void DSICE_PRINT_SIMPLE_LOOP_LOG_STD(dsice_tuner_id tuner_id) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->printSimpleLoopLog();
	}

	void DSICE_PRINT_SIMPLE_LOOP_LOG_FILE(dsice_tuner_id tuner_id, const char* file_path) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->printSimpleLoopLog(file_path);
	}

	void DSICE_OUTPUT_SEARCH_SPACE_FOR_VIEWER(dsice_tuner_id tuner_id, const char* file_path, double default_value) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->outputSearchSpaceForViewer(file_path, default_value);
	}

	void DSICE_OUTPUT_TUNER_LOG_FOR_VIEWER(dsice_tuner_id tuner_id, const char* file_path) {

		std::shared_ptr<Tuner<double>> target_tuner = getTuner(tuner_id);

		target_tuner->outputLogForViewer(file_path);
	}
}