#include <stdexcept>
#include <memory>
#include <vector>
#include <limits>
#include <algorithm>

#include "dsice_search_space.hpp"
#include "observed_d_spline.hpp"
#include "dsice_database_core.hpp"
#include "equally_spaced_d_spline.hpp"
#include "one_dim_dsp_searcher.hpp"

namespace dsice {

	void OneDimDspSearcher::suggestBasedOnApproximation(void) {

		this->suggest_target.clear();

		if (this->lower_is_better) {

			// 最良推定
			for (std::size_t i : this->d_spline->getLowerZoneSampleIndexes()) {

				const coordinate& tmp_c = this->line.getPointCoordinate(i);

				if (!this->measured_coordinates.contains(tmp_c)) {
					this->suggest_target.push_back(tmp_c);
				}
			}

			// 最悪推定
			for (std::size_t i : this->d_spline->getHigherZoneSampleIndexes()) {
				
				const coordinate& tmp_c = this->line.getPointCoordinate(i);

				if (!this->measured_coordinates.contains(tmp_c)) {
					this->suggest_target.push_back(tmp_c);
				}
			}
		}
		else {

			// 最良推定
			for (std::size_t i : this->d_spline->getHigherZoneSampleIndexes()) {
				
				const coordinate& tmp_c = this->line.getPointCoordinate(i);

				if (!this->measured_coordinates.contains(tmp_c)) {
					this->suggest_target.push_back(tmp_c);
				}
			}

			// 最悪推定
			for (std::size_t i : this->d_spline->getLowerZoneSampleIndexes()) {
				
				const coordinate& tmp_c = this->line.getPointCoordinate(i);

				if (!this->measured_coordinates.contains(tmp_c)) {
					this->suggest_target.push_back(tmp_c);
				}
			}
		}

		// 実測済ゾーン内の曲率データを抽出 (std::sort を使うために曲率が第一要素)
		std::vector<double> original_sample_curvatures = this->d_spline->getSampleCurvatures();
		std::vector<std::pair<double, std::size_t>> curvatures;
		for (std::size_t i : this->d_spline->getMeasuredZoneSampleIndexes()) {
			curvatures.emplace_back(original_sample_curvatures[i], i);
		}

		// 曲率で降順ソート
		std::sort(curvatures.begin(), curvatures.end(), std::greater<std::pair<double, std::size_t>>());

		// 格納
		for (const std::pair<double, std::size_t>& c : curvatures) {

			const coordinate& tmp_c = this->line.getPointCoordinate(c.second);

			if (!this->measured_coordinates.contains(tmp_c)) {
				this->suggest_target.push_back(tmp_c);
			}
		}
	}

