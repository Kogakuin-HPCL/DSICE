#include <memory>
#include <algorithm>

#include "dsice_search_space.hpp"
#include "coordinate_line.hpp"
#include "tri_equally_indexer.hpp"
#include "dsice_database_core.hpp"
#include "line_iterative_tri_searcher.hpp"
#include "line_single_tri_searcher.hpp"

namespace dsice {

	void LineSingleTriSearcher::listupTriPoints(std::shared_ptr<const DatabaseCore> database) {

		const CoordinateLine& line = this->substance.getLineInfomation();

		std::size_t left = 0;
		std::size_t right = line.getPointsNum() - 1;
		std::pair<std::size_t, std::size_t> tmp_mids = TriEquallyIndexer::getTriPoints(left, right);

		if (!database->hasSample(line.getPointCoordinate(left))) {
			this->tri_points.push_back(line.getPointCoordinate(left));
		}
		if (!database->hasSample(line.getPointCoordinate(right))) {
			this->tri_points.push_back(line.getPointCoordinate(right));
		}
		if (!database->hasSample(line.getPointCoordinate(tmp_mids.first))) {
			this->tri_points.push_back(line.getPointCoordinate(tmp_mids.first));
		}
		if (!database->hasSample(line.getPointCoordinate(tmp_mids.second))) {
			this->tri_points.push_back(line.getPointCoordinate(tmp_mids.second));
		}
	}

	LineSingleTriSearcher::LineSingleTriSearcher(std::shared_ptr<const DatabaseCore> database, const coordinate& example_coordinate, const DirectionLine& direction_info, bool low_value_is_better) :
		substance(database, example_coordinate, direction_info, low_value_is_better)
	{
		this->listupTriPoints(database);
	}

	LineSingleTriSearcher::LineSingleTriSearcher(std::shared_ptr<const DatabaseCore> database, const coordinate& coordinate1, const coordinate& coordinate2, bool low_value_is_better) :
		substance(database, coordinate1, coordinate2, low_value_is_better)
	{
		this->listupTriPoints(database);
	}

	LineSingleTriSearcher::LineSingleTriSearcher(std::shared_ptr<const DatabaseCore> database, const CoordinateLine& line_info, bool low_value_is_better) :
		substance(database, line_info, low_value_is_better)
	{
		this->listupTriPoints(database);
	}

	LineSingleTriSearcher::LineSingleTriSearcher(const LineSingleTriSearcher& original) :
		substance(original.substance)
	{
		this->tri_points = original.tri_points;
		this->measured_coordinate_buffer = original.measured_coordinate_buffer;
	}

	LineSingleTriSearcher::~LineSingleTriSearcher(void) {
		// DO_NOTHING
	}

	const CoordinateLine& LineSingleTriSearcher::getLineInfomation(void) const {
		return this->substance.getLineInfomation();
	}

	const coordinate& LineSingleTriSearcher::getSuggested(void) const {
		return this->substance.getSuggested();
	}

	const coordinate_list& LineSingleTriSearcher::getSuggestedList(void) const {
		return this->substance.getSuggestedList();
	}

	void LineSingleTriSearcher::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->substance.setMetricValue(measured_coordinate, metric_value);
		this->measured_coordinate_buffer.push_back(measured_coordinate);
	}

	bool LineSingleTriSearcher::updateState(void) {

		if (this->measured_coordinate_buffer.empty()) {
			return false;
		}

		for (const coordinate& measured_coordinate : this->measured_coordinate_buffer) {
			auto target = std::find(this->tri_points.begin(), this->tri_points.end(), measured_coordinate);
			if (target != this->tri_points.end()) {
				this->tri_points.erase(target);
			}
		}

		this->measured_coordinate_buffer.clear();

		this->substance.updateState();

		return true;
	}

	bool LineSingleTriSearcher::isSearchFinished(void) const {
		return this->tri_points.empty();
	}

	const coordinate_list& LineSingleTriSearcher::getTargetCoordinate(void) const {
		return this->substance.getTargetCoordinate();
	}

	const std::unordered_set<coordinate>& LineSingleTriSearcher::getMeasuredCoordinates(void) const {
		return this->substance.getMeasuredCoordinates();
	}

	const coordinate& LineSingleTriSearcher::getBestJudgedCoordinate(void) const {
		return this->substance.getBestJudgedCoordinate();
	}

	const coordinate& LineSingleTriSearcher::getBestMeasuredCoordinate(void) const {
		return this->substance.getBestMeasuredCoordinate();
	}
}
