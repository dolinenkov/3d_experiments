#include <xrndr/async/Async.hh>
#include <asio.hpp>

namespace xrndr
{

class ThreadPool::Impl
{
public:
	asio::thread_pool thread_pool = asio::thread_pool(128);
};

ThreadPool& ThreadPool::get()
{
	static ThreadPool threadPool(std::make_unique<Impl>());
	return threadPool;
}

void ThreadPool::post(std::function<void()> f)
{
	asio::post(impl->thread_pool, std::move(f));
}

ThreadPool::ThreadPool(std::unique_ptr<Impl> impl): impl(std::move(impl))
{
}

class ExecutionContext::Impl
{
public:
	asio::io_context io_context;
};

ExecutionContext& ExecutionContext::current()
{
	static thread_local ExecutionContext executionContext(std::make_unique<Impl>());
	return executionContext;
}

void ExecutionContext::post(std::function<void()> f)
{
	asio::post(impl->io_context, std::move(f));
}

void ExecutionContext::poll()
{
	impl->io_context.poll();
}

ExecutionContext::ExecutionContext(std::unique_ptr<Impl> impl): impl(std::move(impl))
{
}

}
