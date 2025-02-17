#include <stdexcept>
#include <memory>
#include <limits>
#include <algorithm>

#include "dsice_search_space.hpp"
#include "dsice_database_core.hpp"
#include "full_searcher.hpp"

namespace dsice {

	FullSearcher::FullSearcher(const coordinate_list& target_coordinates, bool low_value_is_better) {

		this->search_target = target_coordinates;
		this->suggested_targets = target_coordinates;
		this->lower_is_better = low_value_is_better;

		if (this->lower_is_better) {
			this->best_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_point_value = std::numeric_limits<double>::lowest();
		}

		if (this->suggested_targets.empty()) {
			this->is_finish_searching = true;
		}
		else {
			this->best_judged_point_coordinate = this->suggested_targets[0];
		}
	}

	FullSearcher::FullSearcher(const FullSearcher& original) {
		this->search_target = original.search_target;
		this->suggested_targets = original.suggested_targets;
		this->measured_coordinates = original.measured_coordinates;
		this->lower_is_better = original.lower_is_better;
		this->measured_coordinate_buffer = original.measured_coordinate_buffer;
		this->measured_metric_value_buffer = original.measured_metric_value_buffer;
		this->best_judged_point_coordinate = original.best_judged_point_coordinate;
		this->best_measured_point_coordinate = original.best_measured_point_coordinate;
		this->best_point_value = original.best_point_value;
		this->is_finish_searching = original.is_finish_searching;
	}

	FullSearcher::~FullSearcher(void) {
		// DO_NOTHING
	}

	const coordinate& FullSearcher::getSuggested(void) const {
		if (this->is_finish_searching) {
			return this->best_judged_point_coordinate;
		}
		else {
			return this->suggested_targets[0];
		}
	}

	const coordinate_list& FullSearcher::getSuggestedList(void) const {
		return this->suggested_targets;
	}

	void FullSearcher::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->measured_coordinate_buffer.push_back(measured_coordinate);
		this->measured_metric_value_buffer.push_back(metric_value);
	}

	bool FullSearcher::updateState(void) {

		if (this->measured_coordinate_buffer.empty()) {
			return false;
		}

		for (std::size_t i = 0; i < this->measured_coordinate_buffer.size(); i++) {

			const coordinate& measured_coordinate = this->measured_coordinate_buffer[i];
			double metric_value = this->measured_metric_value_buffer[i];
			this->measured_coordinates.insert(measured_coordinate);

			if (this->lower_is_better) {
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

			auto target = std::find(this->suggested_targets.begin(), this->suggested_targets.end(), measured_coordinate);
			if (target != this->suggested_targets.end()) {
				this->suggested_targets.erase(target);
			}
		}

		// 全点探索が終わったら終了条件を満たしたことだけ保存し，対象を補充
		if (suggested_targets.empty()) {
			this->is_finish_searching = true;
			this->suggested_targets = this->search_target;
		}

		this->measured_coordinate_buffer.clear();
		this->measured_metric_value_buffer.clear();

		return true;
	}

	bool FullSearcher::isSearchFinished(void) const {
		return this->is_finish_searching;
	}

	const coordinate_list& FullSearcher::getTargetCoordinate(void) const {
		return this->search_target;
	}

	const std::unordered_set<coordinate>& FullSearcher::getMeasuredCoordinates(void) const {
		return this->measured_coordinates;
	}

	const coordinate& FullSearcher::getBestJudgedCoordinate(void) const {
		return this->best_judged_point_coordinate;
	}

	const coordinate& FullSearcher::getBestMeasuredCoordinate(void) const {
		return this->best_measured_point_coordinate;
	}
}