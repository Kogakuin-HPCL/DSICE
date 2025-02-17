#include <memory>
#include <limits>

#include "dsice_metric.hpp"
#include "overwritten_double.hpp"
#include "average_double.hpp"
#include "dsice_search_space.hpp"
#include "dsice_standard_database.hpp"

namespace dsice {

	StandardDatabase::StandardDatabase(const space_size& search_space, DatabaseMetricType database_metric_type) {
		this->search_space_size = search_space;
		this->metric_type = database_metric_type;
	}

	StandardDatabase::StandardDatabase(const StandardDatabase& original) {
		
		this->search_space_size = original.search_space_size;

		for (const std::pair<const coordinate, std::shared_ptr<Metric>>& original_sample : original.samples) {
			this->samples.emplace(original_sample.first, original_sample.second->makeCopy());
		}

		this->base_points = original.base_points;
		this->latest_base_coordinate = original.latest_base_coordinate;
		this->has_base_coordinate_changed = original.has_base_coordinate_changed;
		this->latest_sample_coordinates = original.latest_sample_coordinates;
		this->latest_measured_values = original.latest_measured_values;
		this->metric_type = original.metric_type;
	}

	StandardDatabase::~StandardDatabase(void) {
		// DO_NOTHING
	}

	dimension_size StandardDatabase::getSpaceDimension(void) const {
		return this->search_space_size.size();
	}

	const space_size& StandardDatabase::getSpaceSize(void) const {
		return this->search_space_size;
	}

	void StandardDatabase::setSampleMetricValue(const coordinate& target_coordinate, double metric_value) {

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

	bool StandardDatabase::hasSample(const coordinate& target_coordinate) const {
		return this->samples.contains(target_coordinate);
	}

	std::size_t StandardDatabase::getExistSamplesNum(void) const {
		return this->samples.size();
	}

	double StandardDatabase::getSampleMetricValue(const coordinate& target_coordinate) const {
		
		if (this->samples.contains(target_coordinate)) {
			return this->samples.at(target_coordinate)->getValue();
		}
		else {
			return std::numeric_limits<double>::quiet_NaN();
		}
	}

	void StandardDatabase::setBasePoint(const coordinate& target_coordinate) {

		if (this->latest_base_coordinate != target_coordinate) {

			this->has_base_coordinate_changed = true;
			this->latest_base_coordinate = target_coordinate;

			if (!this->base_points.contains(target_coordinate)) {
				this->base_points.emplace(target_coordinate, std::unordered_set<DirectionLine>());
			}
		}
		else {
			this->has_base_coordinate_changed = false;
		}
	}

	const std::unordered_map<coordinate, std::unordered_set<DirectionLine>>& StandardDatabase::getBasePoints(void) const {
		return this->base_points;
	}

	bool StandardDatabase::hasBeenBasePoint(const coordinate& target_coordinate) const {
		return this->base_points.contains(target_coordinate);
	}

	void StandardDatabase::recordSearchedDirection(const coordinate& base_coordinate, const DirectionLine& line_info) {

		if (!this->base_points.contains(base_coordinate)) {
			this->base_points.emplace(base_coordinate, std::unordered_set<DirectionLine>());
		}

		this->base_points.at(base_coordinate).insert(line_info);
	}

	bool StandardDatabase::isSearchedDirection(const coordinate& base_coordinate, const DirectionLine& line_info) const {
		
		if (this->base_points.contains(base_coordinate)) {
			if (this->base_points.at(base_coordinate).contains(line_info)) {
				return true;
			}
		}

		return false;
	}

	void StandardDatabase::setLoopEnd(void) {
		this->latest_sample_coordinates.clear();
		this->latest_measured_values.clear();
	}

	const coordinate_list& StandardDatabase::getLatestSampleCoordinate(void) const {
		return this->latest_sample_coordinates;
	}

	const std::vector<double>& StandardDatabase::getLatestMeasuredValue(void) const {
		return this->latest_measured_values;
	}

	const coordinate& StandardDatabase::getLatestBaseCoordinate(void) const {
		return this->latest_base_coordinate;
	}

	bool StandardDatabase::hasBaseCoordinateChanged(void) const {
		return this->has_base_coordinate_changed;
	}
}