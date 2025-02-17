#include "cstddef"
#include "overwritten_double.hpp"

namespace dsice {

	OverwrittenDouble::OverwrittenDouble(void) {
		// DO_NOTHING
	}

	OverwrittenDouble::OverwrittenDouble(double init_value) {
		this->value = init_value;
		this->count = 1;
	}

	OverwrittenDouble::OverwrittenDouble(const OverwrittenDouble& original) {
		this->value = original.value;
		this->count = original.count;
	}

	OverwrittenDouble::~OverwrittenDouble(void) {
		// DO_NOTHING
	}

	OverwrittenDouble::operator double() const {
		return this->value;
	}

	double OverwrittenDouble::getValue(void) const {
		return this->value;
	}

	void OverwrittenDouble::setValue(double metric_value) {
		this->count++;
		this->value = metric_value;
	}

	std::size_t OverwrittenDouble::getRecordedCount(void) const {
		return this->count;
	}

	std::shared_ptr<Metric> OverwrittenDouble::makeCopy(void) const {
		return std::make_shared<OverwrittenDouble>(*this);
	}
}