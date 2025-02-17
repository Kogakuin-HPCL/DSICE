#ifndef DSICE_DATABASE_STANDARD_FUNCTIONS_HPP_
#define DSICE_DATABASE_STANDARD_FUNCTIONS_HPP_

#include "dsice_database_base_point_extension.hpp"
#include "dsice_database_latest_sample_extension.hpp"
#include "dsice_database_latest_base_extension.hpp"

namespace dsice {

	/// <summary>
	/// DSICE のデータベース型における標準的な機能を一通り定義したインタフェースセット
	/// </summary>
	class DatabaseStandardFunctions :
		virtual public DatabaseBasePointExtension,
		virtual public DatabaseLatestSampleExtension,
		virtual public DatabaseLatestBaseExtension {};
}

#endif // !DSICE_DATABASE_STANDARD_FUNCTIONS_HPP_
