#include <stdexcept>
#include <memory>
#include <limits>
#include <algorithm>

#include "dsice_search_space.hpp"
#include "dsice_database_standard_functions.hpp"
#include "direction_line.hpp"
#include "coordinate_line.hpp"
#include "around_points.hpp"
#include "simple_direction_searcher.hpp"

namespace dsice {

	SimpleDirectionSearcher::SimpleDirectionSearcher(std::shared_ptr<const DatabaseStandardFunctions> search_database, const coordinate& target_coordinate, std::size_t max_changed_axis_num, bool low_value_is_better) {

		this->base_coordinate = target_coordinate;
		this->lower_is_better = low_value_is_better;
		this->best_judged_point_coordinate = target_coordinate;

		if (this->lower_is_better) {
			this->best_judged_point_value = std::numeric_limits<double>::max();
			this->best_measured_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_judged_point_value = std::numeric_limits<double>::lowest();
			this->best_measured_point_value = std::numeric_limits<double>::lowest();
		}

		if (search_database->hasSample(target_coordinate)) {
			this->best_judged_point_value = search_database->getSampleMetricValue(target_coordinate);
		}

		AroundPoints points(search_database->getSpaceSize(), target_coordinate, max_changed_axis_num);
		this->around_coordinates = points.getAroundPoints();

		for (const coordinate& c : this->around_coordinates) {

			// 探索済方向は除外
			DirectionLine direction(this->base_coordinate, c);
			if (search_database->isSearchedDirection(this->base_coordinate, direction)) {
				continue;
			}

			if (search_database->hasSample(c)) {

				this->measured_coordinates.insert(c);
				this->measured_line.insert(direction);
				double v = search_database->getSampleMetricValue(c);

				if (this->lower_is_better) {

					if (v < this->best_judged_point_value) {
						this->best_judged_point_coordinate = c;
						this->best_judged_point_value = v;
					}

					if (v < this->best_measured_point_value) {
						this->best_measured_point_coordinate = c;
						this->best_measured_point_value = v;
					}
				}
				else {

					if (v > this->best_judged_point_value) {
						this->best_judged_point_coordinate = c;
						this->best_judged_point_value = v;
					}

					if (v > this->best_measured_point_value) {
						this->best_measured_point_coordinate = c;
						this->best_measured_point_value = v;
					}
				}
			}
			else {
				this->suggested_coordiates.push_back(c);
			}
		}

		if (this->suggested_coordiates.empty()) {
			this->is_finish_searching = true;
			this->suggested_coordiates = this->around_coordinates;
		}
	}

	SimpleDirectionSearcher::SimpleDirectionSearcher(const SimpleDirectionSearcher& original) {
		this->around_coordinates = original.around_coordinates;
		this->suggested_coordiates = original.suggested_coordiates;
		this->measured_coordinates = original.measured_coordinates;
		this->base_coordinate = original.base_coordinate;
		this->lower_is_better = original.lower_is_better;
		this->measured_coordinate_buffer = original.measured_coordinate_buffer;
		this->measured_metric_value_buffer = original.measured_metric_value_buffer;
		this->best_judged_point_coordinate = original.best_judged_point_coordinate;
		this->best_judged_point_value = original.best_judged_point_value;
		this->best_measured_point_coordinate = original.best_measured_point_coordinate;
		this->best_measured_point_value = original.best_measured_point_value;
		this->is_finish_searching = original.is_finish_searching;
		this->measured_line = original.measured_line;
	}

	SimpleDirectionSearcher::~SimpleDirectionSearcher(void) {
		// DO_NOTHING
	}

	const coordinate& SimpleDirectionSearcher::getSuggested(void) const {

		if (this->is_finish_searching) {
			return this->best_judged_point_coordinate;
		}
		else {
			return this->suggested_coordiates[0];
		}
	}

	const coordinate_list& SimpleDirectionSearcher::getSuggestedList(void) const {
		return this->suggested_coordiates;
	}

	void SimpleDirectionSearcher::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->measured_coordinate_buffer.push_back(measured_coordinate);
		this->measured_metric_value_buffer.push_back(metric_value);
	}

	bool SimpleDirectionSearcher::updateState(void) {

		if (this->measured_coordinate_buffer.empty()) {
			return false;
		}

		for (std::size_t i = 0; i < this->measured_coordinate_buffer.size(); i++) {

			const coordinate& measured_coordinate = this->measured_coordinate_buffer[i];
			double metric_value = this->measured_metric_value_buffer[i];
			this->measured_coordinates.insert(measured_coordinate);

			DirectionLine direction(this->base_coordinate, measured_coordinate);
			this->measured_line.insert(direction);

			if (this->lower_is_better) {

				if (metric_value < this->best_judged_point_value) {
					this->best_judged_point_coordinate = measured_coordinate;
					this->best_judged_point_value = metric_value;
				}

				if (metric_value < this->best_measured_point_value) {
					this->best_measured_point_coordinate = measured_coordinate;
					this->best_measured_point_value = metric_value;
				}
			}
			else {

				if (metric_value > this->best_judged_point_value) {
					this->best_judged_point_coordinate = measured_coordinate;
					this->best_judged_point_value = metric_value;
				}

				if (metric_value > this->best_measured_point_value) {
					this->best_measured_point_coordinate = measured_coordinate;
					this->best_measured_point_value = metric_value;
				}
			}

			// O(N) だが，AroundPoints の周辺点列挙に比べれば気にならない程度のはず
			coordinate_list::iterator erase_target = std::find(this->suggested_coordiates.begin(), this->suggested_coordiates.end(), measured_coordinate);
			if (erase_target != this->suggested_coordiates.end()) {
				this->suggested_coordiates.erase(erase_target);
			}
		}

		if (this->suggested_coordiates.empty()) {
			this->is_finish_searching = true;
			this->suggested_coordiates = this->around_coordinates;
		}

		this->measured_coordinate_buffer.clear();
		this->measured_metric_value_buffer.clear();

		return true;
	}

	bool SimpleDirectionSearcher::isSearchFinished(void) const {
		return this->is_finish_searching;
	}

	const coordinate_list& SimpleDirectionSearcher::getTargetCoordinate(void) const {
		return this->around_coordinates;
	}

	const std::unordered_set<coordinate>& SimpleDirectionSearcher::getMeasuredCoordinates(void) const {
		return this->measured_coordinates;
	}

	const coordinate& SimpleDirectionSearcher::getBestJudgedCoordinate(void) const {
		return this->best_judged_point_coordinate;
	}

	const coordinate& SimpleDirectionSearcher::getBestMeasuredCoordinate(void) const {
		return this->best_measured_point_coordinate;
	}

	const std::unordered_set<DirectionLine>& SimpleDirectionSearcher::getMeasuredDirection(void) const {
		return this->measured_line;
	}
}