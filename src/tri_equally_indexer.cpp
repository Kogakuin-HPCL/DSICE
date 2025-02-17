#include <vector>
#include <queue>
#include <stdexcept>

#include "tri_equally_indexer.hpp"

namespace dsice {

	std::pair<std::size_t, std::size_t> TriEquallyIndexer::getTriPoints(std::size_t left, std::size_t right) {

		if (left == right) {
			return std::pair<std::size_t, std::size_t>(left, right);
		}

		if (right < left) {
			std::size_t tmp = left;
			left = right;
			right = tmp;
		}

		std::size_t segment_length = right - left;

		if (segment_length == 2) {
			return std::pair<std::size_t, std::size_t>(left + 1, right);
		}

		std::size_t mid1 = left + (segment_length / 3);
		std::size_t mid2 = mid1 + ((right - mid1) / 2);

		return std::pair<std::size_t, std::size_t>(mid1, mid2);
	}

	TriEquallyIndexer::TriEquallyIndexer(std::size_t left, std::size_t right) {

		if (left == right) {
			this->indexes.push_back(left);
			return;
		}

		if (right < left) {
			std::size_t tmp = left;
			left = right;
			right = tmp;
		}

		this->indexes.push_back(left);
		this->indexes.push_back(right);

		std::queue<std::pair<std::size_t, std::size_t>> segments;
		segments.emplace(left, right);

		while (!segments.empty()) {
			
			std::pair<std::size_t, std::size_t> target_segment = segments.front();
			segments.pop();

			std::size_t seg_left = target_segment.first;
			std::size_t seg_right = target_segment.second;

			std::size_t segment_length = seg_right - seg_left;

			if (segment_length == 1) {
				continue;
			}
			else if (segment_length == 2) {
				this->indexes.push_back(left + 1);
				continue;
			}

			std::size_t mid1 = seg_left + (segment_length / 3);
			std::size_t mid2 = mid1 + ((seg_right - mid1) / 2);

			this->indexes.push_back(mid1);
			this->indexes.push_back(mid2);

			segments.emplace(seg_left, mid1);
			segments.emplace(mid1, mid2);
			segments.emplace(mid2, seg_right);
		}
	}

	TriEquallyIndexer::TriEquallyIndexer(const TriEquallyIndexer& original) {
		this->indexes = original.indexes;
	}

	TriEquallyIndexer::~TriEquallyIndexer(void) {
		// DO_NOTHING
	}

	std::size_t TriEquallyIndexer::getNum(void) const {
		return this->indexes.size();
	}

	const std::vector<std::size_t>& TriEquallyIndexer::getIndexes(void) const {
		return this->indexes;
	}
}
