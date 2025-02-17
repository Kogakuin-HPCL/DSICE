#include <memory>
#include <limits>

#include "dsice_metric.hpp"
#include "overwritten_double.hpp"
#include "average_double.hpp"
#include "dsice_search_space.hpp"
#include "dsice_logging_database.hpp"

namespace dsice {

	LoggingDatabase::LoggingDatabase(const space_size& search_space, DatabaseMetricType database_metric_type) {
		this->search_space_size = search_space;
		this->metric_type = database_metric_type;
		this->log.emplace_back(coordinate());
	}

	LoggingDatabase::LoggingDatabase(const LoggingDatabase& original) {

		this->search_space_size = original.search_space_size;

		for (const std::pair<const coordinate, std::shared_ptr<Metric>>& original_sample : original.samples) {
			this->samples.emplace(original_sample.first, original_sample.second->makeCopy());
		}

		this->base_points = original.base_points;
		this->metric_type = original.metric_type;
		this->log = original.log;
	}

	LoggingDatabase::~LoggingDatabase(void) {
		// DO_NOTHING
	}

	dimension_size LoggingDatabase::getSpaceDimension(void) const {
		return this->search_space_size.size();
	}

	const space_size& LoggingDatabase::getSpaceSize(void) const {
		return this->search_space_size;
	}

	void LoggingDatabase::setSampleMetricValue(const coordinate& target_coordinate, double metric_value) {

		if (this->log.back().getSuggestLogList().size() == 0) {
			this->log.back().updateCandidateList(coordinate_list());
		}
		this->log.back().appendMeasuredData(target_coordinate, metric_value);

		this->latest_sample_coordinates.push_back(target_coordinate);
		this->latest_measured_values.push_back(metric_value);

		if (this->samples.contains(target_coordinate)) {
			this->samples.at(target_coordinate)->setValue(metric_value);
		}
		else {
			switch (this->metric_type) {
			case DatabaseMetricType::OverwrittenDouble:
				this->samples.emplace(target_coordinate, std::make_shared<OverwrittenDouble>(metric_value));
				break;
			case DatabaseMetricType::AverageDouble:
				this->samples.emplace(target_coordinate, std::make_shared<AverageDouble>(metric_value));
				break;
			}
		}
	}

	bool LoggingDatabase::hasSample(const coordinate& target_coordinate) const {
		return this->samples.contains(target_coordinate);
	}

	std::size_t LoggingDatabase::getExistSamplesNum(void) const {
		return this->samples.size();
	}

	double LoggingDatabase::getSampleMetricValue(const coordinate& target_coordinate) const {

		if (this->samples.contains(target_coordinate)) {
			return this->samples.at(target_coordinate)->getValue();
		}
		else {
			return std::numeric_limits<double>::quiet_NaN();
		}
	}

	void LoggingDatabase::setBasePoint(const coordinate& target_coordinate) {

		if (this->log.back().getBasePoint() == target_coordinate) {
			this->has_base_coordinate_changed = false;
			return;
		}

		this->log.emplace_back(target_coordinate);
		this->has_base_coordinate_changed = true;
		
		if (!this->base_points.contains(target_coordinate)) {
			this->base_points.emplace(target_coordinate, std::unordered_set<DirectionLine>());
		}
	}

	void LoggingDatabase::updateCandidateList(const coordinate_list candidates_list) {
		this->log.back().updateCandidateList(candidates_list);
	}

	const std::unordered_map<coordinate, std::unordered_set<DirectionLine>>& LoggingDatabase::getBasePoints(void) const {
		return this->base_points;
	}

	bool LoggingDatabase::hasBeenBasePoint(const coordinate& target_coordinate) const {
		return this->base_points.contains(target_coordinate);
	}

	void LoggingDatabase::recordSearchedDirection(const coordinate& base_coordinate, const DirectionLine& line_info) {

		if (!this->base_points.contains(base_coordinate)) {
			this->base_points.emplace(base_coordinate, std::unordered_set<DirectionLine>());
		}

		this->base_points.at(base_coordinate).insert(line_info);
	}

	bool LoggingDatabase::isSearchedDirection(const coordinate& base_coordinate, const DirectionLine& line_info) const {

		if (this->base_points.contains(base_coordinate)) {
			if (this->base_points.at(base_coordinate).contains(line_info)) {
				return true;
			}
		}

		return false;
	}

	void LoggingDatabase::setLoopEnd(void) {
		this->latest_sample_coordinates.clear();
		this->latest_measured_values.clear();
	}

	const coordinate_list& LoggingDatabase::getLatestSampleCoordinate(void) const {
		return this->latest_sample_coordinates;
	}

	const std::vector<double>& LoggingDatabase::getLatestMeasuredValue(void) const {
		return this->latest_measured_values;
	}

	const coordinate& LoggingDatabase::getLatestBaseCoordinate(void) const {
		return this->log.back().getBasePoint();
	}

	bool LoggingDatabase::hasBaseCoordinateChanged(void) const {
		return this->has_base_coordinate_changed;
	}

	coordinate_list LoggingDatabase::getBaseCoordinateListLog(void) const {

		coordinate_list tmp_l;

		// log の最初は未登録状態の場合のダミー基準点だから除外
		for (std::size_t i = 1; i < this->log.size(); i++) {
			tmp_l.push_back(this->log[i].getBasePoint());
		}

		return tmp_l;
	}

	std::vector<coordinate_list> LoggingDatabase::getSuggestedCoordinateListsLog(void) const {

		std::vector<coordinate_list> tmp_l;

		for (const BasePointLog& b_log : this->log) {
			for (const SuggestGroupLog& sg_log : b_log.getSuggestLogList()) {
				tmp_l.push_back(sg_log.getCandidateList());
			}
		}

		return tmp_l;
	}

	std::vector<dsice_measure_data> LoggingDatabase::getMeasuredDataLog(void) const {

		std::vector<dsice_measure_data> tmp_d;

		for (const BasePointLog& b_log : this->log) {
			for (const SuggestGroupLog& sg_log : b_log.getSuggestLogList()) {
				for (const SamplingLog& sl_log : sg_log.getMeasuredDataList()) {
					tmp_d.emplace_back(sl_log.getSampleCoordinate(), sl_log.getMetricValue());
				}
			}
		}

		return tmp_d;
	}

	const std::vector<BasePointLog>& LoggingDatabase::getAllMeasuringLog(void) const {
		return this->log;
	}
}