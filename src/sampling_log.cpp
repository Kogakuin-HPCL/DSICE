#include <chrono>

#include "dsice_search_space.hpp"
#include "sampling_log.hpp"

namespace dsice {

	SamplingLog::SamplingLog(const coordinate& sample_coordinate, double metric_value) {
		this->measure_data = dsice_measure_data(sample_coordinate, metric_value);
		
		std::chrono::system_clock::time_point n = std::chrono::system_clock::now();
		this->measure_start_time = n;
		this->measure_end_time = n;
	}

	SamplingLog::SamplingLog(const coordinate& sample_coordinate, double metric_value, std::chrono::system_clock::time_point measure_start_time, std::chrono::system_clock::time_point measure_end_time) {
		this->measure_data = dsice_measure_data(sample_coordinate, metric_value);
		this->measure_start_time = measure_start_time;
		this->measure_end_time = measure_end_time;
	}

	SamplingLog::SamplingLog(const SamplingLog& original) {
		this->measure_data = original.measure_data;
		this->measure_start_time = original.measure_start_time;
		this->measure_end_time = original.measure_end_time;
	}

	SamplingLog::~SamplingLog(void) {
		// DO_NOTHING
	}

	const coordinate& SamplingLog::getSampleCoordinate(void) const {
		return this->measure_data.first;
	}

	double SamplingLog::getMetricValue(void) const {
		return this->measure_data.second;
	}

	const std::chrono::system_clock::time_point& SamplingLog::getMeasureStartTime(void) const {
		return this->measure_start_time;
	}

	const std::chrono::system_clock::time_point& SamplingLog::getMeasureEndTime(void) const {
		return this->measure_end_time;
	}

	std::chrono::system_clock::duration SamplingLog::getMeasureDuration(void) const {
		return this->measure_end_time - this->measure_start_time;
	}
}