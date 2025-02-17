#include <memory>
#include <vector>
#include <string>

#include "operation_integrator.hpp"
#include "dsice_tuner.hpp"

namespace dsice {

	/// <summary>
	/// private メンバを隠蔽するための Pimpl イディオム用クラス．<br/>
	/// これを Integrator にできなくはないが，ヘッダと実装の分離を徹底するためにポインタ所持の役割に徹する
	/// </summary>
	/// <typeparam name="T">性能パラメタの値型</typeparam>
	template <typename T>
	class Tuner<T>::TunerPrivateMembers {
	private:
		/// <summary>
		/// 隠蔽された統合層
		/// </summary>
		std::shared_ptr<OperationIntegrator<T>> integrator = nullptr;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		TunerPrivateMembers(void);
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="parameters">性能パラメタの取りうる値リスト</param>
		TunerPrivateMembers(const std::vector<std::vector<T>>& parameters);
		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="original">コピー元</param>
		TunerPrivateMembers(const TunerPrivateMembers& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~TunerPrivateMembers(void);
		/// <summary>
		/// 統合層へのアクセス
		/// </summary>
		/// <returns>統合層のポインタ</returns>
		std::shared_ptr<OperationIntegrator<T>> Integrator(void);
	};

	template <typename T>
	Tuner<T>::TunerPrivateMembers::TunerPrivateMembers(void) {
		this->integrator = std::make_shared<OperationIntegrator<T>>();
	}

	template <typename T>
	Tuner<T>::TunerPrivateMembers::TunerPrivateMembers(const std::vector<std::vector<T>>& parameters) {
		this->integrator = std::make_shared<OperationIntegrator<T>>(parameters);
	}

	template <typename T>
	Tuner<T>::TunerPrivateMembers::TunerPrivateMembers(const TunerPrivateMembers& original) {
		this->integrator = std::make_shared<OperationIntegrator<T>>(*(original.integrator));
	}

	template <typename T>
	Tuner<T>::TunerPrivateMembers::~TunerPrivateMembers(void) {
		// DO_NOTHING
	}

	template <typename T>
	std::shared_ptr<OperationIntegrator<T>> Tuner<T>::TunerPrivateMembers::Integrator(void){
		return this->integrator;
	}

	//
	// Tuner
	//

	template <typename T>
	Tuner<T>::Tuner(void) {
		this->private_members = std::make_shared<Tuner<T>::TunerPrivateMembers>();
	}

	template <typename T>
	Tuner<T>::Tuner(const std::vector<std::vector<T>>& parameters) {
		this->private_members = std::make_shared<Tuner<T>::TunerPrivateMembers>(parameters);
	}

	template <typename T>
	Tuner<T>::Tuner(const Tuner& original) {
		this->private_members = std::make_shared<Tuner<T>::TunerPrivateMembers>(*(original.private_members));
	}

	template <typename T>
	Tuner<T>::~Tuner(void) {
		// DO_NOTHING
	}

	template <typename T>
	void Tuner<T>::appendParameter(const std::vector<T>& parameter) {
		this->private_members->Integrator()->appendParameter(parameter);
	}

	template <typename T>
	void Tuner<T>::appendParameterByLinearSpace(const T& min, const T& max, const T& space_length) {
		this->private_members->Integrator()->appendParameterByLinearSpace(min, max, space_length);
	}

	template <typename T>
	void Tuner<T>::selectMode(ExecutionMode execution_mode) {
		this->private_members->Integrator()->selectMode(execution_mode);
	}
	
	template <typename T>
	void Tuner<T>::config_doInitialSearch(bool do_init_search) {
		this->private_members->Integrator()->config_doInitialSearch(do_init_search);
	}

	template <typename T>
	void Tuner<T>::config_specifyInitialParameter(const std::vector<T>& initial_parameter) {
		this->private_members->Integrator()->config_specifyInitialParameter(initial_parameter);
	}

	template <typename T>
	void Tuner<T>::config_recordLog(bool record_log) {
		this->private_members->Integrator()->config_recordLog(record_log);
	}

	template <typename T>
	void Tuner<T>::config_lowerValueIsBetter(bool lower_value_is_better) {
		this->private_members->Integrator()->config_lowerValueIsBetter(lower_value_is_better);
	}

	template <typename T>
	void Tuner<T>::config_higherValueIsBetter(bool higher_value_is_better) {
		this->private_members->Integrator()->config_higherValueIsBetter(higher_value_is_better);
	}

	template <typename T>
	void Tuner<T>::config_setParallelMode(bool parallel_mode) {
		this->private_members->Integrator()->config_setParallelMode(parallel_mode);
	}

	template <typename T>
	void Tuner<T>::config_setDSplineAlpha(double alpha) {
		this->private_members->Integrator()->config_setDSplineAlpha(alpha);
	}

	template <typename T>
	void Tuner<T>::config_setDatabaseMetricType(DatabaseMetricType metric_type) {
		this->private_members->Integrator()->config_setDatabaseMetricType(metric_type);
	}

	template <typename T>
	const std::vector<T>& Tuner<T>::getSuggestedNext(void) {
		return this->private_members->Integrator()->getSuggestedNext();
	}

	template <typename T>
	const std::vector<std::vector<T>>& Tuner<T>::getSuggestedList(void) {
		return this->private_members->Integrator()->getSuggestedList();
	}

	template <typename T>
	void Tuner<T>::setMetricValue(double metric_value) {
		this->private_members->Integrator()->setMetricValue(metric_value);
	}

	template <typename T>
	std::size_t Tuner<T>::getParametersNum(void) const {
		return this->private_members->Integrator()->getParametersNum();
	}

	template <typename T>
	std::size_t Tuner<T>::getParameterLength(std::size_t parameter_idx) const {
		return this->private_members->Integrator()->getParameterLength(parameter_idx);
	}

	template <typename T>
	const std::vector<std::vector<T>>& Tuner<T>::getTargetParameters(void) const {
		return this->private_members->Integrator()->getTargetParameters();
	}

	template <typename T>
	std::vector<T> Tuner<T>::getTentativeBestParameter(void) const {
		return this->private_members->Integrator()->getTentativeBestParameter();
	}

	template <typename T>
	bool Tuner<T>::isSearchFinished(void) const {
		return this->private_members->Integrator()->isSearchFinished();
	}

	template <typename T>
	void Tuner<T>::setTimePerformance(void) {
		this->private_members->Integrator()->setTimePerformance();
	}

	template <typename T>
	void Tuner<T>::setMetricValuesList(const std::vector<double>& metric_values_list) {
		this->private_members->Integrator()->setMetricValuesList(metric_values_list);
	}

	template <typename T>
	void Tuner<T>::printTuningResult(std::string file_path) const {
		this->private_members->Integrator()->printTuningResult(file_path);
	}

	template <typename T>
	void Tuner<T>::printSimpleLoopLog(std::string file_path) const {
		this->private_members->Integrator()->printSimpleLoopLog(file_path);
	}

	template <typename T>
	void Tuner<T>::outputSearchSpaceForViewer(std::string file_path, double default_value) const {
		this->private_members->Integrator()->outputSearchSpaceForViewer(file_path, default_value);
	}

	template <typename T>
	void Tuner<T>::outputLogForViewer(std::string file_path) const {
		this->private_members->Integrator()->outputLogForViewer(file_path);
	}

	// コンパイルさせるための実態を記述
	template class Tuner<int>;
	template class Tuner<long long>;
	template class Tuner<std::size_t>;
	template class Tuner<float>;
	template class Tuner<double>;
}