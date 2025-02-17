#ifndef DSICE_DATABASE_METRIC_TYPE_HPP_
#define DSICE_DATABASE_METRIC_TYPE_HPP_

namespace dsice {

	/// <summary>
	/// DSICE のデータベースが保持する性能評価値のふるまいを指定する列挙型
	/// </summary>
	enum class DatabaseMetricType {
		OverwrittenDouble,
		AverageDouble
	};
}

#endif // !DSICE_DATABASE_METRIC_TYPE_HPP_
