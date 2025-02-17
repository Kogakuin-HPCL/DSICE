#include <memory>

#include "dsice_search_space.hpp"
#include "uni_measurer.hpp"

namespace dsice {

	UniMeasurer::UniMeasurer(const coordinate& target_coordinate) {
		this->target = { target_coordinate };
	}

	UniMeasurer::UniMeasurer(const UniMeasurer& original) {
		this->target = original.target;
		this->measured_coordinate_buffer = original.measured_coordinate_buffer;
		this->measured_coordinates = original.measured_coordinates;
	}

	UniMeasurer::~UniMeasurer(void) {
		// DO_NOTHING
	}

	const coordinate& UniMeasurer::getSuggested(void) const {
		return this->target[0];
	}

	const coordinate_list& UniMeasurer::getSuggestedList(void) const {
		return this->target;
	}

	// 第 2 引数は使わないため無視
	void UniMeasurer::setMetricValue(const coordinate& measured_coordinate, double) {
		this->measured_coordinate_buffer.insert(measured_coordinate);
	}

	bool UniMeasurer::updateState(void) {
		
		if (this->measured_coordinate_buffer.empty()) {
			return false;
		}

		this->measured_coordinates.insert(this->target[0]);

		this->measured_coordinate_buffer.clear();

		return true;
	}

	bool UniMeasurer::isSearchFinished(void) const {
		return this->measured_coordinates.contains(this->target[0]);
	}

	const coordinate_list& UniMeasurer::getTargetCoordinate(void) const {
		return this->target;
	}

	const std::unordered_set<coordinate>& UniMeasurer::getMeasuredCoordinates(void) const {
		return this->measured_coordinates;
	}

	const coordinate& UniMeasurer::getBestJudgedCoordinate(void) const {
		return this->target[0];
	}

	const coordinate& UniMeasurer::getBestMeasuredCoordinate(void) const {
		return this->target[0];
	}
}