#include "base_point_log.hpp"

namespace dsice {

	BasePointLog::BasePointLog(const coordinate& base_point) {
		this->base_point = base_point;
	}

	BasePointLog::BasePointLog(const coordinate& base_point, const coordinate_list& first_candidate_list) {
		this->base_point = base_point;
		this->suggest_log_list.push_back(SuggestGroupLog(first_candidate_list));
	}

	BasePointLog::BasePointLog(const BasePointLog& original) {
		this->base_point = original.base_point;
		this->suggest_log_list = original.suggest_log_list;
	}

	BasePointLog::~BasePointLog(void) {
		// DO_NOTHING
	}

	void BasePointLog::appendMeasuredData(const SamplingLog& measured_data) {
		this->suggest_log_list.back().appendMeasuredData(measured_data);
	}

	void BasePointLog::appendMeasuredData(const coordinate& measured_coordinate, double metric_value) {
		this->suggest_log_list.back().appendMeasuredData(measured_coordinate, metric_value);
	}

	void BasePointLog::appendMeasuredData(const coordinate& measured_coordinate, double metric_value, std::chrono::system_clock::time_point measure_start_time, std::chrono::system_clock::time_point measure_end_time) {
		this->suggest_log_list.back().appendMeasuredData(measured_coordinate, metric_value, measure_start_time, measure_end_time);
	}

	void BasePointLog::updateCandidateList(const coordinate_list& candidate_list) {
		this->suggest_log_list.push_back(SuggestGroupLog(candidate_list));
	}

	const coordinate& BasePointLog::getBasePoint(void) const {
		return this->base_point;
	}

	const std::vector<SuggestGroupLog>& BasePointLog::getSuggestLogList(void) const {
		return this->suggest_log_list;
	}
}