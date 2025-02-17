#include <memory>
#include <vector>

#include "dsice_search_space.hpp"
#include "coordinate_line.hpp"
#include "dsice_viewer_background.hpp"

namespace dsice {

	size_t DSICE_VIEWER_GET_LINE_POINTS_NUM(size_t parameter_num, const size_t* space, const size_t* coordinate1, const size_t* coordinate2) {

		static std::shared_ptr<CoordinateLine> line;

		space_size s;
		coordinate c1;
		coordinate c2;
		for (std::size_t i = 0; i < parameter_num; i++) {
			s.push_back(space[i]);
			c1.push_back(coordinate1[i]);
			c2.push_back(coordinate2[i]);
		}

		line = std::make_shared<CoordinateLine>(s, c1, c2);

		return line->getPointsNum();
	}

	const std::size_t** DSICE_VIEWER_GET_LINE_POINTS(std::size_t parameter_num, const std::size_t* space, const std::size_t* coordinate1, const std::size_t* coordinate2) {

		static std::shared_ptr<CoordinateLine> line;
		static std::vector <const std::size_t*> line_points;

		space_size s;
		coordinate c1;
		coordinate c2;
		for (std::size_t i = 0; i < parameter_num; i++) {
			s.push_back(space[i]);
			c1.push_back(coordinate1[i]);
			c2.push_back(coordinate2[i]);
		}

		line = std::make_shared<CoordinateLine>(s, c1, c2);
		line_points.clear();
		for (const coordinate& c : line->getPoints()) {
			line_points.push_back(c.data());
		}

		return line_points.data();
	}
}