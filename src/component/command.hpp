#pragma once

namespace command
{
	class params
	{
	public:
		params();

		int size() const;
		const char* get(int index) const;
		std::string join(int index) const;
		std::vector<std::string> get_all() const;

		const char* operator[](const int index) const	
		{
			return this->get(index); //
		}

	private:
		DWORD nesting_;
	};

	void add_raw(const char* name, void (*callback)());
	void add(const char* name, const std::function<void(const params&)>& callback);
	void add(const char* name, const std::function<void()>& callback);
	void execute(std::string command);
}