#pragma once
#include <memory>
#include <functional>

namespace xrndr {

class ThreadPool {
public:
	static ThreadPool& get();
	void post(std::function<void()>);

private:
	ThreadPool() = delete;
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	class Impl;
	explicit ThreadPool(std::unique_ptr<Impl>);
	std::unique_ptr<Impl> impl;
};

class ExecutionContext {
public:
	static ExecutionContext& current();
	void post(std::function<void()>);
	void poll();

private:
	ExecutionContext() = delete;
	ExecutionContext(const ExecutionContext&) = delete;
	ExecutionContext(ExecutionContext&&) = delete;
	ExecutionContext& operator=(const ExecutionContext&) = delete;
	ExecutionContext& operator=(ExecutionContext&&) = delete;

	class Impl;
	explicit ExecutionContext(std::unique_ptr<Impl>);
	std::unique_ptr<Impl> impl;
};

}
