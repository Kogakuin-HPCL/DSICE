#include <memory>

#include "dsice_searchers_manager.hpp"

namespace dsice {

	ManagedSearcher::ManagedSearcher(std::shared_ptr<Searcher> target_searcher, bool running) {
		this->searcher = target_searcher;
		this->is_running = running;
	}

	ManagedSearcher::ManagedSearcher(const ManagedSearcher& original) {
		this->searcher = original.searcher;
		this->is_running = original.is_running;
	}

	ManagedSearcher::~ManagedSearcher(void) {
		// DO_NOTHING
	}

	ManagedSearcher& ManagedSearcher::operator=(const ManagedSearcher& original) {
		
		this->searcher = original.searcher;
		this->is_running = original.is_running;

		return *this;
	}

	std::shared_ptr<Searcher> ManagedSearcher::getSearcher(void) const {
		return this->searcher;
	}

	bool ManagedSearcher::isRunning(void) const {
		return this->is_running;
	}

	void ManagedSearcher::setRunningState(bool state) {
		this->is_running = state;
	}
}