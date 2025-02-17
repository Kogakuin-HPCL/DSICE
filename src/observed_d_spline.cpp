#include <memory>
#include <vector>
#include <algorithm>
#include <limits>

#include "d_spline.hpp"
#include "observed_d_spline.hpp"

namespace dsice {

	void ObservedDSpline::updateMeasuredZone(void) {

		this->lower_zone_markers.clear();
		this->measured_zone_markers.clear();
		this->higher_zone_markers.clear();
		this->lower_zone_samples.clear();
		this->measured_zone_samples.clear();
		this->higher_zone_samples.clear();

		//
		// マーカ位置
		// 
		
		// インデックスと値の組を配列化
		// std::sort() を使うために第 1 要素を値にする
		std::vector<std::pair<double, std::size_t>> marker_curve_values;
		std::vector<double> marker_values = this->d_spline->getMarkerValues();
		for (std::size_t i = 0; i < this->d_spline->getMarkersNum(); i++) {
			marker_curve_values.emplace_back(marker_values[i], i);
		}

		// 昇順ソート
		std::sort(marker_curve_values.begin(), marker_curve_values.end());

		std::size_t m_idx = 0;
		for (; (m_idx < marker_curve_values.size()) && (marker_curve_values[m_idx].first < this->measured_min); m_idx++) {
			this->lower_zone_markers.push_back(marker_curve_values[m_idx].second);
		}
		for (; (m_idx < marker_curve_values.size()) && (marker_curve_values[m_idx].first <= this->measured_max); m_idx++) {
			this->measured_zone_markers.push_back(marker_curve_values[m_idx].second);
		}
		for (std::size_t i = 0; i < (marker_curve_values.size() - m_idx); i++) {
			this->higher_zone_markers.push_back(marker_curve_values[marker_curve_values.size() - i - 1].second);
		}

		//
		// 標本位置
		//
		
		// インデックスと値の組を配列化
		// std::sort() を使うために第 1 要素を値にする
		std::vector<std::pair<double, std::size_t>> sample_curve_values;
		std::vector<double> sample_values = this->d_spline->getSampleValues();
		for (std::size_t i = 0; i < this->d_spline->getSamplesNum(); i++) {
			sample_curve_values.emplace_back(sample_values[i], i);
		}

		// 昇順ソート
		std::sort(sample_curve_values.begin(), sample_curve_values.end());

		std::size_t s_idx = 0;
		for (; (s_idx < sample_curve_values.size()) && (sample_curve_values[s_idx].first < this->measured_min); s_idx++) {
			this->lower_zone_samples.push_back(sample_curve_values[s_idx].second);
		}
		for (; (s_idx < sample_curve_values.size()) && (sample_curve_values[s_idx].first <= this->measured_max); s_idx++) {
			this->measured_zone_samples.push_back(sample_curve_values[s_idx].second);
		}
		for (std::size_t i = 0; i < (sample_curve_values.size() - s_idx); i++) {
			this->higher_zone_samples.push_back(sample_curve_values[sample_curve_values.size() - i - 1].second);
		}
	}

	void ObservedDSpline::updateCurvatures(void) {
		
		this->all_curvatures = std::vector<double>(this->d_spline->getMarkersNum(), 0);

		std::vector<double> curve_values = this->d_spline->getMarkerValues();
		
		for (std::size_t i = 1; i < (this->d_spline->getMarkersNum() - 1); i++) {
			this->all_curvatures[i] = std::abs(curve_values[i - 1] - 2 * curve_values[i] + curve_values[i + 1]);
		}

		this->sample_curvatures.clear();
		for (std::size_t i = 0; i < this->d_spline->getSamplesNum(); i++) {
			this->sample_curvatures.push_back(this->all_curvatures[this->d_spline->sampleIdx2MarkerIdx(i)]);
		}
	}

