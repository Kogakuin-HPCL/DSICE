#include <memory>
#include <vector>
#include <limits>
#include <random>
#include <algorithm>

#include "dsice_search_space.hpp"
#include "dsice_database_core.hpp"
#include "simple_lhd_searcher.hpp"

namespace dsice {

	SimpleLhdSearcher::SimpleLhdSearcher(std::shared_ptr<const DatabaseCore> search_database, bool low_value_is_better) {

		this->lower_is_better = low_value_is_better;

		if (this->lower_is_better) {
			this->best_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_point_value = std::numeric_limits<double>::lowest();
		}

		const space_size& space = search_database->getSpaceSize();

		std::size_t max_axis_length = 0;

		// 最も単純な LHD である斜め配置を生成
		std::vector<std::vector<std::size_t>> indexes;
		for (std::size_t i = 0; i < space.size(); i++) {

			if (space[i] > max_axis_length) {
				max_axis_length = space[i];
			}

			indexes.push_back(std::vector<std::size_t>());

			for (std::size_t j = 0; j < space[i]; j++) {
				indexes.back().push_back(j);
			}
		}

		// シャッフルにより斜めよりはマシな LHD に変換
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());
		for (std::size_t i = 1; i < space.size(); i++) {
			for (std::size_t j = 0; j < space[i]; j++) {
				
				std::size_t shuffle_idx = engine() % space[i];
				
				std::size_t tmp = indexes[i][j];
				indexes[i][j] = indexes[i][shuffle_idx];
				indexes[i][shuffle_idx] = tmp;
			}
		}

		// 生成されたインデックスを座標変換し，探索対象を決定
		for (std::size_t i = 0; i < max_axis_length; i++) {

			coordinate tmp;

			for (std::size_t j = 0; j < space.size(); j++) {

				// 全ライン網羅を保証するため，短い軸はループ
				std::size_t looped_idx = i % indexes[j].size();

				tmp.push_back(indexes[j][looped_idx]);
			}

			this->target_coordinates.push_back(tmp);

			if (search_database->hasSample(tmp)) {

				this->measured_coordinates.insert(tmp);
				double v = search_database->getSampleMetricValue(tmp);

				if (this->lower_is_better) {
					if (v < this->best_point_value) {
						this->best_point_coordinate = tmp;
						this->best_point_value = v;
					}
				}
				else {
					if (v > this->best_point_value) {
						this->best_point_coordinate = tmp;
						this->best_point_value = v;
					}
				}
			}
			else {
				this->suggest_coordinates.push_back(tmp);
			}
		}
	}

	SimpleLhdSearcher::SimpleLhdSearcher(const SimpleLhdSearcher& original) {
		this->target_coordinates = original.target_coordinates;
		this->suggest_coordinates = original.suggest_coordinates;
		this->measured_coordinates = original.measured_coordinates;
		this->lower_is_better = original.lower_is_better;
		this->measured_coordinate_buffer = original.measured_coordinate_buffer;
		this->measured_metric_value_buffer = original.measured_metric_value_buffer;
		this->best_point_coordinate = original.best_point_coordinate;
		this->best_point_value = original.best_point_value;
	}

	SimpleLhdSearcher::~SimpleLhdSearcher(void) {
		// DO_NOTHING
	}

	const coordinate& SimpleLhdSearcher::getSuggested(void) const {

		if (this->suggest_coordinates.empty()) {
			return this->best_point_coordinate;
		}

		return this->suggest_coordinates[0];
	}

	const coordinate_list& SimpleLhdSearcher::getSuggestedList(void) const {
		return this->suggest_coordinates;
	}

	void SimpleLhdSearcher::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->measured_coordinate_buffer.push_back(measured_coordinate);
		this->measured_metric_value_buffer.push_back(metric_value);
	}

	bool SimpleLhdSearcher::updateState(void) {

		if (this->measured_coordinate_buffer.empty()) {
			return false;
		}

		for (std::size_t i = 0; i < this->measured_coordinate_buffer.size(); i++) {

			const coordinate& measured_coordinate = this->measured_coordinate_buffer[i];
			double metric_value = this->measured_metric_value_buffer[i];
			this->measured_coordinates.insert(measured_coordinate);

			if (this->lower_is_better) {
				if (metric_value < this->best_point_value) {
					this->best_point_coordinate = measured_coordinate;
					this->best_point_value = metric_value;
				}
			}
			else {
				if (metric_value > this->best_point_value) {
					this->best_point_coordinate = measured_coordinate;
					this->best_point_value = metric_value;
				}
			}

			auto target = std::find(this->suggest_coordinates.begin(), this->suggest_coordinates.end(), measured_coordinate);
			if (target != this->suggest_coordinates.end()) {
				this->suggest_coordinates.erase(target);
			}
		}

		this->measured_coordinate_buffer.clear();
		this->measured_metric_value_buffer.clear();

		return true;
	}

	bool SimpleLhdSearcher::isSearchFinished(void) const {
		return this->suggest_coordinates.empty();
	}

	const coordinate_list& SimpleLhdSearcher::getTargetCoordinate(void) const {
		return this->target_coordinates;
	}

	const std::unordered_set<coordinate>& SimpleLhdSearcher::getMeasuredCoordinates(void) const {
		return this->measured_coordinates;
	}

	const coordinate& SimpleLhdSearcher::getBestJudgedCoordinate(void) const {
		return this->best_point_coordinate;
	}

	const coordinate& SimpleLhdSearcher::getBestMeasuredCoordinate(void) const {
		return this->best_point_coordinate;
	}
}