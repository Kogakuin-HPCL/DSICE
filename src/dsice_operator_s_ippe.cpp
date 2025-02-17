#include <stdexcept>
#include <memory>
#include <string>

#include "dsice_search_space.hpp"
#include "dsice_database_metric_type.hpp"
#include "dsice_standard_database.hpp"
#include "dsice_logging_database.hpp"
#include "uni_measurer.hpp"
#include "line_single_tri_searcher.hpp"
#include "one_dim_dsp_searcher.hpp"
#include "dsice_operator_s_ippe.hpp"

namespace dsice {

	Operator_S_IPPE::Operator_S_IPPE(const space_size& parameters, bool low_value_is_better, bool logging_on, double alpha, DatabaseMetricType metric_type) {

		if (parameters.empty()) {
			throw std::invalid_argument("Need one parameter at least.");
		}

		if (parameters.size() > 1) {
			throw std::invalid_argument("Cannot search in two or more parameters.");
		}

		if (parameters[0] < 1) {
			throw std::invalid_argument("Each parameter needs values to be measured.");
		}

		this->lower_is_better = low_value_is_better;
		this->is_logging_mode_on = logging_on;
		this->dsp_alpha = alpha;

		if (this->is_logging_mode_on) {
			this->database = std::make_shared<LoggingDatabase>(parameters, metric_type);
		}
		else {
			this->database = std::make_shared<StandardDatabase>(parameters, metric_type);
		}

		this->base_coordinate = { 0 };
		this->database->setBasePoint(this->base_coordinate);

		// 対象が 1 点の場合は探索しようがないので即終了
		if (parameters[0] == 1) {

			this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
			this->searching_phase = 2;
			
			if (this->is_logging_mode_on) {
				std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
				ld->updateCandidateList(this->searcher->getSuggestedList());
			}

			return;
		}

		CoordinateLine line(parameters, this->base_coordinate, { 1 });
		this->searcher = std::make_shared<LineSingleTriSearcher>(this->database, line, this->lower_is_better);

		this->searching_phase = 0;

		if (this->is_logging_mode_on) {
			std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
			ld->updateCandidateList(this->searcher->getSuggestedList());
		}
	}

	Operator_S_IPPE::Operator_S_IPPE(const Operator_S_IPPE& original) {

		this->searching_phase = original.searching_phase;
		this->lower_is_better = original.lower_is_better;
		this->is_logging_mode_on = original.is_logging_mode_on;
		this->base_coordinate = original.base_coordinate;
		this->loop_count = original.loop_count;
		this->dsp_alpha = original.dsp_alpha;

		if (std::shared_ptr<StandardDatabase> sd = std::dynamic_pointer_cast<StandardDatabase>(original.database)) {
			this->database = std::make_shared<StandardDatabase>(*sd);
		}
		else {
			this->database = std::make_shared<LoggingDatabase>(*(std::dynamic_pointer_cast<LoggingDatabase>(original.database)));
		}

		switch (this->searching_phase) {
		case 0:
			this->searcher = std::make_shared<LineSingleTriSearcher>(*(std::dynamic_pointer_cast<LineSingleTriSearcher>(original.searcher)));
			break;
		case 1:
			this->searcher = std::make_shared<OneDimDspSearcher>(*(std::dynamic_pointer_cast<OneDimDspSearcher>(original.searcher)));
			break;
		default:
			this->searcher = std::make_shared<UniMeasurer>(*(std::dynamic_pointer_cast<UniMeasurer>(original.searcher)));
			break;
		}
	}

	Operator_S_IPPE::~Operator_S_IPPE(void) {
		// DO_NOTHING
	}

	const coordinate& Operator_S_IPPE::getBaseCoordinate(void) const {
		return this->base_coordinate;
	}

	const coordinate& Operator_S_IPPE::getSuggested(void) {

		this->updateState();

		return this->searcher->getSuggested();
	}

	const coordinate_list& Operator_S_IPPE::getSuggestedList(void) {

		this->updateState();

		return this->searcher->getSuggestedList();
	}

