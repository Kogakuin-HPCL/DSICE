#include <stdexcept>
#include <memory>
#include <unordered_map>

#include "d_spline.hpp"
#include "equally_spaced_d_spline.hpp"
#include "d_spline_c_wrapper.hpp"

namespace dsice {

	std::unordered_map<d_spline_id, std::shared_ptr<DSpline>>& getDSplinesMap(void) {

		static std::unordered_map<d_spline_id, std::shared_ptr<DSpline>> d_splines_map;

		return d_splines_map;
	}

	std::shared_ptr<DSpline> getDSpline(d_spline_id id) {

		std::unordered_map<d_spline_id, std::shared_ptr<DSpline>>& d_splines_map = getDSplinesMap();

		if (!d_splines_map.contains(id)) {
			throw std::invalid_argument("Specified d-Spline is not found.\n");
		}

		return d_splines_map.at(id);
	}

	std::unordered_map<d_spline_id, std::vector<double>>& getDSplineSampleValuesBuffers(void) {

		static std::unordered_map<d_spline_id, std::vector<double>> buffers;

		return buffers;
	}

	d_spline_id createNewDSplineId(void) {

		static d_spline_id d_spline_count = 0;

		d_spline_count++;
		
		return d_spline_count;
	}

	d_spline_id DSPLINE_EQ_CREATE(std::size_t sample_num, double alpha, std::size_t interpolated_point_num) {

		std::unordered_map<std::size_t, std::shared_ptr<DSpline>>& d_splines_map = getDSplinesMap();
		d_spline_id id = createNewDSplineId();

		d_splines_map.emplace(id, std::make_shared<EquallySpacedDSpline>(sample_num, alpha, interpolated_point_num));

		return id;
	}

	d_spline_id DSPLINE_EQ_CREATE_STANDARD(std::size_t sample_num) {

		std::unordered_map<std::size_t, std::shared_ptr<DSpline>>& d_splines_map = getDSplinesMap();
		d_spline_id id = createNewDSplineId();

		d_splines_map.emplace(id, std::make_shared<EquallySpacedDSpline>(sample_num));

		return id;
	}

	void DSPLINE_DELETE(d_spline_id id) {

		std::unordered_map<std::size_t, std::shared_ptr<DSpline>>& d_splines_map = getDSplinesMap();
		std::unordered_map<d_spline_id, std::vector<double>>& sample_values_buffers = getDSplineSampleValuesBuffers();

		if (d_splines_map.contains(id)) {
			d_splines_map.erase(id);
		}

		if (sample_values_buffers.contains(id)) {
			sample_values_buffers.erase(id);
		}
	}

	void DSPLINE_UPDATE(d_spline_id id, size_t sample_idx, double sample_value) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		d_spline->update(sample_idx, sample_value);
	}

	void DSPLINE_UPDATE_MANY(d_spline_id id, size_t sample_num, size_t* sample_idx_list, double* sample_value_list) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		std::vector<std::pair<std::size_t, double>> data;
		for (std::size_t i = 0; i < sample_num; i++) {
			data.emplace_back(sample_idx_list[i], sample_value_list[i]);
		}

		d_spline->update(data);
	}

	double DSPLINE_GET_MARKER_VALUE(d_spline_id id, size_t marker_idx) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		return d_spline->getMarkerValue(marker_idx);
	}

	double DSPLINE_GET_SAMPLE_VALUE(d_spline_id id, size_t sample_idx) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		return d_spline->getSampleValue(sample_idx);
	}

	const double* DSPLINE_GET_MARKER_VALUES(d_spline_id id) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		return d_spline->getMarkerValues().data();
	}

	const double* DSPLINE_GET_SAMPLE_VALUES(d_spline_id id) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		std::unordered_map<d_spline_id, std::vector<double>>& sample_values_buffers = getDSplineSampleValuesBuffers();
		if (sample_values_buffers.contains(id)) {
			sample_values_buffers.at(id) = d_spline->getSampleValues();
		}
		else {
			sample_values_buffers.emplace(id, d_spline->getSampleValues());
		}

		return sample_values_buffers.at(id).data();
	}

	size_t DSPLINE_MARKER_IDX_2_SAMPLE_IDX(d_spline_id id, size_t marker_idx) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		return d_spline->markerIdx2SampleIdx(marker_idx);
	}

	size_t DSPLINE_SAMPLE_IDX_2_MARKER_IDX(d_spline_id id, size_t sample_idx) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		return d_spline->sampleIdx2MarkerIdx(sample_idx);
	}

	size_t DSPLINE_GET_MARKERS_NUM(d_spline_id id) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		return d_spline->getMarkersNum();
	}

	size_t DSPLINE_GET_SAMPLES_NUM(d_spline_id id) {

		std::shared_ptr<DSpline> d_spline = getDSpline(id);

		return d_spline->getSamplesNum();
	}
}