	void ObservedDSpline::checkBestEstimated(std::size_t used_samples_num) {

		std::vector<double> marker_values = this->d_spline->getMarkerValues();
		std::size_t tmp_lowest_idx = 0;
		double tmp_lowest_value = marker_values[0];
		std::size_t tmp_highest_idx = 0;
		double tmp_highest_value = marker_values[0];
		for (std::size_t i = 1; i < this->d_spline->getMarkersNum(); i++) {

			double v = marker_values[i];

			if (v < tmp_lowest_value) {
				tmp_lowest_idx = this->d_spline->markerIdx2SampleIdx(i);
				tmp_lowest_value = v;
			}

			if (v > tmp_highest_value) {
				tmp_highest_idx = this->d_spline->markerIdx2SampleIdx(i);
				tmp_highest_value = v;
			}
		}

		if (tmp_lowest_idx == this->lowest_estimated_index) {
			this->consecutive_lowest_count += used_samples_num;
		}
		else {
			this->lowest_estimated_index = tmp_lowest_idx;
			this->consecutive_lowest_count = 1;
		}

		if (tmp_highest_idx == this->highest_estimated_index) {
			this->consecutive_highest_count += used_samples_num;
		}
		else {
			this->highest_estimated_index = tmp_highest_idx;
			this->consecutive_highest_count = 1;
		}
	}

	ObservedDSpline::ObservedDSpline(std::shared_ptr<DSpline> target_d_spline, DSplineObservationMode observation_mode) :
		mode(observation_mode)
	{
		this->d_spline = target_d_spline;

		// 初期状態は全体が未実測ゾーン
		for (std::size_t i = 0; i < this->d_spline->getMarkersNum(); i++) {
			this->lower_zone_markers.push_back(i);
			this->higher_zone_markers.push_back(i);
		}
		for (std::size_t i = 0; i < this->d_spline->getSamplesNum(); i++) {
			this->lower_zone_samples.push_back(i);
			this->higher_zone_samples.push_back(i);
		}
	}

	ObservedDSpline::ObservedDSpline(const ObservedDSpline& original):
		mode(original.mode)
	{
		this->d_spline = original.d_spline;
		this->all_curvatures = original.all_curvatures;
		this->sample_curvatures = original.sample_curvatures;
		this->measured_max = original.measured_max;
		this->measured_min = original.measured_max;
		this->lowest_estimated_index = original.lowest_estimated_index;
		this->highest_estimated_index = original.highest_estimated_index;
		this->consecutive_highest_count = original.consecutive_highest_count;
		this->consecutive_lowest_count = original.consecutive_lowest_count;
		this->higher_zone_markers = original.higher_zone_markers;
		this->measured_zone_markers = original.measured_zone_markers;
		this->lower_zone_markers = original.lower_zone_markers;
		this->higher_zone_samples = original.higher_zone_samples;
		this->measured_zone_samples = original.measured_zone_samples;
		this->lower_zone_samples = original.lower_zone_samples;
	}

	ObservedDSpline::~ObservedDSpline(void) {
		// DO_NOTHING
	}

	const std::vector<double>& ObservedDSpline::getAllCurvatures(void) const {
		return this->all_curvatures;
	}

	const std::vector<double>& ObservedDSpline::getSampleCurvatures(void) const {
		return this->sample_curvatures;
	}

	std::size_t ObservedDSpline::getConsecutiveLowestCount(void) const {
		return this->consecutive_lowest_count;
	}

	std::size_t ObservedDSpline::getConsecutiveHighestCount(void) const {
		return this->consecutive_highest_count;
	}

	double ObservedDSpline::getMaxMeasuredValue(void) const {
		return this->measured_max;
	}

	double ObservedDSpline::getMinMeasuredValue(void) const {
		return this->measured_min;
	}

	const std::vector<std::size_t>& ObservedDSpline::getHigherZoneMarkerIndexes(void) const {
		return this->higher_zone_markers;
	}

