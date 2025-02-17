#include <memory>
#include <vector>
#include <unordered_map>
#include <limits>

#include "dsice_search_space.hpp"
#include "direction_line.hpp"
#include "around_points.hpp"
#include "dsice_database_core.hpp"
#include "dsice_database_base_point_extension.hpp"
#include "uni_measurer.hpp"
#include "line_single_tri_searcher.hpp"
#include "one_dim_dsp_searcher.hpp"
#include "radial_dsp_searcher.hpp"

namespace dsice {

	void RadialDspSearcher::updateSuggested(void) {

		this->suggested_coordinates.clear();

		for (std::size_t i = 0; i < this->searchers.size(); i++) {
			
			std::size_t max_suggested_length = 0;
			std::vector<coordinate_list> suggested;

			for (std::size_t j = 0; j < this->searchers[i].size(); j++) {
				if (this->searchers[i][j].isRunning()) {
					
					suggested.push_back(this->searchers[i][j].getSearcher()->getSuggestedList());
					
					if (suggested.back().size() > max_suggested_length) {
						max_suggested_length = suggested.back().size();
					}
				}
			}

			for (std::size_t j = 0; j < max_suggested_length; j++) {
				for (std::size_t k = 0; k < suggested.size(); k++) {
					if (j < suggested[k].size()) {
						this->suggested_coordinates.push_back(suggested[k][j]);
					}
				}
			}
		}
	}

	RadialDspSearcher::RadialDspSearcher(std::shared_ptr<const DatabaseCore> search_database, const coordinate& central_coordinate, std::size_t max_changed_axis_num, bool low_value_is_better, double alpha):
		database(search_database)
	{
		this->base_coordinate = central_coordinate;
		this->lower_is_better = low_value_is_better;
		this->dsp_alpha = alpha;
		this->best_judged_point_coordinate = this->base_coordinate;

		// Searcher を入れる場所を作成 (0 に中心実測機構，1 から 1 次元探索機構)
		for (std::size_t i = 0; i <= max_changed_axis_num; i++) {
			this->searchers.emplace_back();
		}

		if (this->database->hasSample(this->base_coordinate)) {
			this->best_point_value = this->database->getSampleMetricValue(this->base_coordinate);
		}
		else {
			
			this->searchers[0].emplace_back(std::make_shared<UniMeasurer>(this->base_coordinate));

			if (this->lower_is_better) {
				this->best_point_value = std::numeric_limits<double>::max();
			}
			else {
				this->best_point_value = std::numeric_limits<double>::lowest();
			}

			this->target_coorinates.push_back(this->base_coordinate);
			this->searchers_of_direction.emplace(DirectionLine(this->base_coordinate, this->base_coordinate), std::pair<std::size_t, std::size_t>(0, 0));
		}

		AroundPoints around_points(this->database->getSpaceSize(), this->base_coordinate, max_changed_axis_num);
		const coordinate_list& around_coordinates = around_points.getAroundPoints();
		std::shared_ptr<const DatabaseBasePointExtension> tmp_basepoint_database = std::dynamic_pointer_cast<const DatabaseBasePointExtension>(this->database);
		for (const coordinate& one_coor : around_coordinates) {

			DirectionLine target_line(this->base_coordinate, one_coor);

			// 探索済方向はスキップ
			if (tmp_basepoint_database->isSearchedDirection(this->base_coordinate, target_line)) {
				continue;
			}

			// 重複を避けて調査
			if (!this->searchers_of_direction.contains(target_line) && !this->finished_direction.contains(target_line)) {

				std::size_t axis_num = target_line.getDirectionDimension();
				std::size_t idx = this->searchers[axis_num].size();

				std::shared_ptr<LineSingleTriSearcher> new_searcher1 = std::make_shared<LineSingleTriSearcher>(this->database, this->base_coordinate, target_line, this->lower_is_better);
				if (new_searcher1->isSearchFinished()) {

					std::shared_ptr<OneDimDspSearcher> new_searcher2 = std::make_shared<OneDimDspSearcher>(this->database, this->base_coordinate, target_line, this->lower_is_better, this->dsp_alpha);

					if (new_searcher2->isSearchFinished()) {
						this->finished_direction.insert(target_line);
					}
					else {
						this->searchers[axis_num].emplace_back(new_searcher2);
						this->searchers_of_direction.emplace(target_line, std::pair<std::size_t, std::size_t>(axis_num, idx));
						this->target_coorinates.insert(this->target_coorinates.end(), new_searcher2->getTargetCoordinate().begin(), new_searcher2->getTargetCoordinate().end());
					}
				}
				else {
					this->searchers[axis_num].emplace_back(new_searcher1);
					this->searchers_of_direction.emplace(target_line, std::pair<std::size_t, std::size_t>(axis_num, idx));
					this->target_coorinates.insert(this->target_coorinates.end(), new_searcher1->getTargetCoordinate().begin(), new_searcher1->getTargetCoordinate().end());
				}
			}
		}

		this->updateSuggested();
	}

