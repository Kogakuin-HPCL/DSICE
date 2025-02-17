#include <stdexcept>
#include <memory>
#include <vector>
#include <limits>
#include <algorithm>

#include "dsice_search_space.hpp"
#include "tri_equally_indexer.hpp"
#include "direction_line.hpp"
#include "dsice_database_core.hpp"
#include "line_iterative_tri_searcher.hpp"

namespace dsice {
	
	LineIterativeTriSearcher::LineIterativeTriSearcher(std::shared_ptr<const DatabaseCore> database, const coordinate& example_coordinate, const DirectionLine& direction_info, bool low_value_is_better) :
		line(database->getSpaceSize(), example_coordinate, direction_info)
	{
		this->low_is_better = low_value_is_better;
		
		TriEquallyIndexer indexer(0, this->line.getPointsNum() - 1);
		this->tri_indexes = indexer.getIndexes();

		coordinate_list points = this->line.getPoints();

		this->best_judged_point_coordinate = points[0];
		if (this->low_is_better) {
			this->best_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_point_value = std::numeric_limits<double>::lowest();
		}

		// 3 分割点で未探索のものを順に探索候補に加える．同時に，探索済の点から最良点を見つけて格納
		for (std::size_t i : this->tri_indexes) {

			coordinate c = points[i];

			if (database->hasSample(c)) {

				this->measured_coordinates.insert(c);
				double v = database->getSampleMetricValue(c);

				if (this->low_is_better) {
					if (v < this->best_point_value) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
				else {
					if (v > this->best_point_value) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
			}
			else {
				this->suggested_coordinates.push_back(c);
			}
		}

		if (this->suggested_coordinates.empty()) {
			this->suggested_coordinates = this->line.getPoints();
			this->is_finish_searching = true;
		}
	}

	LineIterativeTriSearcher::LineIterativeTriSearcher(std::shared_ptr<const DatabaseCore> database, const coordinate& coordinate1, const coordinate& coordinate2, bool low_value_is_better):
		line(database->getSpaceSize(), coordinate1, coordinate2)
	{
		if (coordinate1 == coordinate2) {
			throw std::invalid_argument("two coordinates must not be the same.");
		}

		this->low_is_better = low_value_is_better;

		TriEquallyIndexer indexer(0, this->line.getPointsNum() - 1);
		this->tri_indexes = indexer.getIndexes();

		coordinate_list points = this->line.getPoints();

		this->best_judged_point_coordinate = points[0];
		if (this->low_is_better) {
			this->best_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_point_value = std::numeric_limits<double>::lowest();
		}

		// 3 分割点で未探索のものを順に探索候補に加える．同時に，探索済の点から最良点を見つけて格納
		for (std::size_t i : this->tri_indexes) {

			coordinate c = points[i];

			if (database->hasSample(c)) {

				this->measured_coordinates.insert(c);
				double v = database->getSampleMetricValue(c);

				if (this->low_is_better) {
					if (v < this->best_point_value) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
				else {
					if (v > this->best_point_value) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
			}
			else {
				this->suggested_coordinates.push_back(c);
			}
		}

		if (this->suggested_coordinates.empty()) {
			this->suggested_coordinates = this->line.getPoints();
			this->is_finish_searching = true;
		}
	}

	LineIterativeTriSearcher::LineIterativeTriSearcher(std::shared_ptr<const DatabaseCore> database, const CoordinateLine& line_info, bool low_value_is_better) :
		line(line_info)
	{
		this->low_is_better = low_value_is_better;

		TriEquallyIndexer indexer(0, this->line.getPointsNum() - 1);
		this->tri_indexes = indexer.getIndexes();

		coordinate_list points = this->line.getPoints();

		this->best_judged_point_coordinate = points[0];
		if (this->low_is_better) {
			this->best_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_point_value = std::numeric_limits<double>::lowest();
		}

		// 3 分割点で未探索のものを順に探索候補に加える．同時に，探索済の点から最良点を見つけて格納
		for (std::size_t i : this->tri_indexes) {

			coordinate c = points[i];

			if (database->hasSample(c)) {

				this->measured_coordinates.insert(c);
				double v = database->getSampleMetricValue(c);

				if (this->low_is_better) {
					if (v < this->best_point_value) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
				else {
					if (v > this->best_point_value) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
			}
			else {
				this->suggested_coordinates.push_back(c);
			}
		}

		if (this->suggested_coordinates.empty()) {
			this->suggested_coordinates = this->line.getPoints();
			this->is_finish_searching = true;
		}
	}

	LineIterativeTriSearcher::LineIterativeTriSearcher(const LineIterativeTriSearcher& original) :
		line(original.line)
	{
		this->suggested_coordinates = original.suggested_coordinates;
		this->tri_indexes = original.tri_indexes;
		this->low_is_better = original.low_is_better;
		this->measured_coordinate_buffer = original.measured_coordinate_buffer;
		this->measured_metric_value_buffer = original.measured_metric_value_buffer;
		this->best_judged_point_coordinate = original.best_judged_point_coordinate;
		this->best_measured_point_coordinate = original.best_measured_point_coordinate;
		this->best_point_value = original.best_point_value;
		this->is_finish_searching = original.is_finish_searching;
	}

	LineIterativeTriSearcher::~LineIterativeTriSearcher(void) {
		// DO_NOTHING
	}

	const CoordinateLine& LineIterativeTriSearcher::getLineInfomation(void) const {
		return this->line;
	}

	const coordinate& LineIterativeTriSearcher::getSuggested(void) const {

		if (this->suggested_coordinates.empty()) {
			return this->best_judged_point_coordinate;
		}
		else {
			return this->suggested_coordinates[0];
		}
	}

	const coordinate_list& LineIterativeTriSearcher::getSuggestedList(void) const {
		return this->suggested_coordinates;
	}

	void LineIterativeTriSearcher::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->measured_coordinate_buffer.push_back(measured_coordinate);
		this->measured_metric_value_buffer.push_back(metric_value);
	}

	bool LineIterativeTriSearcher::updateState(void) {

		if (this->measured_coordinate_buffer.empty()) {
			return false;
		}

		for (std::size_t i = 0; i < this->measured_coordinate_buffer.size(); i++) {

			const coordinate& measured_coordinate = this->measured_coordinate_buffer[i];
			double metric_value = this->measured_metric_value_buffer[i];
			this->measured_coordinates.insert(measured_coordinate);

			if (this->low_is_better) {
				if (metric_value < this->best_point_value) {
					this->best_judged_point_coordinate = measured_coordinate;
					this->best_measured_point_coordinate = measured_coordinate;
					this->best_point_value = metric_value;
				}
			}
			else {
				if (metric_value > this->best_point_value) {
					this->best_judged_point_coordinate = measured_coordinate;
					this->best_measured_point_coordinate = measured_coordinate;
					this->best_point_value = metric_value;
				}
			}

			auto target = std::find(this->suggested_coordinates.begin(), this->suggested_coordinates.end(), measured_coordinate);
			if (target != this->suggested_coordinates.end()) {
				this->suggested_coordinates.erase(target);
			}
		}

		if (this->suggested_coordinates.empty()) {
			this->is_finish_searching = true;
			this->suggested_coordinates = this->line.getPoints();
		}

		this->measured_coordinate_buffer.clear();
		this->measured_metric_value_buffer.clear();

		return true;
	}

	bool LineIterativeTriSearcher::isSearchFinished(void) const {
		return this->is_finish_searching;
	}

	const coordinate_list& LineIterativeTriSearcher::getTargetCoordinate(void) const {
		return this->line.getPoints();
	}

	const std::unordered_set<coordinate>& LineIterativeTriSearcher::getMeasuredCoordinates(void) const {
		return this->measured_coordinates;
	}

	const coordinate& LineIterativeTriSearcher::getBestJudgedCoordinate(void) const {
		return this->best_judged_point_coordinate;
	}

	const coordinate& LineIterativeTriSearcher::getBestMeasuredCoordinate(void) const {
		return this->best_measured_point_coordinate;
	}
}