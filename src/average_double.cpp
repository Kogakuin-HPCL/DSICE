#include <cstddef>
#include "average_double.hpp"

namespace dsice {

	AverageDouble::AverageDouble(void) {
		// DO_NOTHING
	}

	AverageDouble::AverageDouble(double init_value) {
		this->count = 1;
		this->value = init_value;
	}

	AverageDouble::AverageDouble(const AverageDouble& original) {
		this->value = original.value;
		this->count = original.count;
	}

	AverageDouble::~AverageDouble(void) {
		// DO_NOTHING
	}

	AverageDouble::operator double() const {
		return this->value;
	}

	double AverageDouble::getValue(void) const {
		return this->value;
	}

	void AverageDouble::setValue(double metric_value) {

		this->count++;

		double diff = metric_value - this->value;
		double effect = diff / this->count;

		this->value += effect;
	}

	std::size_t AverageDouble::getRecordedCount(void) const {
		return this->count;
	}

	std::shared_ptr<Metric> AverageDouble::makeCopy(void) const {
		return std::make_shared<AverageDouble>(*this);
	}
}