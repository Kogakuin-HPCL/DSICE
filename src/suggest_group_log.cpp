#include "suggest_group_log.hpp"

namespace dsice {

	SuggestGroupLog::SuggestGroupLog(const coordinate_list& candidate_list) {
		this->candidate_list = candidate_list;
	}

	SuggestGroupLog::SuggestGroupLog(const SuggestGroupLog& original) {
		this->candidate_list = original.candidate_list;
		this->measured_data_list = original.measured_data_list;
	}

	SuggestGroupLog::~SuggestGroupLog(void) {
		// DO_NOTHING
	}

	void SuggestGroupLog::appendMeasuredData(const SamplingLog& measured_data) {
		this->measured_data_list.push_back(measured_data);
	}

	void SuggestGroupLog::appendMeasuredData(const coordinate& measured_coordinate, double metric_value) {
		this->measured_data_list.push_back(SamplingLog(measured_coordinate, metric_value));
	}

	void SuggestGroupLog::appendMeasuredData(const coordinate& measured_coordinate, double metric_value, std::chrono::system_clock::time_point measure_start_time, std::chrono::system_clock::time_point measure_end_time) {
		this->measured_data_list.push_back(SamplingLog(measured_coordinate, metric_value, measure_start_time, measure_end_time));
	}

	const coordinate_list& SuggestGroupLog::getCandidateList(void) const {
		return this->candidate_list;
	}

	const std::vector<SamplingLog>& SuggestGroupLog::getMeasuredDataList(void) const {
		return this->measured_data_list;
	}
}