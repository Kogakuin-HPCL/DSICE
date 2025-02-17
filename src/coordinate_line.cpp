#include <vector>

#include "dsice_search_space.hpp"
#include "direction_line.hpp"
#include "coordinate_line.hpp"

namespace dsice {

	CoordinateLine::CoordinateLine(const space_size& space_info, const coordinate& example_coordinate, const DirectionLine& line_info) :
		direction(line_info)
	{
		// その地点を示す方向の場合は指定点のみ格納して終了
		if (!this->direction.hasDimension()) {

			this->line_coordinates.push_back(example_coordinate);
			this->point_indexes.emplace(example_coordinate, 0);

			return;
		}

		const std::vector<long long> forward_direction = this->direction.getDirection();
		const std::vector<long long> reverse_direction = this->direction.getReverse();
		std::size_t dimension = forward_direction.size();

		// 指定点から「逆方向」にたどる
		coordinate_list tmp_reverse_coordinates;
		std::size_t reverse_distance = 1;
		while (true) {

			coordinate tmp_coordinate;
			bool in_space = true;

			for (std::size_t i = 0; i < dimension; i++) {

				long long diff = reverse_direction[i] * reverse_distance;

				long long point = static_cast<long long>(example_coordinate[i]) + diff;

				if (point < 0 || static_cast<long long>(space_info[i]) <= point) {
					in_space = false;
					break;
				}

				tmp_coordinate.push_back(point);
			}

			if (!in_space) {
				break;
			}

			tmp_reverse_coordinates.push_back(tmp_coordinate);
			reverse_distance++;
		}

		// 端から指定点直前までを格納
		for (auto rc = tmp_reverse_coordinates.rbegin(); rc != tmp_reverse_coordinates.rend(); rc++) {
			this->line_coordinates.push_back(*rc);
			this->point_indexes.emplace(*rc, this->point_indexes.size());
		}

		// 指定点から「順方向」にたどる
		std::size_t forward_distance = 0;
		while (true) {

			coordinate tmp_coordinate;
			bool in_space = true;

			for (std::size_t i = 0; i < dimension; i++) {

				long long diff = forward_direction[i] * forward_distance;
				long long point = static_cast<long long>(example_coordinate[i]) + diff;

				if (point < 0 || static_cast<long long>(space_info[i]) <= point) {
					in_space = false;
					break;
				}

				tmp_coordinate.push_back(point);
			}

			if (!in_space) {
				break;
			}

			this->line_coordinates.push_back(tmp_coordinate);
			this->point_indexes.emplace(tmp_coordinate, this->point_indexes.size());
			forward_distance++;
		}
	}

	CoordinateLine::CoordinateLine(const space_size& space_info, const coordinate& coordinate1, const coordinate& coordinate2) :
		direction(coordinate1, coordinate2)
	{
		// その地点を示す方向の場合は指定点のみ格納して終了
		if (!this->direction.hasDimension()) {

			this->line_coordinates.push_back(coordinate1);
			this->point_indexes.emplace(coordinate1, 0);

			return;
		}

		const std::vector<long long> forward_direction = this->direction.getDirection();
		const std::vector<long long> reverse_direction = this->direction.getReverse();
		std::size_t dimension = forward_direction.size();

		// 指定点から「逆方向」にたどる
		coordinate_list tmp_reverse_coordinates;
		std::size_t reverse_distance = 1;
		while (true) {

			coordinate tmp_coordinate;
			bool in_space = true;

			for (std::size_t i = 0; i < dimension; i++) {

				long long diff = reverse_direction[i] * reverse_distance;
				long long point = static_cast<long long>(coordinate1[i]) + diff;

				if (point < 0 || static_cast<long long>(space_info[i]) <= point) {
					in_space = false;
					break;
				}

				tmp_coordinate.push_back(point);
			}

			if (!in_space) {
				break;
			}

			tmp_reverse_coordinates.push_back(tmp_coordinate);
			reverse_distance++;
		}

		// 端から指定点直前までを格納
		for (auto rc = tmp_reverse_coordinates.rbegin(); rc != tmp_reverse_coordinates.rend(); rc++) {
			this->line_coordinates.push_back(*rc);
			this->point_indexes.emplace(*rc, this->point_indexes.size());
		}

		// 指定点から「順方向」にたどる
		std::size_t forward_distance = 0;
		while (true) {

			coordinate tmp_coordinate;
			bool in_space = true;

			for (std::size_t i = 0; i < dimension; i++) {

				long long diff = forward_direction[i] * forward_distance;
				long long point = static_cast<long long>(coordinate1[i]) + diff;

				if (point < 0 || static_cast<long long>(space_info[i]) <= point) {
					in_space = false;
					break;
				}

				tmp_coordinate.push_back(point);
			}

			if (!in_space) {
				break;
			}

			this->line_coordinates.push_back(tmp_coordinate);
			this->point_indexes.emplace(tmp_coordinate, this->point_indexes.size());
			forward_distance++;
		}
	}

	CoordinateLine::CoordinateLine(const CoordinateLine& original) :
		direction(original.direction)
	{
		this->line_coordinates = original.line_coordinates;
		this->point_indexes = original.point_indexes;
	}

	CoordinateLine::~CoordinateLine(void) {
		// DO_NOTHING
	}

	const DirectionLine& CoordinateLine::getLineDirection(void) const {
		return this->direction;
	}

	std::size_t CoordinateLine::getPointsNum(void) const {
		return this->line_coordinates.size();
	}

	const std::vector<coordinate>& CoordinateLine::getPoints(void) const {
		return this->line_coordinates;
	}

	const coordinate& CoordinateLine::getPointCoordinate(std::size_t point_index) const {
		return this->line_coordinates[point_index];
	}

	std::size_t CoordinateLine::getPointIndex(const coordinate& point_coordinate) const {
		return this->point_indexes.at(point_coordinate);
	}
}