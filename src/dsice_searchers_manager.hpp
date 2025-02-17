#ifndef DSICE_SEARCHERS_MANAGER_HPP_
#define DSICE_SEARCHERS_MANAGER_HPP_

#include <memory>

#include "dsice_searcher.hpp"

namespace dsice {

	/// <summary>
	/// 探索機構とその稼働状況を保持する型
	/// </summary>
	class ManagedSearcher final {
	private:
		/// <summary>
		/// 稼働状況 (稼働していれば true)
		/// </summary>
		bool is_running;
		/// <summary>
		/// 探索機構
		/// </summary>
		std::shared_ptr<Searcher> searcher;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="target_searcher">格納する探索機構</param>
		/// <param name="running">稼働状況 (稼働していれば true)</param>
		ManagedSearcher(std::shared_ptr<Searcher> target_searcher, bool running = true);
		/// <summary>
		/// コピーコンストラクタ (探索機構はポインタコピー)
		/// </summary>
		/// <param name="original">コピー元</param>
		ManagedSearcher(const ManagedSearcher& original);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~ManagedSearcher(void);
		/// <summary>
		/// コピー代入演算子
		/// </summary>
		/// <param name="original">コピー元</param>
		/// <returns>代入先 (*this)</returns>
		ManagedSearcher& operator=(const ManagedSearcher& original);
		/// <summary>
		/// 探索機構を取得
		/// </summary>
		/// <returns>探索機構</returns>
		std::shared_ptr<Searcher> getSearcher(void) const;
		/// <summary>
		/// 探索機構が稼働しているかどうか
		/// </summary>
		/// <returns>探索機構が稼働していれば true</returns>
		bool isRunning(void) const;
		/// <summary>
		/// 探索機構の稼働状況を設定
		/// </summary>
		/// <param name="state">探索機構の稼働状況</param>
		void setRunningState(bool state);
	};

	/// <summary>
	/// 複数の Searcher を同時に稼働させ，それぞれの探索推奨点を統括した独自の探索推奨点リストを生成する機構
	/// </summary>
	class SearchersManager : public Searcher {
		/// <summary>
		/// 管理中の内部探索機構の数を取得
		/// </summary>
		/// <returns>管理中の内部探索機構の数</returns>
		virtual std::size_t getManagedSearchersNum(void) const = 0;
		/// <summary>
		/// 管理中内部探索機構のうち，まだ終了条件を満たさずに稼働しているものの数を取得
		/// </summary>
		/// <returns>終了条件をまだ満たしていない内部探索機構の数</returns>
		virtual std::size_t getRunningSearchersNum(void) const = 0;
		/// <summary>
		/// 管理中内部探索機構のうち，終了条件を満たしたものの数を取得
		/// </summary>
		/// <returns>終了条件を満たした内部探索機構の数</returns>
		virtual std::size_t getFinishedSearchersNum(void) const = 0;
	};
}

#endif // !DSICE_SEARCHERS_MANAGER_HPP_