	RadialDspSearcher::RadialDspSearcher(const RadialDspSearcher& original):
		database(original.database)
	{
		this->base_coordinate = original.base_coordinate;
		this->searchers_of_direction = original.searchers_of_direction;
		this->finished_direction = original.finished_direction;
		this->target_coorinates = original.target_coorinates;
		this->suggested_coordinates = original.suggested_coordinates;
		this->lower_is_better = original.lower_is_better;
		this->dsp_alpha = original.dsp_alpha;
		this->measured_coordinate_buffer = original.measured_coordinate_buffer;
		this->measured_metric_value_buffer = original.measured_metric_value_buffer;
		this->is_logging_mode_on = original.is_logging_mode_on;
		this->best_judged_point_coordinate = original.best_judged_point_coordinate;
		this->best_measured_point_coordinate = original.best_measured_point_coordinate;
		this->best_point_value = original.best_point_value;
		this->is_found_better_point = original.is_found_better_point;

		for (std::size_t i = 0; i < original.searchers.size(); i++) {
			
			this->searchers.emplace_back();

			for (std::size_t j = 0; j < original.searchers[i].size(); j++) {
				if (std::shared_ptr<LineSingleTriSearcher> s1 = std::dynamic_pointer_cast<LineSingleTriSearcher>(original.searchers[i][j].getSearcher())) {
					this->searchers[i].emplace_back(std::make_shared<LineSingleTriSearcher>(*s1), original.searchers[i][j].isRunning());
				}
				else {
					std::shared_ptr<OneDimDspSearcher> s2 = std::dynamic_pointer_cast<OneDimDspSearcher>(original.searchers[i][j].getSearcher());
					this->searchers[i].emplace_back(std::make_shared<OneDimDspSearcher>(*s2), original.searchers[i][j].isRunning());
				}
			}
		}
	}

	RadialDspSearcher::~RadialDspSearcher(void) {
		// DO_NOTHING
	}

	const coordinate& RadialDspSearcher::getSuggested(void) const {
		if (this->suggested_coordinates.empty()) {
			return this->best_judged_point_coordinate;
		}
		else {
			return this->suggested_coordinates[0];
		}
	}

	const coordinate_list& RadialDspSearcher::getSuggestedList(void) const {
		return this->suggested_coordinates;
	}

