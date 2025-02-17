#include <stdexcept>
#include <memory>
#include <string>

#include "dsice_search_space.hpp"
#include "dsice_database_metric_type.hpp"
#include "dsice_database_standard_functions.hpp"
#include "dsice_standard_database.hpp"
#include "dsice_logging_database.hpp"
#include "uni_measurer.hpp"
#include "simple_direction_searcher.hpp"
#include "line_single_tri_searcher.hpp"
#include "one_dim_dsp_searcher.hpp"
#include "dsice_operator_s_2017.hpp"

namespace dsice {

	Operator_S_2017::Operator_S_2017(const space_size& parameters, bool low_value_is_better, bool logging_on, double alpha, DatabaseMetricType metric_type) {

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

		// 中心点を初期点とする
		coordinate center_point;
		for (std::size_t param_length : parameters) {
			if (param_length == 0) {
				throw std::invalid_argument("All parameters must have configurable values.");
			}
			center_point.push_back(param_length / 2);
		}

		this->base_coordinate = center_point;
		this->database->setBasePoint(center_point);

		this->searcher = std::make_shared<UniMeasurer>(center_point);
		if (this->is_logging_mode_on) {
			std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
			ld->updateCandidateList(this->searcher->getSuggestedList());
		}
	}

	Operator_S_2017::Operator_S_2017(const Operator_S_2017& original) {

		this->searching_phase = original.searching_phase;
		this->lower_is_better = original.lower_is_better;
		this->is_logging_mode_on = original.is_logging_mode_on;
		this->base_coordinate = original.base_coordinate;
		this->searching_line = original.searching_line;
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
			this->searcher = std::make_shared<UniMeasurer>(*(std::dynamic_pointer_cast<UniMeasurer>(original.searcher)));
			break;
		case 1:
			this->searcher = std::make_shared<SimpleDirectionSearcher>(*(std::dynamic_pointer_cast<SimpleDirectionSearcher>(original.searcher)));
			break;
		case 2:
			this->searcher = std::make_shared<LineSingleTriSearcher>(*(std::dynamic_pointer_cast<LineSingleTriSearcher>(original.searcher)));
			break;
		case 3:
			this->searcher = std::make_shared<OneDimDspSearcher>(*(std::dynamic_pointer_cast<OneDimDspSearcher>(original.searcher)));
			break;
		default:
			this->searcher = std::make_shared<UniMeasurer>(*(std::dynamic_pointer_cast<UniMeasurer>(original.searcher)));
			break;
		}
	}

	Operator_S_2017::~Operator_S_2017(void) {
		// DO_NOTHING
	}

	const coordinate& Operator_S_2017::getBaseCoordinate(void) const {
		return this->base_coordinate;
	}

	const coordinate& Operator_S_2017::getSuggested(void) {

		this->updateState();

		return this->searcher->getSuggested();
	}

	const coordinate_list& Operator_S_2017::getSuggestedList(void) {

		this->updateState();

		return this->searcher->getSuggestedList();
	}

	void Operator_S_2017::setMetricValue(const coordinate& measured_coordinate, double metric_value) {
		this->database->setSampleMetricValue(measured_coordinate, metric_value);
		this->searcher->setMetricValue(measured_coordinate, metric_value);
	}

	bool Operator_S_2017::isSearchFinished(void) {

		this->updateState();

		return this->searching_phase == 4;
	}

	const coordinate& Operator_S_2017::getBestJudgedCoordinate(void) {

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

	const coordinate& Operator_S_2017::getBestMeasuredCoordinate(void) {

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

	void Operator_S_2017::updateState(void) {

		bool searcher_updated = this->searcher->updateState();

		if (!searcher_updated) {
			return;
		}

		this->loop_count++;

		// ログなので終了条件は無関係に記録でよい
		if (this->is_logging_mode_on) {
			std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
			ld->updateCandidateList(this->searcher->getSuggestedList());
		}
		this->database->setLoopEnd();

		if (this->searcher->isSearchFinished()) {

			bool phase_changing = true;

			while (phase_changing) {
				switch (this->searching_phase) {
				case 0:

					this->searcher = std::make_shared<SimpleDirectionSearcher>(this->database, this->base_coordinate, this->database->getSpaceDimension(), this->lower_is_better);
					this->searching_phase = 1;

					if (this->is_logging_mode_on) {
						std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
						ld->updateCandidateList(this->searcher->getSuggestedList());
					}

					// 周辺点が存在しない，つまり探索空間に座標が 1 点しか存在しない場合はここで終了
					if (this->searcher->isSearchFinished()) {
						this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
						this->searching_phase = 4;
						if (this->is_logging_mode_on) {
							std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
							ld->updateCandidateList(this->searcher->getSuggestedList());
						}
					}

					phase_changing = false;
					break;
				case 1:

					// 方向探索で最良判定された方向以外は今後見るつもりがない
					// 軸数追加後のためにここで全方向を記録しておく
					for (auto d : std::dynamic_pointer_cast<SimpleDirectionSearcher>(this->searcher)->getMeasuredDirection()) {
						this->database->recordSearchedDirection(this->base_coordinate, d);
					}

					// 周辺点より基準点の方が良い場合は JudgeCoordinate が基準点になってしまうため MeasuredCoordinate を使う
					this->searching_line = std::make_shared<CoordinateLine>(this->database->getSpaceSize(), this->base_coordinate, this->searcher->getBestMeasuredCoordinate());

					this->searcher = std::make_shared<LineSingleTriSearcher>(this->database, *(this->searching_line), this->lower_is_better);
					this->searching_phase = 2;
					if (this->is_logging_mode_on) {
						std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
						ld->updateCandidateList(this->searcher->getSuggestedList());
					}

					// 4 分割点が探索済なら近似へ
					if (this->searcher->isSearchFinished()) {
						break;
					}

					phase_changing = false;
					break;
				case 2:

					this->searcher = std::make_shared<OneDimDspSearcher>(this->database, *(this->searching_line), this->lower_is_better);
					this->searching_phase = 3;
					if (this->is_logging_mode_on) {
						std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
						ld->updateCandidateList(this->searcher->getSuggestedList());
					}

					// 初期近似で終了条件を満たした場合
					if (this->searcher->isSearchFinished()) {
						break;
					}

					phase_changing = false;
					break;
				case 3:

					if (this->base_coordinate == this->searcher->getBestJudgedCoordinate()) {
						this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
						this->searching_phase = 4;
						if (this->is_logging_mode_on) {
							std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
							ld->updateCandidateList(this->searcher->getSuggestedList());
						}
					}
					else {

						this->base_coordinate = this->searcher->getBestJudgedCoordinate();
						this->database->setBasePoint(this->base_coordinate);
						this->database->recordSearchedDirection(this->base_coordinate, this->searching_line->getLineDirection());

						this->searching_line = nullptr;
						this->searcher = std::make_shared<SimpleDirectionSearcher>(this->database, this->base_coordinate, this->database->getSpaceDimension(), this->lower_is_better);
						this->searching_phase = 1;
						if (this->is_logging_mode_on) {
							std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
							ld->updateCandidateList(this->searcher->getSuggestedList());
						}

						if (this->searcher->isSearchFinished()) {
							this->searcher = std::make_shared<UniMeasurer>(this->base_coordinate);
							this->searching_phase = 4;
							if (this->is_logging_mode_on) {
								std::shared_ptr<LoggingDatabase> ld = std::dynamic_pointer_cast<LoggingDatabase>(this->database);
								ld->updateCandidateList(this->searcher->getSuggestedList());
							}
							break;
						}
					}

					phase_changing = false;
					break;
				default:
					// DO_NOTHING
					phase_changing = false;
					break;
				}
			}
		}
	}

	std::shared_ptr<DatabaseCore> Operator_S_2017::getDatabase(void) const {
		return this->database;
	}

	std::string Operator_S_2017::getAlgorithmId(void) const {
		return "S_2017";
	}

	std::size_t Operator_S_2017::getLoopCount(void) const {
		return this->loop_count;
	}

	std::string Operator_S_2017::getSearchModeName(void) const {
		switch (this->searching_phase) {
		case 0:
			return "Initial Search";
		case 1:
			return "Direction Search";
		case 2:
			return "One Dimensional d-Spline Search (Preparing Approxymation)";
		case 3:
			return "One Dimensional d-Spline Search";
		default:
			return "Finished";
		}
	}

	std::shared_ptr<const ObservedDSpline> Operator_S_2017::getUsedDSpline(void) const {

		if (std::shared_ptr<OneDimDspSearcher> s = std::dynamic_pointer_cast<OneDimDspSearcher>(this->searcher)) {
			return s->getUsedDSpline();
		}
		else {
			return nullptr;
		}
	}
}