	OneDimDspSearcher::OneDimDspSearcher(std::shared_ptr<const DatabaseCore> search_database, const coordinate& example_coordinate, const DirectionLine& direction_info, bool low_value_is_better, double alpha) :
		line(search_database->getSpaceSize(), example_coordinate, direction_info)
	{
		this->d_spline = std::make_shared<ObservedDSpline>(std::make_shared<EquallySpacedDSpline>(this->line.getPointsNum(), alpha), DSplineObservationMode::MEASURED_ZONE);
		this->lower_is_better = low_value_is_better;
		this->best_judged_point_coordinate = example_coordinate;
		if (this->lower_is_better) {
			this->best_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_point_value = std::numeric_limits<double>::lowest();
		}

		// データベースから初期データ登録
		std::vector<std::pair<std::size_t, double>> initial_data;
		for (const coordinate& c : this->line.getPoints()) {
			if (search_database->hasSample(c)) {

				this->measured_coordinates.insert(c);
				double v = search_database->getSampleMetricValue(c);

				initial_data.emplace_back(this->line.getPointIndex(c), v);

				if (this->lower_is_better) {
					if (this->best_point_value > v) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
				else {
					if (this->best_point_value < v) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
			}
		}
		this->d_spline->update(initial_data);

		if (initial_data.empty()) {
			this->suggest_target = this->line.getPoints();
		}
		else {
			this->suggestBasedOnApproximation();
		}
	}

	OneDimDspSearcher::OneDimDspSearcher(std::shared_ptr<const DatabaseCore> search_database, const coordinate& coordinate1, const coordinate& coordinate2, bool low_value_is_better, double alpha):
		line(search_database->getSpaceSize(), coordinate1, coordinate2)
	{
		this->d_spline = std::make_shared<ObservedDSpline>(std::make_shared<EquallySpacedDSpline>(this->line.getPointsNum(), alpha), DSplineObservationMode::MEASURED_ZONE);
		this->lower_is_better = low_value_is_better;
		this->best_judged_point_coordinate = coordinate1;
		if (this->lower_is_better) {
			this->best_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_point_value = std::numeric_limits<double>::lowest();
		}

		// データベースから初期データ登録
		std::vector<std::pair<std::size_t, double>> initial_data;
		for (const coordinate& c : this->line.getPoints()) {
			if (search_database->hasSample(c)) {

				this->measured_coordinates.insert(c);
				double v = search_database->getSampleMetricValue(c);

				initial_data.emplace_back(this->line.getPointIndex(c), v);

				if (this->lower_is_better) {
					if (this->best_point_value > v) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
				else {
					if (this->best_point_value < v) {
						this->best_judged_point_coordinate = c;
						this->best_measured_point_coordinate = c;
						this->best_point_value = v;
					}
				}
			}
		}
		this->d_spline->update(initial_data);

		if (initial_data.empty()) {
			this->suggest_target = this->line.getPoints();
		}
		else {
			this->suggestBasedOnApproximation();
		}
	}

	OneDimDspSearcher::OneDimDspSearcher(std::shared_ptr<const DatabaseCore> search_database, const CoordinateLine& line_info, bool low_value_is_better, double alpha) :
		line(line_info)
	{
		this->d_spline = std::make_shared<ObservedDSpline>(std::make_shared<EquallySpacedDSpline>(this->line.getPointsNum(), alpha), DSplineObservationMode::MEASURED_ZONE);
		this->lower_is_better = low_value_is_better;
		this->best_judged_point_coordinate = this->line.getPointCoordinate(0);
		if (this->lower_is_better) {
			this->best_point_value = std::numeric_limits<double>::max();
		}
		else {
			this->best_point_value = std::numeric_limits<double>::lowest();
		}

		// データベースから初期データ登録
		std::vector<std::pair<std::size_t, double>> initial_data;
		for (const coordinate& c : this->line.getPoints()) {
			if (search_database->hasSample(c)) {

				this->measured_coordinates.insert(c);
				double v = search_database->getSampleMetricValue(c);

				initial_data.emplace_back(this->line.getPointIndex(c), v);

				if (this->lower_is_better) {
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
		}
		this->d_spline->update(initial_data);

		if (initial_data.empty()) {
			this->suggest_target = this->line.getPoints();
		}
		else {
			this->suggestBasedOnApproximation();
		}
	}

	OneDimDspSearcher::OneDimDspSearcher(const OneDimDspSearcher& original) :
		line(original.line),
		d_spline(original.d_spline)
	{
		this->suggest_target = original.suggest_target;
		this->lower_is_better = original.lower_is_better;
		this->measured_coordinate_buffer = original.measured_coordinate_buffer;
		this->measured_metric_value_buffer = original.measured_metric_value_buffer;
		this->best_judged_point_coordinate = original.best_judged_point_coordinate;
		this->best_measured_point_coordinate = original.best_measured_point_coordinate;
		this->best_point_value = original.best_point_value;
		this->measured_coordinates = original.measured_coordinates;
	}

	OneDimDspSearcher::~OneDimDspSearcher(void) {
		// DO_NOTHING
	}

	const CoordinateLine& OneDimDspSearcher::getLineInfomation(void) const {
		return this->line;
	}

	std::shared_ptr<const ObservedDSpline> OneDimDspSearcher::getUsedDSpline(void) const {
		return std::shared_ptr<ObservedDSpline>(this->d_spline);
	}

	const coordinate& OneDimDspSearcher::getSuggested(void) const {
		if (this->suggest_target.empty()) {
			return this->best_judged_point_coordinate;
		}
		else {
			return this->suggest_target[0];
		}
	}

	const coordinate_list& OneDimDspSearcher::getSuggestedList(void) const {
		return this->suggest_target;
	}

	void OneDimDspSearcher::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->measured_coordinate_buffer.push_back(measured_coordinate);
		this->measured_metric_value_buffer.push_back(metric_value);
	}

	bool OneDimDspSearcher::updateState(void) {

		if (this->measured_coordinate_buffer.empty()) {
			return false;
		}

		std::vector<std::pair<std::size_t, double>> delivered_data;
		for (std::size_t i = 0; i < this->measured_coordinate_buffer.size(); i++) {

			const coordinate& measured_coordinate = this->measured_coordinate_buffer[i];
			double metric_value = this->measured_metric_value_buffer[i];

			this->measured_coordinates.insert(measured_coordinate);

			delivered_data.emplace_back(this->line.getPointIndex(measured_coordinate), metric_value);

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
		}

		this->d_spline->update(delivered_data);
		this->suggestBasedOnApproximation();

		this->measured_coordinate_buffer.clear();
		this->measured_metric_value_buffer.clear();

		return true;
	}

	bool OneDimDspSearcher::isSearchFinished(void) const {
		
		// 最良推定点の連続回数 3
		if (this->lower_is_better) {
			if (this->d_spline->getConsecutiveLowestCount() >= 3) {
				return true;
			}
		}
		else {
			if (this->d_spline->getConsecutiveHighestCount() >= 3) {
				return true;
			}
		}

		// 直線上の探索済点の数 30
		if (this->measured_coordinates.size() >= 30) {
			return true;
		}

		// 全点探索済
		if (this->measured_coordinates.size() >= this->d_spline->getSamplesNum()) {
			return true;
		}

		return false;
	}

	const coordinate_list& OneDimDspSearcher::getTargetCoordinate(void) const {
		return this->line.getPoints();
	}

	const std::unordered_set<coordinate>& OneDimDspSearcher::getMeasuredCoordinates(void) const {
		return this->measured_coordinates;
	}

	const coordinate& OneDimDspSearcher::getBestJudgedCoordinate(void) const {
		return this->best_judged_point_coordinate;
	}

	const coordinate& OneDimDspSearcher::getBestMeasuredCoordinate(void) const {
		return this->best_measured_point_coordinate;
	}
}