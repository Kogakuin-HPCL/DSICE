#include <vector>

#include "equally_spaced_d_spline.hpp"

namespace dsice {

	void EquallySpacedDSpline::setupNearestSampleIdx(void) {

		// 両端の緩衝部は 2 点
		this->nearest_sample_idx.push_back(0);
		this->nearest_sample_idx.push_back(0);

		// 指定された数の点を空けつつ標本データ点を配置
		std::size_t interpolated_half_num = this->interpolated_num / 2;
		this->nearest_sample_idx.push_back(0);
		for (std::size_t i = 0; i < this->samples_num - 1; i++) {

			for (std::size_t j = 0; j < interpolated_half_num; j++) {
				this->nearest_sample_idx.push_back(i);
			}
			for (std::size_t j = interpolated_half_num; j < this->interpolated_num; j++) {
				this->nearest_sample_idx.push_back(i + 1);
			}

			this->nearest_sample_idx.push_back(i + 1);
		}

		// 両端の緩衝部は 2 点
		this->nearest_sample_idx.push_back(this->samples_num - 1);
		this->nearest_sample_idx.push_back(this->samples_num - 1);
	}

	EquallySpacedDSpline::EquallySpacedDSpline(std::size_t sample_value_num, double alpha, std::size_t interpolated_point_num) :
		samples_num(sample_value_num),
		markers_num(sample_value_num + 4 + interpolated_point_num * (sample_value_num - 1)),
		interpolated_num(interpolated_point_num),
		d_spline(this->markers_num, alpha)
	{
		this->setupNearestSampleIdx();
	}

	EquallySpacedDSpline::EquallySpacedDSpline(const EquallySpacedDSpline& original):
		samples_num(original.samples_num),
		markers_num(original.markers_num),
		interpolated_num(original.interpolated_num),
		d_spline(original.d_spline)
	{
		this->nearest_sample_idx = original.nearest_sample_idx;
	}

	EquallySpacedDSpline::~EquallySpacedDSpline(void) {
		// DO_NOTHING
	}

	void EquallySpacedDSpline::update(std::size_t sample_idx, double sample_value) {
		this->d_spline.update(this->sampleIdx2MarkerIdx(sample_idx), sample_value);
	}

	void EquallySpacedDSpline::update(const std::pair<std::size_t, double>& sample_data) {
		this->d_spline.update(this->sampleIdx2MarkerIdx(sample_data.first), sample_data.second);
	}

	void EquallySpacedDSpline::update(const std::vector<std::pair<std::size_t, double>>& sample_data) {
		
		std::vector<std::pair<std::size_t, double>> marker_data;
		for (const std::pair<std::size_t, double>& one_sample : sample_data) {
			marker_data.emplace_back(this->sampleIdx2MarkerIdx(one_sample.first), one_sample.second);
		}

		this->d_spline.update(marker_data);
	}

	double EquallySpacedDSpline::getMarkerValue(std::size_t marker_idx) const {
		return this->d_spline.getF()[marker_idx];
	}

	double EquallySpacedDSpline::getSampleValue(std::size_t sample_idx) const {
		return this->d_spline.getF()[this->sampleIdx2MarkerIdx(sample_idx)];
	}

	const std::vector<double>& EquallySpacedDSpline::getMarkerValues(void) const {
		return this->d_spline.getF();
	}

	std::vector<double> EquallySpacedDSpline::getSampleValues(void) const {

		std::vector<double> sample_values;
		for (std::size_t i = 0; i < this->samples_num; i++) {
			sample_values.push_back(this->d_spline.getF()[this->sampleIdx2MarkerIdx(i)]);
		}

		return sample_values;
	}

	std::size_t EquallySpacedDSpline::markerIdx2SampleIdx(std::size_t marker_idx) const {
		return this->nearest_sample_idx[marker_idx];
	}

	std::size_t EquallySpacedDSpline::sampleIdx2MarkerIdx(std::size_t sample_idx) const {
		
		// 条件分岐なしで記述可能なため，記憶配列を持たずに毎回計算する
		return 2 + sample_idx * (this->interpolated_num + 1);
	}

	std::size_t EquallySpacedDSpline::getMarkersNum(void) const {
		return this->markers_num;
	}

	std::size_t EquallySpacedDSpline::getSamplesNum(void) const {
		return this->samples_num;
	}

	const std::vector<double>& EquallySpacedDSpline::getZ(void) const {
		return this->d_spline.getZ();
	}

	const std::vector<double>& EquallySpacedDSpline::getB(void) const {
		return this->d_spline.getB();
	}

	const std::vector<double>& EquallySpacedDSpline::getF(void) const {
		return this->d_spline.getF();
	}
}