	void RadialDspSearcher::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->measured_coordinate_buffer.push_back(measured_coordinate);
		this->measured_metric_value_buffer.push_back(metric_value);
	}

	bool RadialDspSearcher::updateState(void) {

		if (this->measured_coordinate_buffer.empty()) {
			return false;
		}

		// データ登録
		for (std::size_t i = 0; i < this->measured_coordinate_buffer.size(); i++) {

			const coordinate& measured_coordinate = this->measured_coordinate_buffer[i];
			double metric_value = this->measured_metric_value_buffer[i];
			this->measured_coordinates.insert(measured_coordinate);

			DirectionLine direction(this->base_coordinate, measured_coordinate);

			if (!this->searchers_of_direction.contains(direction)) {
				continue;
			}

			const std::pair<std::size_t, std::size_t>& searcher_idx = this->searchers_of_direction.at(direction);
			this->searchers[searcher_idx.first][searcher_idx.second].getSearcher()->setMetricValue(measured_coordinate, metric_value);
			this->searchers[searcher_idx.first][searcher_idx.second].getSearcher()->updateState();

			if (measured_coordinate == this->base_coordinate) {
				for (std::size_t j = 1; j < this->searchers.size();j++) {
					for (std::size_t k = 0; k < this->searchers[j].size(); k++) {
						this->searchers[j][k].getSearcher()->setMetricValue(measured_coordinate, metric_value);
						this->searchers[j][k].getSearcher()->updateState();
					}
				}
			}
		}

		std::vector<std::shared_ptr<Searcher>> tmp_finished_direction_searcher;

		// 探索機構の終了チェック
		if (!this->searchers[0].empty()) {
			if (this->searchers[0][0].isRunning()) {

				std::shared_ptr<Searcher> s = this->searchers[0][0].getSearcher();

				if (s->isSearchFinished()) {
					this->best_point_value = this->database->getSampleMetricValue(s->getBestMeasuredCoordinate());
				}

				this->searchers[0][0].setRunningState(false);
				this->searchers_of_direction.erase(DirectionLine(this->base_coordinate, this->base_coordinate));
			}
		}
		for (std::size_t i = 1; i < this->searchers.size(); i++) {
			for (std::size_t j = 0; j < this->searchers[i].size(); j++) {
				if (this->searchers[i][j].isRunning()) {

					std::shared_ptr<Searcher> s0 = this->searchers[i][j].getSearcher();

					if (s0->isSearchFinished()) {

						if (std::shared_ptr<LineSingleTriSearcher> s1 = std::dynamic_pointer_cast<LineSingleTriSearcher>(s0)) {

							std::shared_ptr<OneDimDspSearcher> new_searcher = std::make_shared<OneDimDspSearcher>(this->database, this->base_coordinate, s1->getLineInfomation().getLineDirection(), this->lower_is_better, this->dsp_alpha);

							if (new_searcher->isSearchFinished()) {
								this->searchers[i][j].setRunningState(false);
								this->searchers_of_direction.erase(s1->getLineInfomation().getLineDirection());
								this->finished_direction.insert(s1->getLineInfomation().getLineDirection());
								tmp_finished_direction_searcher.push_back(new_searcher);
							}
							else {
								this->searchers[i][j] = ManagedSearcher(new_searcher);
							}
						}
						else {
							std::shared_ptr<OneDimDspSearcher> s2 = std::dynamic_pointer_cast<OneDimDspSearcher>(s0);
							this->searchers[i][j].setRunningState(false);
							this->searchers_of_direction.erase(s2->getLineInfomation().getLineDirection());
							this->finished_direction.insert(s2->getLineInfomation().getLineDirection());
							tmp_finished_direction_searcher.push_back(s2);
						}
					}
				}
			}
		}

		// 全体の更新
		for (std::shared_ptr<Searcher> s : tmp_finished_direction_searcher) {

			double v = this->database->getSampleMetricValue(s->getBestMeasuredCoordinate());

			if (this->lower_is_better) {
				if (v < this->best_point_value) {
					this->best_measured_point_coordinate = s->getBestMeasuredCoordinate();
					this->best_judged_point_coordinate = s->getBestJudgedCoordinate();
					this->best_point_value = v;
					this->is_found_better_point = true;
				}
			}
			else {
				if (v > this->best_point_value) {
					this->best_measured_point_coordinate = s->getBestMeasuredCoordinate();
					this->best_judged_point_coordinate = s->getBestJudgedCoordinate();
					this->best_point_value = v;
					this->is_found_better_point = true;
				}
			}
		}

		this->updateSuggested();

		this->measured_coordinate_buffer.clear();
		this->measured_metric_value_buffer.clear();

		return true;
	}

	bool RadialDspSearcher::isSearchFinished(void) const {
		return this->searchers_of_direction.empty();
	}

	const coordinate_list& RadialDspSearcher::getTargetCoordinate(void) const {
		return this->target_coorinates;
	}

	const std::unordered_set<coordinate>& RadialDspSearcher::getMeasuredCoordinates(void) const {
		return this->measured_coordinates;
	}

	const coordinate& RadialDspSearcher::getBestJudgedCoordinate(void) const {
		return this->best_judged_point_coordinate;
	}

	const coordinate& RadialDspSearcher::getBestMeasuredCoordinate(void) const {
		return best_measured_point_coordinate;
	}

	std::size_t RadialDspSearcher::getManagedSearchersNum(void) const {
		return this->searchers_of_direction.size() + this->finished_direction.size();
	}

	std::size_t RadialDspSearcher::getRunningSearchersNum(void) const {
		return this->searchers_of_direction.size();
	}

	std::size_t RadialDspSearcher::getFinishedSearchersNum(void) const {
		return this->finished_direction.size();
	}

	bool RadialDspSearcher::isFoundBetterPoint(void) const {
		return this->is_found_better_point;
	}

	const std::unordered_set<DirectionLine>& RadialDspSearcher::getFinishedDirections(void) const {
		// 推定途中の方向はさらに良い値があってもおかしくないので返さない
		return this->finished_direction;
	}
}