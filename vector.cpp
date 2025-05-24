#include <iostream>
#include <new>
#include <initializer_list>
#include <cassert>

template <typename T, typename Allocator = std::allocator<T>>
class Vector {

	// typedefs / usings
	using iterator = T*;
	using reverse_iterator = T*;
	using const_iterator = const T*;
	using const_reverse_iterator = const T*;

	using size_type = size_t;
	using reference = T&;
	using const_reference = const T&;


	// forward declarations

	void ensure_capacity();
	size_type size() const noexcept;
	reference operator[](size_type pos) noexcept;
	const_reference operator[](size_type pos) const noexcept;

private:

	T* _data;
	size_t _size;
	size_t _capacity;

	Allocator _allocator;



public:

	// element access

	reference at(size_type pos) noexcept(false) {

		if (pos >= size())
			throw std::out_of_range("Index out of range");

		return _data[pos];


	}

	const_reference at(size_type pos) const noexcept(false) {

		if (pos >= size())
			throw std::out_of_range("Index out of range");

		return _data[pos];


	}




	reference operator[](size_type pos) noexcept {

		return _data[pos];

	}

	const_reference operator[](size_type pos) const noexcept {

		return _data[pos];

	}

	reference front() noexcept {

		assert(size() >= 1 && "No elements in vector");

		return *_data;

	}

	const_reference front() const noexcept {

		assert(size() >= 1 && "No elements in vector");

		return *_data;

	}

	reference back() noexcept {

		assert(size() >= 1 && "No elements in vector");

		return _data[_size - 1];

	}

	const_reference back() const noexcept {

		assert(size() >= 1 && "No elements in vector");

		return _data[_size - 1];

	}

	T* data() noexcept {

		return _data;

	}

	const T* data() const noexcept {

		return _data;
	}













	//iterators

	iterator begin() noexcept { return _data; };
	iterator end() noexcept { return _data + _size; };

	const_iterator begin() const noexcept { return _data; };
	const_iterator end() const noexcept { return _data + _size; };
	const_iterator cbegin() const noexcept { return _data; };
	const_iterator cend() const noexcept { return _data + _size; };

	//reverse_iterator rbegin noexcept{ return end(); };
	//const_reverse_iterator rbegin const noexcept{ return end(); };
	//const_reverse_iterator rcbegin const noexcept{ return end(); };

	//reverse_iterator rend() noexcept { return begin() - 1 };
	//const_reverse_iterator rend() const noexcept { return begin() - 1 };
	//const_reverse_iterator rcend() const noexcept { return begin() - 1 };




	//capacity

	bool empty() const noexcept {

		return _size == 0;
	}

	size_type size() const noexcept {
		return _size;
	}

	void reserve(size_type newCapacity) {

		if (newCapacity <= _capacity)
			return;


		T* allocatedMemory = _allocator.allocate(newCapacity);

		if (_data != nullptr) {


			for (size_t i{}; i < _size; ++i)
			{
				_allocator.construct(allocatedMemory + i, std::move_if_noexcept(_data[i])); // may fail |  revert if yes
				_allocator.destroy(_data + i);
			}


			_allocator.deallocate(_data, _capacity);
		}

		_data = allocatedMemory;
		_capacity = newCapacity;


	}

	size_type capacity() const noexcept {
		return _capacity;
	}

	void shrink_to_fit() {

		if (_capacity == _size)
			return;

		if (_size == 0) {

			_allocator.deallocate(_data, _capacity);
			_capacity = 0;
			_data = nullptr;
			return;
		}



		T* allocatedMemory = _allocator.allocate(_size);

		for (size_t i{}; i < _size; ++i)
		{
			_allocator.construct(allocatedMemory + i, std::move_if_noexcept(_data[i]));
			_allocator.destroy(_data + i);
		}


		_allocator.deallocate(_data, _capacity);


		_data = allocatedMemory;
		_capacity = _size;

	}





	// modifiers

	void clear() {

		assert(_data != nullptr || _size == 0);

		for (size_t i{}; i < _size; ++i) {

			_allocator.destroy(_data + i);
		}

		_size = 0;



	}

	void push_back(const T& item) {
		ensure_capacity();
		_allocator.construct(_data + _size, item);
		++_size;
	}

	void push_back(T&& item) {
		ensure_capacity();
		_allocator.construct(_data + _size, std::move(item));
		++_size;
	}

	void pop_back() {

		assert(_size > 0 && "pop_back() called on empty vector");
		_allocator.destroy(_data + _size - 1);
		--_size;

	}


	// constructors


	Vector() : _data{ nullptr }, _size{ 0 }, _capacity{ 0 } {}

	Vector(const std::initializer_list<T> items) : _data{ nullptr }, _size{ 0 }, _capacity{ 0 } {

		reserve(items.size());

		for (size_t i{}; i < items.size(); ++i) {

			_allocator.construct(_data + i, *(items.begin() + i));
		}

		_size = items.size();
		_capacity = items.size();

	}

	Vector(size_t numberOfDefaults, const T& item) : _data{ nullptr }, _size{ 0 }, _capacity{ 0 } {
		reserve(numberOfDefaults);
		for (size_t i = 0; i < numberOfDefaults; ++i) {
			_allocator.construct(_data + i, item);
		}
		_size = numberOfDefaults;
	}

	~Vector() {

		if (!_data)
			return;

		clear();

		_allocator.deallocate(_data, _capacity);

	}




private:


	void ensure_capacity() {

		if (_size == _capacity)
			reserve(_capacity == 0 ? 1 : _capacity * 2);

	}

};