	const std::vector<std::size_t>& ObservedDSpline::getMeasuredZoneMarkerIndexes(void) const {
		return this->measured_zone_markers;
	}

	const std::vector<std::size_t>& ObservedDSpline::getLowerZoneMarkerIndexes(void) const {
		return this->lower_zone_markers;
	}

	const std::vector<std::size_t>& ObservedDSpline::getHigherZoneSampleIndexes(void) const {
		return this->higher_zone_samples;
	}

	const std::vector<std::size_t>& ObservedDSpline::getMeasuredZoneSampleIndexes(void) const {
		return this->measured_zone_samples;
	}

	const std::vector<std::size_t>& ObservedDSpline::getLowerZoneSampleIndexes(void) const {
		return this->lower_zone_samples;
	}

	void ObservedDSpline::update(std::size_t sample_idx, double sample_value) {

		this->d_spline->update(sample_idx, sample_value);
		this->updateCurvatures();
		this->checkBestEstimated();

		if (this->measured_max < sample_value) {
			this->measured_max = sample_value;
		}

		if (measured_min > sample_value) {
			this->measured_min = sample_value;
		}

		if (this->mode == DSplineObservationMode::MEASURED_ZONE) {
			this->updateMeasuredZone();
		}
	}

	void ObservedDSpline::update(const std::pair<std::size_t, double>& sample_data) {

		this->d_spline->update(sample_data);
		this->updateCurvatures();
		this->checkBestEstimated();

		double sample_value = sample_data.second;

		if (this->measured_max < sample_value) {
			this->measured_max = sample_value;
		}

		if (measured_min > sample_value) {
			this->measured_min = sample_value;
		}

		if (this->mode == DSplineObservationMode::MEASURED_ZONE) {
			this->updateMeasuredZone();
		}
	}

	void ObservedDSpline::update(const std::vector<std::pair<std::size_t, double>>& sample_data) {

		this->d_spline->update(sample_data);
		this->updateCurvatures();
		this->checkBestEstimated(sample_data.size());

		for (const std::pair<std::size_t, double>& one_data : sample_data) {

			double sample_value = one_data.second;

			if (this->measured_max < sample_value) {
				this->measured_max = sample_value;
			}

			if (measured_min > sample_value) {
				this->measured_min = sample_value;
			}
		}

		if (this->mode == DSplineObservationMode::MEASURED_ZONE) {
			this->updateMeasuredZone();
		}
	}

	double ObservedDSpline::getMarkerValue(std::size_t marker_idx) const {
		return this->d_spline->getMarkerValue(marker_idx);
	}

	double ObservedDSpline::getSampleValue(std::size_t sample_idx) const {
		return this->d_spline->getSampleValue(sample_idx);
	}

	const std::vector<double>& ObservedDSpline::getMarkerValues(void) const {
		return this->d_spline->getMarkerValues();
	}

	std::vector<double> ObservedDSpline::getSampleValues(void) const {
		return this->d_spline->getSampleValues();
	}

	std::size_t ObservedDSpline::markerIdx2SampleIdx(std::size_t marker_idx) const {
		return this->d_spline->markerIdx2SampleIdx(marker_idx);
	}

	std::size_t ObservedDSpline::sampleIdx2MarkerIdx(std::size_t sample_idx) const {
		return this->d_spline->sampleIdx2MarkerIdx(sample_idx);
	}

	std::size_t ObservedDSpline::getMarkersNum(void) const {
		return this->d_spline->getMarkersNum();
	}

	std::size_t ObservedDSpline::getSamplesNum(void) const {
		return this->d_spline->getSamplesNum();
	}

	const std::vector<double>& ObservedDSpline::getZ(void) const {
		return this->d_spline->getZ();
	}

	const std::vector<double>& ObservedDSpline::getB(void) const {
		return this->d_spline->getB();
	}

	const std::vector<double>& ObservedDSpline::getF(void) const {
		return this->d_spline->getF();
	}
}