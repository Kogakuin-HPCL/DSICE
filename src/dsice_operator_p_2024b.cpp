#include <memory>
#include <limits>
#include <string>

#include "dsice_search_space.hpp"
#include "direction_line.hpp"
#include "dsice_database_metric_type.hpp"
#include "dsice_standard_database.hpp"
#include "dsice_logging_database.hpp"
#include "simple_lhd_searcher.hpp"
#include "radial_dsp_searcher.hpp"
#include "uni_measurer.hpp"
#include "dsice_operator_p_2024b.hpp"

namespace dsice {

	Operator_P_2024B::Operator_P_2024B(const space_size& parameters, bool low_value_is_better, bool logging_on, OperatorInitializeWay initialize_way, double alpha, DatabaseMetricType metric_type, coordinate initial_coordinate) {

		if (parameters.empty()) {
			throw std::invalid_argument("need one parameter at least.");
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

		if (this->lower_is_better) {
			this->base_value = std::numeric_limits<double>::max();
		}
		else {
			this->base_value = std::numeric_limits<double>::lowest();
		}

		switch (initialize_way) {
		case OperatorInitializeWay::SPECIFIED:
			this->base_coordinate = initial_coordinate;
			this->database->setBasePoint(this->base_coordinate);

			this->searcher = std::make_shared<RadialDspSearcher>(this->database, this->base_coordinate, this->database->getSpaceDimension(), this->lower_is_better, this->dsp_alpha);
			this->searching_phase = 1;
			if (this->is_logging_mode_on) {
				std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
				ld->updateCandidateList(this->searcher->getSuggestedList());
			}
			break;
		case OperatorInitializeWay::INITIAL_SEARCH:
			this->searcher = std::make_shared<SimpleLhdSearcher>(this->database, this->lower_is_better);

			this->base_coordinate = this->searcher->getSuggested();
			this->database->setBasePoint(this->base_coordinate);

			if (this->is_logging_mode_on) {
				std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
				ld->updateCandidateList(this->searcher->getSuggestedList());
			}
			break;
		default:
			// 固定で中心点を初期点とする
			coordinate center_point;
			for (std::size_t param_length : parameters) {
				if (param_length == 0) {
					throw std::invalid_argument("All parameters must have configurable values.");
				}
				center_point.push_back(param_length / 2);
			}

			this->base_coordinate = center_point;
			this->database->setBasePoint(this->base_coordinate);

			this->searcher = std::make_shared<RadialDspSearcher>(this->database, this->base_coordinate, this->database->getSpaceDimension(), this->lower_is_better, this->dsp_alpha);
			this->searching_phase = 1;
			if (this->is_logging_mode_on) {
				std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
				ld->updateCandidateList(this->searcher->getSuggestedList());
			}
			break;
		}
	}

	Operator_P_2024B::Operator_P_2024B(const Operator_P_2024B& original) {

		this->searching_phase = original.searching_phase;
		this->lower_is_better = original.lower_is_better;
		this->is_logging_mode_on = original.is_logging_mode_on;
		this->dsp_alpha = original.dsp_alpha;
		this->base_coordinate = original.base_coordinate;
		this->base_value = original.base_value;
		this->loop_count = original.loop_count;

		if (std::shared_ptr<StandardDatabase> sd = std::dynamic_pointer_cast<StandardDatabase>(original.database)) {
			this->database = std::make_shared<StandardDatabase>(*sd);
		}
		else {
			this->database = std::make_shared<LoggingDatabase>(*(std::dynamic_pointer_cast<LoggingDatabase>(original.database)));
		}

		switch (this->searching_phase) {
		case 0:
			this->searcher = std::make_shared<SimpleLhdSearcher>(*(std::dynamic_pointer_cast<SimpleLhdSearcher>(original.searcher)));
			break;
		case 1:
			this->searcher = std::make_shared<RadialDspSearcher>(*(std::dynamic_pointer_cast<RadialDspSearcher>(original.searcher)));
			break;
		case 2:
			this->searcher = std::make_shared<UniMeasurer>(*(std::dynamic_pointer_cast<UniMeasurer>(original.searcher)));
			break;
		}
	}

	Operator_P_2024B::~Operator_P_2024B(void) {
		// DO_NOTHING
	}

	const coordinate& Operator_P_2024B::getBaseCoordinate(void) const {
		return this->base_coordinate;
	}

	const coordinate& Operator_P_2024B::getSuggested(void) {

		this->updateState();

		return this->searcher->getSuggested();
	}

	const coordinate_list& Operator_P_2024B::getSuggestedList(void) {

		this->updateState();

		return this->searcher->getSuggestedList();
	}

	void Operator_P_2024B::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->database->setSampleMetricValue(measured_coordinate, metric_value);
		this->searcher->setMetricValue(measured_coordinate, metric_value);
	}