	void Operator_S_IPPE::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->database->setSampleMetricValue(measured_coordinate, metric_value);
		this->searcher->setMetricValue(measured_coordinate, metric_value);
	}

	bool Operator_S_IPPE::isSearchFinished(void) {

		this->updateState();

		return this->searching_phase == 2;
	}

	const coordinate& Operator_S_IPPE::getBestJudgedCoordinate(void) {

		this->updateState();

		if (!this->database->hasSample(this->searcher->getBestJudgedCoordinate())) {
			return this->base_coordinate;
		}
		else if (!this->database->hasSample(this->base_coordinate)) {
			return this->searcher->getBestJudgedCoordinate();
		}

		double tmp_newest_best = this->database->getSampleMetricValue(this->searcher->getBestJudgedCoordinate());
		double base_value = this->database->getSampleMetricValue(this->base_coordinate);

		if (this->lower_is_better) {
			if (tmp_newest_best > base_value) {
				return this->base_coordinate;
			}
			else {
				return this->searcher->getBestJudgedCoordinate();
			}
		}
		else {
			if (tmp_newest_best < base_value) {
				return this->base_coordinate;
			}
			else {
				return this->searcher->getBestJudgedCoordinate();
			}
		}
	}

	const coordinate& Operator_S_IPPE::getBestMeasuredCoordinate(void) {

		this->updateState();

		if (!this->database->hasSample(this->searcher->getBestMeasuredCoordinate())) {
			return this->base_coordinate;
		}
		else if (!this->database->hasSample(this->base_coordinate)) {
			return this->searcher->getBestMeasuredCoordinate();
		}

		double tmp_newest_best = this->database->getSampleMetricValue(this->searcher->getBestMeasuredCoordinate());
		double base_value = this->database->getSampleMetricValue(this->base_coordinate);

		if (this->lower_is_better) {
			if (tmp_newest_best > base_value) {
				return this->base_coordinate;
			}
			else {
				return this->searcher->getBestMeasuredCoordinate();
			}
		}
		else {
			if (tmp_newest_best < base_value) {
				return this->base_coordinate;
			}
			else {
				return this->searcher->getBestMeasuredCoordinate();
			}
		}
	}

	void Operator_S_IPPE::updateState(void) {

		bool searcher_updated = this->searcher->updateState();

		if (!searcher_updated) {
			return;
		}

		this->loop_count++;
		if (this->is_logging_mode_on) {
			std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
			ld->updateCandidateList(this->searcher->getSuggestedList());
		}
		this->database->setLoopEnd();

		if (this->searcher->isSearchFinished()) {

			switch (this->searching_phase) {
			case 0:

				this->searcher = std::make_shared<OneDimDspSearcher>(this->database, CoordinateLine(this->database->getSpaceSize(), this->base_coordinate, { 1 }), this->lower_is_better, this->dsp_alpha);
				this->searching_phase = 1;
				if (this->is_logging_mode_on) {
					std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
					ld->updateCandidateList(this->searcher->getSuggestedList());
				}

				// 初期近似で終了条件を満たした場合
				if (this->searcher->isSearchFinished()) {

					this->base_coordinate = this->searcher->getBestJudgedCoordinate();

					this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
					this->searching_phase = 2;
					if (this->is_logging_mode_on) {
						std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
						ld->updateCandidateList(this->searcher->getSuggestedList());
					}
				}

				break;
			case 1:

				this->base_coordinate = this->searcher->getBestJudgedCoordinate();

				this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
				this->searching_phase = 2;
				if (this->is_logging_mode_on) {
					std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
					ld->updateCandidateList(this->searcher->getSuggestedList());
				}

				break;
			default:
				// DO_NOTHING
				break;
			}
		}
	}

	std::shared_ptr<DatabaseCore> Operator_S_IPPE::getDatabase(void) const {
		return this->database;
	}

	std::string Operator_S_IPPE::getAlgorithmId(void) const {
		return "S_IPPE";
	}

	std::size_t Operator_S_IPPE::getLoopCount(void) const {
		return this->loop_count;
	}

	std::string Operator_S_IPPE::getSearchModeName(void) const {
		switch (this->searching_phase) {
		case 0:
			return "Preparing";
		case 1:
			return "Estimation";
		default:
			return "Finished";
		}
	}

	std::shared_ptr<const ObservedDSpline> Operator_S_IPPE::getUsedDSpline(void) const {
		if (std::shared_ptr<OneDimDspSearcher> s = std::dynamic_pointer_cast<OneDimDspSearcher>(this->searcher)) {
			return s->getUsedDSpline();
		}
		else {
			return nullptr;
		}
	}
}