	void Operator_P_2024B::updateState(void) {

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

				this->base_coordinate = this->searcher->getBestMeasuredCoordinate();
				this->database->setBasePoint(this->base_coordinate);
				this->base_value = this->database->getSampleMetricValue(this->base_coordinate);

				this->searcher = std::make_shared<RadialDspSearcher>(this->database, this->base_coordinate, this->database->getSpaceDimension(), this->lower_is_better, this->dsp_alpha);
				this->searching_phase = 1;
				if (this->is_logging_mode_on) {
					std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
					ld->updateCandidateList(this->searcher->getSuggestedList());
				}

				// SimpleLhdSearcher も RadialDspSearcher も最良推定点を実測するまで終了しないため，ここで終了の場合は直接 Phase 2 に移行できる
				if (this->searcher->isSearchFinished()) {

					const std::unordered_set<DirectionLine>& searched_directions = std::dynamic_pointer_cast<RadialDspSearcher>(this->searcher)->getFinishedDirections();
					for (const DirectionLine& direction : searched_directions) {
						this->database->recordSearchedDirection(this->base_coordinate, direction);
					}

					this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
					this->searching_phase = 2;
					if (this->is_logging_mode_on) {
						std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
						ld->updateCandidateList(this->searcher->getSuggestedList());
					}
				}

				break;
			case 1:

				const std::unordered_set<DirectionLine>& searched_directions = std::dynamic_pointer_cast<RadialDspSearcher>(this->searcher)->getFinishedDirections();
				for (const DirectionLine& direction : searched_directions) {
					this->database->recordSearchedDirection(this->base_coordinate, direction);
				}

				const coordinate& tmp_good_coordinate = this->searcher->getBestMeasuredCoordinate();
				double tmp_good_value = this->database->getSampleMetricValue(tmp_good_coordinate);
				
				if (this->lower_is_better) {
					if (tmp_good_value < this->base_value) {

						DirectionLine trajectry_line = DirectionLine(this->base_coordinate, tmp_good_coordinate);
						this->base_coordinate = tmp_good_coordinate;
						this->database->setBasePoint(this->base_coordinate);
						this->database->recordSearchedDirection(this->base_coordinate, trajectry_line);
						this->base_value = tmp_good_value;
						this->searcher = std::make_shared<RadialDspSearcher>(this->database, this->base_coordinate, this->database->getSpaceDimension(), this->lower_is_better, this->dsp_alpha);

						if (this->searcher->isSearchFinished()) {
							this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
							this->searching_phase = 2;
						}
					}
					else {
						this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
						this->searching_phase = 2;
					}
				}
				else {
					if (tmp_good_value > this->base_value) {
						
						DirectionLine trajectry_line = DirectionLine(this->base_coordinate, tmp_good_coordinate);
						this->base_coordinate = this->searcher->getBestMeasuredCoordinate();
						this->database->setBasePoint(this->base_coordinate);
						this->database->recordSearchedDirection(this->base_coordinate, trajectry_line);
						this->base_value = tmp_good_value;
						
						this->searcher = std::make_shared<RadialDspSearcher>(this->database, this->base_coordinate, this->database->getSpaceDimension(), this->lower_is_better, this->dsp_alpha);
						if (this->is_logging_mode_on) {
							std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
							ld->updateCandidateList(this->searcher->getSuggestedList());
						}
						
						if (this->searcher->isSearchFinished()) {
							this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
							this->searching_phase = 2;
							if (this->is_logging_mode_on) {
								std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
								ld->updateCandidateList(this->searcher->getSuggestedList());
							}
						}
					}
					else {
						this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
						this->searching_phase = 2;
						if (this->is_logging_mode_on) {
							std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
							ld->updateCandidateList(this->searcher->getSuggestedList());
						}
					}
				}
				break;
			}
		}
	}

	bool Operator_P_2024B::isSearchFinished(void) {
		return this->searching_phase == 2;
	}

	const coordinate& Operator_P_2024B::getBestJudgedCoordinate(void) {
		return this->base_coordinate;
	}

	const coordinate& Operator_P_2024B::getBestMeasuredCoordinate(void) {
		return this->base_coordinate;
	}

	std::string Operator_P_2024B::getAlgorithmId(void) const {
		return "P_2024B";
	}

	std::shared_ptr<DatabaseCore> Operator_P_2024B::getDatabase(void) const {
		return this->database;
	}

	std::size_t Operator_P_2024B::getLoopCount(void) const {
		return this->loop_count;
	}

	std::string Operator_P_2024B::getSearchModeName(void) const {
		switch (this->searching_phase) {
		case 0:
			return "Initial Search";
		case 1:
			return "Radial d-Spline Search";
		default:
			return "Finished";
		}
	}
}